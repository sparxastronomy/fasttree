#!/usr/bin/env python3
"""
Generate Benchmark Plots & HTML Reports
========================================
Static script to generate interactive Plotly HTML charts and summary reports
from benchmark result files in docs/benchmark_results/.
"""

import glob
import os
import re
from pathlib import Path
import pandas as pd

try:
    import plotly.express as px
    import plotly.graph_objects as go
    HAS_PLOTLY = True
except ImportError:
    HAS_PLOTLY = False


def parse_benchmark_file(file_path):
    data = []
    current_config = "Default"
    device_name = "CPU"
    precision_name = "double"
    periodic_status = "OFF"
    run_date = "Unknown Date"

    file_name = os.path.basename(file_path)
    if "gpu" in file_name.lower():
        device_name = "GPU"

    bench_pattern = re.compile(
        r"^(?P<name>\w+(?:-\w+)?)/(?P<particles>\d+[KMG])(?:/(?P<params>\S+))?\s+"
        r"(?P<time>[\d.]+)\s+(?P<time_unit>[mu]?s)\s+"
        r"(?P<cpu>[\d.]+)\s+(?P<cpu_unit>[mu]?s)\s+"
        r"(?P<iterations>\d+)\s*"
        r"(?:PeakRSS_MB=(?P<rss>[\d.]+[kK]?))?\s*"
        r"(?:items_per_second=(?P<throughput>[\d.]+[KMGk]?/s))?"
    )

    try:
        content = Path(file_path).read_text()
    except Exception as e:
        print(f"Could not read {file_path}: {e}")
        return pd.DataFrame()

    for line in content.splitlines():
        line = line.strip()
        if line.startswith("# Scaling Benchmark Results:"):
            title_part = line.replace("# Scaling Benchmark Results:", "").strip()
            current_config = title_part
            if "int32" in title_part:
                precision_name = "int32"
            elif "int64" in title_part:
                precision_name = "int64"
            elif "double" in title_part:
                precision_name = "double"
            if "Periodic BC: ON" in line or "periodic_on" in file_name.lower():
                periodic_status = "ON"
            else:
                periodic_status = "OFF"
            continue
        elif line.startswith("Date:"):
            run_date = line.replace("Date:", "").strip()
            continue
        elif line.startswith("Periodic BC:"):
            periodic_status = line.replace("Periodic BC:", "").strip()
            continue

        match = bench_pattern.search(line)
        if match:
            d = match.groupdict()
            time_val = float(d["time"])
            if d["time_unit"] == "us":
                time_val /= 1000.0
            elif d["time_unit"] == "s":
                time_val *= 1000.0

            rss_val = 0.0
            if d["rss"]:
                rss_str = d["rss"].lower()
                rss_val = float(rss_str[:-1]) * 1000 if rss_str.endswith("k") else float(rss_str)

            p_str = d["particles"]
            p_num = int(p_str[:-1]) * {"K": 10**3, "M": 10**6, "G": 10**9}[p_str[-1]]

            cfg_label = f"{precision_name} (Periodic: {periodic_status})"

            data.append({
                "Device": device_name,
                "Precision": precision_name,
                "Periodic_BC": periodic_status,
                "Configuration": cfg_label,
                "Test": d["name"],
                "Particles_Str": p_str,
                "Particles_Num": p_num,
                "Params": d["params"].strip() if d["params"] else "None",
                "Time_ms": time_val,
                "PeakRSS_MB": rss_val,
                "Source_File": file_name,
            })

    return pd.DataFrame(data)


def main():
    out_dir = Path("docs/benchmark_plots")
    out_dir.mkdir(parents=True, exist_ok=True)

    all_dfs = []
    for fpath in glob.glob("docs/benchmark_results/*.md"):
        df = parse_benchmark_file(fpath)
        if not df.empty:
            all_dfs.append(df)

    if not all_dfs:
        print("No benchmark result files found in docs/benchmark_results/")
        return

    df_all = pd.concat(all_dfs, ignore_index=True)
    print(f"Loaded {len(df_all)} benchmark data rows across {df_all['Source_File'].nunique()} files.")

    # Always generate standalone HTML table report
    html_report_path = out_dir / "benchmark_summary_report.html"
    html_content = f"""<!DOCTYPE html>
<html>
<head>
    <title>FastTree Benchmark Scaling Summary</title>
    <style>
        body {{ font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; margin: 30px; background-color: #f9f9f9; }}
        h1 {{ color: #2c3e50; }}
        table {{ border-collapse: collapse; width: 100%; margin-top: 20px; background: white; box-shadow: 0 1px 3px rgba(0,0,0,0.1); }}
        th, td {{ padding: 10px 15px; text-align: left; border-bottom: 1px solid #ddd; }}
        th {{ background-color: #34495e; color: white; }}
        tr:hover {{ background-color: #f1f1f1; }}
        .badge {{ background: #27ae60; color: white; padding: 3px 8px; border-radius: 4px; font-size: 0.9em; }}
    </style>
</head>
<body>
    <h1>FastTree Benchmark Scaling Summary Report</h1>
    <p>Total data rows: <span class="badge">{len(df_all)}</span> across <span class="badge">{df_all['Source_File'].nunique()} configurations</span></p>
    {df_all.to_html(classes="table", index=False)}
</body>
</html>
"""
    html_report_path.write_text(html_content)
    print(f"Generated HTML summary report: {html_report_path}")

    # Plotly interactive charts if available
    if HAS_PLOTLY:
        for test_name in df_all["Test"].unique():
            sub_df = df_all[df_all["Test"] == test_name].sort_values("Particles_Num")
            fig = px.line(
                sub_df,
                x="Particles_Str",
                y="Time_ms",
                color="Configuration",
                symbol="Periodic_BC",
                log_y=True,
                markers=True,
                title=f"Execution Time Scaling for {test_name}",
                labels={"Particles_Str": "Number of Particles", "Time_ms": "Time (ms)"},
                color_discrete_sequence=px.colors.qualitative.Bold,
            )
            fig.update_layout(height=600, width=900)

            html_path = out_dir / f"time_scaling_{test_name.lower()}.html"
            fig.write_html(str(html_path))
            print(f"Generated Plotly chart: {html_path}")
    else:
        print("Note: Install 'plotly' (pip install plotly) for interactive Plotly charts.")


if __name__ == "__main__":
    main()
