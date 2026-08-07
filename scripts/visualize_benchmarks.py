#!/usr/bin/env python3
"""
FastTree Benchmark Scaling Dashboard
=====================================
Interactive Streamlit dashboard to visualize single-node CPU/GPU scaling,
multi-precision performance (double, int32, int64), periodic BC impact,
and MPI domain decomposition scaling.
"""

import glob
import os
import re
import sys
from pathlib import Path

import pandas as pd
import plotly.express as px
import streamlit as st

# Detect if running in stlite/Pyodide
IS_PYODIDE = "pyodide" in sys.modules or "stlite" in sys.modules


# --- Helpers: Fetch Release & Parse Data ---
@st.cache_data(ttl=3600)
def get_latest_release(repo_path):
    try:
        if IS_PYODIDE:
            return "See GitHub"
        else:
            import requests

            url = f"https://api.github.com/repos/{repo_path}/releases/"
            response = requests.get(url)
            response.raise_for_status()
            return response.json()[0].get("tag_name", "Unknown Version")
    except Exception:
        return "Latest (API Fetch Failed)"


@st.cache_data
def parse_benchmark_file(file_path):
    """
    Parses Google Benchmark results from markdown files in docs/benchmark_results/
    or root benchmark results.
    """
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
        st.error(f"Could not read {file_path}: {e}")
        return pd.DataFrame(), "File Not Found"

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
        elif line.startswith("## Configuration:"):
            current_config = line.replace("## Configuration:", "").strip()
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
                rss_val = (
                    float(rss_str[:-1]) * 1000
                    if rss_str.endswith("k")
                    else float(rss_str)
                )

            tp_val = 0.0
            if d["throughput"]:
                tp_str = d["throughput"].replace("/s", "").strip()
                unit = tp_str[-1].upper() if tp_str[-1].isalpha() else ""
                val_num = float(tp_str[:-1]) if unit else float(tp_str)
                mult = {"K": 1e3, "M": 1e6, "G": 1e9}.get(unit, 1.0)
                tp_val = (val_num * mult) / 1e6  # Millions per second

            p_str = d["particles"]
            p_num = int(p_str[:-1]) * {"K": 10**3, "M": 10**6, "G": 10**9}[p_str[-1]]

            cfg_label = f"{precision_name} (Periodic: {periodic_status})"

            data.append({
                "Device": device_name,
                "Precision": precision_name,
                "Periodic_BC": periodic_status,
                "Configuration": cfg_label,
                "Raw_Config": current_config,
                "Test": d["name"],
                "Particles_Str": p_str,
                "Particles_Num": p_num,
                "Params": d["params"].strip() if d["params"] else "None",
                "Time_ms": time_val,
                "PeakRSS_MB": rss_val,
                "Throughput_M_s": tp_val,
                "Source_File": file_name,
            })

    return pd.DataFrame(data), run_date


@st.cache_data
def load_all_benchmark_files():
    """Scans and parses all markdown benchmark result files."""
    all_dfs = []
    dates = {}

    search_paths = [
        "docs/benchmark_results/scaling_*.md",
        "docs/benchmark_results/*.md",
    ]

    found_files = set()
    for pattern in search_paths:
        for fpath in glob.glob(pattern):
            if os.path.isfile(fpath) and fpath not in found_files:
                fname = os.path.basename(fpath)
                if fname.startswith("scaling_"):
                    found_files.add(fpath)
                    df, rdate = parse_benchmark_file(fpath)
                    if not df.empty:
                        all_dfs.append(df)
                        dates[fname] = rdate

    if not all_dfs:
        return pd.DataFrame(), dates

    combined_df = pd.concat(all_dfs, ignore_index=True)
    return combined_df, dates


@st.cache_data
def parse_mpi_benchmark_file(file_path, device_name):
    data = []
    current_config = "Default"
    current_ranks = 1
    current_dataset_str = ""
    current_dataset_num = 0

    try:
        with open(file_path, "r") as f:
            for line in f:
                line = line.strip()
                if line.startswith("## Configuration:"):
                    current_config = line.replace("## Configuration:", "").strip()
                elif line.startswith("MPI Ranks:"):
                    try:
                        current_ranks = int(line.split(":")[1].strip())
                    except ValueError:
                        pass
                elif line.startswith("Dataset:"):
                    match = re.search(r"Dataset:\s*(\w+)\s*\((\d+)\s*particles", line)
                    if match:
                        current_dataset_str = match.group(1)
                        current_dataset_num = int(match.group(2))
                elif re.match(r"^\d+\.\s+.*", line) or line.startswith(
                    "Total Domain Decomposition"
                ):
                    parts = re.split(r"\s{2,}", line)
                    if len(parts) >= 4:
                        data.append({
                            "Device": device_name,
                            "Configuration": current_config,
                            "MPI_Ranks": current_ranks,
                            "Particles_Str": current_dataset_str,
                            "Particles_Num": current_dataset_num,
                            "Phase": parts[0],
                            "Min_Time_s": float(parts[1]),
                            "Max_Time_s": float(parts[2]),
                            "Avg_Time_s": float(parts[3]),
                        })
    except FileNotFoundError:
        return pd.DataFrame()
    return pd.DataFrame(data)


def hex_to_rgba(hex_color, alpha):
    """Convert hex color to rgba string for Plotly shaded fills"""
    hex_color = hex_color.lstrip("#")
    return f"rgba({int(hex_color[0:2], 16)}, {int(hex_color[2:4], 16)}, {int(hex_color[4:6], 16)}, {alpha})"


# --- Dictionaries ---
labels_dict = {
    "Particles_Str": "Number of Particles",
    "Time_ms": "Execution Time (ms)",
    "PeakRSS_MB": "Peak RSS (MB)",
    "Throughput_M_s": "Throughput (M items/sec)",
    "Configuration": "Configuration",
    "Precision": "Precision Mode",
    "Periodic_BC": "Periodic BC",
    "Test": "Benchmark Test",
    "Device": "Hardware",
    "MPI_Ranks": "MPI Ranks",
}

legend_dict = dict(
    orientation="h",
    yanchor="bottom",
    y=-0.35,
    xanchor="center",
    x=0.5,
    title_text="Legend",
    font=dict(size=12),
    title_font=dict(size=14),
)

# --- Streamlit Dashboard UI Setup ---
st.set_page_config(layout="wide", page_title="FastTree Benchmark Scaling Dashboard")

st.markdown(
    "<h1 style='text-align: center;'>namespace fasttree :: Scaling & Precision Performance Dashboard</h1>",
    unsafe_allow_html=True,
)

repo_path = "sparxastronomy/fasttree"
latest_release = get_latest_release(repo_path)
st.markdown(
    f"""
    <div style='text-align: center; margin-bottom: 15px;'>
    <b>Repository:</b> <a href='https://github.com/{repo_path}'>{repo_path}</a> &nbsp;|&nbsp; 
    <b>Latest Release:</b> <code>{latest_release}</code>
    </div>
    """,
    unsafe_allow_html=True,
)
st.markdown("---")

# --- Session State for Page Navigation ---
if "current_page" not in st.session_state:
    st.session_state.current_page = "Single Node Scaling"

# --- Navigation Bar ---
nav_cols = st.columns([1, 2, 2, 2, 1])
with nav_cols[1]:
    if st.button(
        "Single Node Scaling",
        width="stretch",
        type="primary"
        if st.session_state.current_page == "Single Node Scaling"
        else "secondary",
    ):
        st.session_state.current_page = "Single Node Scaling"
        st.rerun()

with nav_cols[2]:
    if st.button(
        "Multi-Precision Comparison",
        width="stretch",
        type="primary"
        if st.session_state.current_page == "Multi-Precision Comparison"
        else "secondary",
    ):
        st.session_state.current_page = "Multi-Precision Comparison"
        st.rerun()

with nav_cols[3]:
    if st.button(
        "MPI Domain Decomposition",
        width="stretch",
        type="primary"
        if st.session_state.current_page == "MPI Domain Decomposition"
        else "secondary",
    ):
        st.session_state.current_page = "MPI Domain Decomposition"
        st.rerun()

st.markdown("---")

# --- Load Single-Node Benchmark Data ---
df_all, run_dates = load_all_benchmark_files()

# --- Page 1: Single Node Scaling ---
if st.session_state.current_page == "Single Node Scaling":
    if df_all.empty:
        st.warning(
            "No benchmark result files found in `docs/benchmark_results/`. "
            "Please run `./test/benchmark/run_cpu_scaling.sh` to generate the scaling logs."
        )
        st.stop()

    st.sidebar.header("Benchmark Filters")

    # Precision filter
    precisions = sorted(df_all["Precision"].unique())
    selected_precisions = st.sidebar.multiselect(
        "Select Precision Modes", precisions, default=precisions
    )

    # Periodic BC filter
    periodic_options = sorted(df_all["Periodic_BC"].unique())
    selected_periodic = st.sidebar.multiselect(
        "Select Periodic BC", periodic_options, default=periodic_options
    )

    # Test filter
    tests = sorted(df_all["Test"].unique())
    selected_tests = st.sidebar.multiselect(
        "Select Benchmark Tests", tests, default=[tests[0]] if tests else []
    )

    # Filter dataframe
    filtered_df = df_all[
        (df_all["Precision"].isin(selected_precisions))
        & (df_all["Periodic_BC"].isin(selected_periodic))
        & (df_all["Test"].isin(selected_tests))
    ].copy()

    # Sub-config filter for RangeQuery/KNNQuery
    needs_param_filter = any(t in ["RangeQuery", "KNNQuery"] for t in selected_tests)
    if needs_param_filter:
        valid_params = sorted(
            filtered_df[filtered_df["Params"] != "None"]["Params"].unique()
        )
        if len(valid_params) > 0:
            param_choice = st.sidebar.radio(
                "Parameter View Mode",
                ["Single Parameter (Recommended)", "All Parameters"],
            )
            if param_choice == "Single Parameter (Recommended)":
                # Pick a representative default like k=16 or R=1.000000
                default_idx = 0
                for idx, p in enumerate(valid_params):
                    if "k=16" in p or "1.000000" in p:
                        default_idx = idx
                        break
                selected_param = st.sidebar.selectbox(
                    "Select Parameter", valid_params, index=default_idx
                )
                filtered_df = filtered_df[
                    (filtered_df["Params"] == selected_param)
                    | (filtered_df["Params"] == "None")
                ].copy()
            else:
                selected_params = st.sidebar.multiselect(
                    "Select Parameters (R / k)", valid_params, default=valid_params
                )
                filtered_df = filtered_df[
                    filtered_df["Params"].isin(selected_params)
                    | (filtered_df["Params"] == "None")
                ].copy()

    if not filtered_df.empty:
        # Create a unique series label to group lines cleanly
        filtered_df["Series"] = filtered_df.apply(
            lambda r: (
                f"{r['Configuration']}"
                + (f" | {r['Test']}" if len(selected_tests) > 1 else "")
                + (
                    f" [{r['Params']}]"
                    if r["Params"] != "None" and needs_param_filter
                    else ""
                )
            ),
            axis=1,
        )

        st.subheader("Execution Time Scaling (Log Scale)", divider="red")

        fig_time = px.line(
            filtered_df.sort_values("Particles_Num"),
            x="Particles_Str",
            y="Time_ms",
            color="Series",
            line_group="Series",
            symbol="Test" if len(selected_tests) > 1 else None,
            log_y=True,
            markers=True,
            title=f"Execution Time Scaling | Tests: {', '.join(selected_tests)}",
            labels=labels_dict,
            color_discrete_sequence=px.colors.qualitative.Bold,
        )
        fig_time.update_layout(
            height=600,
            legend=legend_dict,
            font=dict(size=13),
        )
        st.plotly_chart(fig_time, width="stretch")

        st.markdown("---")
        st.subheader("Memory Usage Scaling (Peak RSS MB)", divider="red")

        # Deduplicate memory dataframe per configuration and particle count to prevent bar overlap
        mem_df = (
            filtered_df
            .groupby(
                ["Configuration", "Test", "Particles_Str", "Particles_Num"],
                as_index=False,
            )["PeakRSS_MB"]
            .mean()
            .sort_values("Particles_Num")
        )

        fig_mem = px.bar(
            mem_df,
            x="Particles_Str",
            y="PeakRSS_MB",
            color="Configuration",
            barmode="group",
            facet_col="Test" if len(selected_tests) > 1 else None,
            log_y=True,
            title="Peak Memory Scaling Across Datasets",
            labels=labels_dict,
            color_discrete_sequence=px.colors.qualitative.Bold,
        )
        fig_mem.update_layout(
            height=500,
            legend=legend_dict,
            font=dict(size=13),
        )
        st.plotly_chart(fig_mem, width="stretch")

        st.markdown("---")
        st.subheader("Raw Benchmark Data", divider="gray")
        sort_cols = [
            c
            for c in ["Test", "Configuration", "Particles_Num"]
            if c in filtered_df.columns
        ]
        sorted_df = filtered_df.sort_values(sort_cols) if sort_cols else filtered_df
        raw_cols = [
            c
            for c in [
                "Configuration",
                "Test",
                "Particles_Str",
                "Params",
                "Time_ms",
                "PeakRSS_MB",
                "Throughput_M_s",
            ]
            if c in sorted_df.columns
        ]
        st.dataframe(
            sorted_df[raw_cols],
            width="stretch",
        )
    else:
        st.warning("No data matches the selected filters.")

# --- Page 2: Multi-Precision Comparison ---
elif st.session_state.current_page == "Multi-Precision Comparison":
    if df_all.empty:
        st.warning("No benchmark data loaded.")
        st.stop()

    st.subheader(
        "Multi-Precision Performance Comparison (double vs. int32 vs. int64)",
        divider="red",
    )

    col1, col2 = st.columns(2)
    with col1:
        sel_test = st.selectbox(
            "Select Test to Compare", sorted(df_all["Test"].unique())
        )
    with col2:
        sel_periodic = st.radio("Periodic BC Mode", ["OFF", "ON"], horizontal=True)

    comp_df = df_all[
        (df_all["Test"] == sel_test) & (df_all["Periodic_BC"] == sel_periodic)
    ]

    if not comp_df.empty:
        fig_comp = px.bar(
            comp_df.sort_values("Particles_Num"),
            x="Particles_Str",
            y="Time_ms",
            color="Precision",
            barmode="group",
            title=f"Execution Time Comparison for {sel_test} (Periodic BC: {sel_periodic})",
            labels=labels_dict,
            color_discrete_sequence=px.colors.qualitative.Set1,
        )
        fig_comp.update_layout(height=500, legend=legend_dict)
        st.plotly_chart(fig_comp, width="stretch")

        # Compute speedup table relative to double
        st.subheader(
            "Speedup Relative to Double Precision (`double` time / `int` time)",
            divider="gray",
        )
        pivot_df = comp_df.pivot_table(
            index=["Particles_Str", "Params"],
            columns="Precision",
            values="Time_ms",
            aggfunc="mean",
        ).reset_index()

        if "double" in pivot_df.columns:
            for prec in ["int32", "int64"]:
                if prec in pivot_df.columns:
                    pivot_df[f"Speedup_{prec}"] = pivot_df["double"] / pivot_df[prec]

        st.dataframe(pivot_df, width="stretch")
    else:
        st.info("No data available for this comparison selection.")

# --- Page 3: MPI Domain Decomposition ---
elif st.session_state.current_page == "MPI Domain Decomposition":
    df_mpi_gpu = parse_mpi_benchmark_file(
        "mpi_benchmark_results_gpu_all_variations.md", "GPU"
    )
    df_mpi_cpu = parse_mpi_benchmark_file(
        "mpi_benchmark_results_cpu_all_variations.md", "CPU"
    )

    if df_mpi_gpu.empty and df_mpi_cpu.empty:
        st.info(
            "No MPI domain decomposition results file (`mpi_benchmark_results_*.md`) found. "
            "Single-node benchmarks are available under the 'Single Node Scaling' page."
        )
        st.stop()

    df_mpi = pd.concat([df_mpi_gpu, df_mpi_cpu], ignore_index=True)
    st.header("MPI Domain Decomposition Analysis")
    st.dataframe(df_mpi, width="stretch")
