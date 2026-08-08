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
import plotly.graph_objects as go
import streamlit as st

# Detect if running in stlite/Pyodide
IS_PYODIDE = "pyodide" in sys.modules or "stlite" in sys.modules


# --- Helpers: Fetch Release & Parse Data ---
@st.cache_data(ttl=3600)
def get_latest_release(repo_path):
    try:
        if IS_PYODIDE:
            return ("v.1.1.0", f"https://github.com/{repo_path}/releases/tag/v.1.1.0")
        else:
            import requests

            # Query tags API first so latest tag like v.1.1.0 is returned even if not drafted as formal release
            tags_url = f"https://api.github.com/repos/{repo_path}/tags"
            resp = requests.get(tags_url, timeout=5)
            if resp.status_code == 200 and resp.json():
                tag_name = resp.json()[0].get("name", "v.1.1.0")
                return (
                    tag_name,
                    f"https://github.com/{repo_path}/releases/tag/{tag_name}",
                )

            rel_url = f"https://api.github.com/repos/{repo_path}/releases/"
            resp = requests.get(rel_url, timeout=5)
            if resp.status_code == 200 and resp.json():
                tag_name = resp.json()[0].get("tag_name", "v.1.1.0")
                return (
                    tag_name,
                    f"https://github.com/{repo_path}/releases/tag/{tag_name}",
                )

            return ("v.1.1.0", f"https://github.com/{repo_path}/tags")
    except Exception:
        return ("v.1.1.0", f"https://github.com/{repo_path}/tags")


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
latest_tag, latest_tag_url = get_latest_release(repo_path)
st.markdown(
    f"""
    <div style='text-align: center; margin-bottom: 15px;'>
    <b>Repository:</b> <a href='https://github.com/{repo_path}'>{repo_path}</a> &nbsp;|&nbsp; 
    <b>Latest Release / Tag:</b> <a href='{latest_tag_url}'><code>{latest_tag}</code></a> &nbsp;|&nbsp;
    <a href='https://github.com/{repo_path}/tags'>All Tags</a> &nbsp;|&nbsp;
    <a href='https://github.com/{repo_path}/releases'>Releases</a>
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

    # Sub-config filter for RangeQuery/KNNQuery/SelfKNNQuery
    needs_param_filter = any(
        t in ["RangeQuery", "KNNQuery", "SelfKNNQuery"] for t in selected_tests
    )
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
        # Hardware Mode Selector before Execution Time Scaling
        st.subheader("Hardware View Mode", divider="red")
        available_devices = sorted(filtered_df["Device"].unique())
        default_dev_idx = 0
        if "Both" in available_devices or len(available_devices) > 1:
            device_options = ["CPU", "GPU", "Both"]
            # Keep only options present in data or Both if multiple exist
            device_view = st.radio(
                "Select Hardware Mode:", device_options, horizontal=True, index=0
            )
        else:
            device_view = available_devices[0]

        if device_view != "Both":
            plot_df = filtered_df[filtered_df["Device"] == device_view].copy()
        else:
            plot_df = filtered_df.copy()

        if not plot_df.empty:
            # Create a unique series label to group lines cleanly
            plot_df["Series"] = plot_df.apply(
                lambda r: (
                    f"{r['Configuration']}"
                    + (f" ({r['Device']})" if device_view == "Both" else "")
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

            line_dash_arg = (
                "Device"
                if device_view == "Both"
                else (
                    "Params"
                    if needs_param_filter and param_choice == "All Parameters"
                    else None
                )
            )
            dash_map = (
                {"GPU": "solid", "CPU": "dash"} if device_view == "Both" else None
            )

            fig_time = px.line(
                plot_df.sort_values("Particles_Num"),
                x="Particles_Str",
                y="Time_ms",
                color="Series",
                line_group="Series",
                line_dash=line_dash_arg,
                line_dash_map=dash_map,
                symbol="Test" if len(selected_tests) > 1 else None,
                log_y=True,
                markers=True,
                title=f"Execution Time Scaling | Hardware: {device_view} | Tests: {', '.join(selected_tests)}",
                labels=labels_dict,
                color_discrete_sequence=px.colors.qualitative.Bold,
            )
            # Increase line thickness and marker size for maximum clarity
            fig_time.update_traces(line=dict(width=3.5), marker=dict(size=8))

            # Multi-Model Theoretical Scaling Curve Fitting
            show_fits = st.sidebar.checkbox("Show Theoretical Fit Curves", value=True)
            if show_fits:
                import numpy as np

                fit_model_choice = st.sidebar.selectbox(
                    "Scaling Fit Model",
                    [
                        "Auto-Detect Best Fit (R²)",
                        "O(N) [Linear + Overhead: c0 + c1*N]",
                        "O(N log N) [Tree Sorting/Build: c0 + c1*N*log2(N)]",
                        "O(N^α) [Power Law: a * N^α]",
                    ],
                )

                for s_name, s_df in plot_df.groupby("Series"):
                    s_clean = s_df.dropna(
                        subset=["Particles_Num", "Time_ms"]
                    ).sort_values("Particles_Num")
                    if len(s_clean) >= 2:
                        x_vals = s_clean["Particles_Num"].values.astype(float)
                        y_vals = s_clean["Time_ms"].values.astype(float)

                        dev_type = (
                            s_df["Device"].iloc[0]
                            if "Device" in s_df.columns
                            else "CPU"
                        )
                        fit_color = (
                            "rgba(110, 110, 110, 0.75)"
                            if dev_type == "CPU"
                            else "rgba(60, 60, 60, 0.75)"
                        )
                        dash_fit = "dashdot" if dev_type == "CPU" else "dot"

                        # Candidate 1: Linear + Overhead (T = c0 + c1 * N)
                        A_lin = np.vstack([np.ones_like(x_vals), x_vals]).T
                        coeffs_lin, _, _, _ = np.linalg.lstsq(A_lin, y_vals, rcond=None)
                        c0_lin, c1_lin = (
                            max(0.0, coeffs_lin[0]),
                            max(0.0, coeffs_lin[1]),
                        )
                        y_fit_lin = c0_lin + c1_lin * x_vals

                        # Candidate 2: N log N + Overhead (T = c0 + c1 * N * log2(N))
                        n_logn = x_vals * np.log2(np.maximum(x_vals, 1.0))
                        A_nlogn = np.vstack([np.ones_like(x_vals), n_logn]).T
                        coeffs_nlogn, _, _, _ = np.linalg.lstsq(
                            A_nlogn, y_vals, rcond=None
                        )
                        c0_nlogn, c1_nlogn = (
                            max(0.0, coeffs_nlogn[0]),
                            max(0.0, coeffs_nlogn[1]),
                        )
                        y_fit_nlogn = c0_nlogn + c1_nlogn * n_logn

                        # Candidate 3: Power Law (T = a * N^alpha)
                        log_x = np.log10(x_vals)
                        log_y = np.log10(y_vals)
                        slope_alpha, intercept_c = np.polyfit(log_x, log_y, 1)
                        y_fit_pow = (10**intercept_c) * (x_vals**slope_alpha)

                        # Helper to compute R^2
                        y_mean = np.mean(y_vals)
                        ss_tot = np.sum((y_vals - y_mean) ** 2)

                        def calc_r2(y_fit):
                            if ss_tot == 0:
                                return 1.0
                            ss_res = np.sum((y_vals - y_fit) ** 2)
                            return max(0.0, 1.0 - (ss_res / ss_tot))

                        r2_lin = calc_r2(y_fit_lin)
                        r2_nlogn = calc_r2(y_fit_nlogn)
                        r2_pow = calc_r2(y_fit_pow)

                        # Select Model
                        if fit_model_choice == "O(N) [Linear + Overhead: c0 + c1*N]":
                            best_y, best_label, best_r2 = (
                                y_fit_lin,
                                f"Fit {s_name} [O(N) c0={c0_lin:.3f}ms]",
                                r2_lin,
                            )
                        elif (
                            fit_model_choice
                            == "O(N log N) [Tree Sorting/Build: c0 + c1*N*log2(N)]"
                        ):
                            best_y, best_label, best_r2 = (
                                y_fit_nlogn,
                                f"Fit {s_name} [O(N log N)]",
                                r2_nlogn,
                            )
                        elif fit_model_choice == "O(N^α) [Power Law: a * N^α]":
                            best_y, best_label, best_r2 = (
                                y_fit_pow,
                                f"Fit {s_name} [O(N^{slope_alpha:.2f})]",
                                r2_pow,
                            )
                        else:
                            # Auto-Detect Best Fit
                            candidates = [
                                (
                                    r2_lin,
                                    y_fit_lin,
                                    f"Fit {s_name} [O(N) c0={c0_lin:.2f}ms]",
                                ),
                                (r2_nlogn, y_fit_nlogn, f"Fit {s_name} [O(N log N)]"),
                                (
                                    r2_pow,
                                    y_fit_pow,
                                    f"Fit {s_name} [O(N^{slope_alpha:.2f})]",
                                ),
                            ]
                            candidates.sort(key=lambda item: item[0], reverse=True)
                            best_r2, best_y, best_label = candidates[0]

                        best_label += f" R²={best_r2:.3f}"

                        fig_time.add_trace(
                            go.Scatter(
                                x=s_clean["Particles_Str"],
                                y=best_y,
                                mode="lines",
                                name=best_label,
                                line=dict(color=fit_color, width=2, dash=dash_fit),
                                hoverinfo="text",
                                hovertext=f"Scaling Model: {best_label}",
                            )
                        )

            fig_time.update_layout(
                height=650,
                legend=legend_dict,
                font=dict(size=13),
            )
            st.plotly_chart(fig_time, width="stretch")

            st.markdown("---")
            st.subheader("Memory Usage Scaling (Peak RSS MB)", divider="red")

            # Deduplicate memory dataframe per configuration, device, and particle count
            mem_df = (
                plot_df
                .groupby(
                    [
                        "Configuration",
                        "Device",
                        "Test",
                        "Particles_Str",
                        "Particles_Num",
                    ],
                    as_index=False,
                )["PeakRSS_MB"]
                .mean()
                .sort_values("Particles_Num")
            )

            pattern_arg = "Device" if device_view == "Both" else None
            pattern_map = {"GPU": "/", "CPU": ""} if device_view == "Both" else None

            fig_mem = px.bar(
                mem_df,
                x="Particles_Str",
                y="PeakRSS_MB",
                color="Configuration",
                pattern_shape=pattern_arg,
                pattern_shape_map=pattern_map,
                barmode="group",
                facet_col="Test" if len(selected_tests) > 1 else None,
                log_y=True,
                title=f"Peak Memory Scaling Across Datasets ({device_view})",
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
                if c in plot_df.columns
            ]
            sorted_df = plot_df.sort_values(sort_cols) if sort_cols else plot_df
            raw_cols = [
                c
                for c in [
                    "Device",
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

            cpu_raw = sorted_df[sorted_df["Device"] == "CPU"][raw_cols]
            gpu_raw = sorted_df[sorted_df["Device"] == "GPU"][raw_cols]

            tab_cpu, tab_gpu = st.tabs([
                "CPU Benchmark Results",
                "GPU Benchmark Results",
            ])
            with tab_cpu:
                if not cpu_raw.empty:
                    st.dataframe(cpu_raw, width="stretch")
                else:
                    st.info("No CPU raw benchmark data available for this selection.")
            with tab_gpu:
                if not gpu_raw.empty:
                    st.dataframe(gpu_raw, width="stretch")
                else:
                    st.info("No GPU raw benchmark data available for this selection.")
        else:
            st.warning("No data matches the selected hardware mode.")
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

    col1, col2, col3 = st.columns([2, 1, 2])
    with col1:
        sel_test = st.selectbox(
            "Select Test to Compare", sorted(df_all["Test"].unique())
        )
    with col2:
        sel_periodic = st.radio("Periodic BC Mode", ["OFF", "ON"], horizontal=True)

    comp_df = df_all[
        (df_all["Test"] == sel_test) & (df_all["Periodic_BC"] == sel_periodic)
    ].copy()

    # Explicitly enforce category ordering: double -> int32 -> int64
    comp_df["Precision"] = pd.Categorical(
        comp_df["Precision"], categories=["double", "int32", "int64"], ordered=True
    )
    comp_df = comp_df.sort_values(["Precision", "Particles_Num"])

    # Parameter filter for RangeQuery / KNNQuery / SelfKNNQuery on Page 2
    if sel_test in ["RangeQuery", "KNNQuery", "SelfKNNQuery"]:
        valid_params_p2 = sorted(
            comp_df[comp_df["Params"] != "None"]["Params"].unique()
        )
        if valid_params_p2:
            default_p2_idx = 0
            for idx, p in enumerate(valid_params_p2):
                if "k=16" in p or "1.000000" in p:
                    default_p2_idx = idx
                    break
            with col3:
                sel_param_p2 = st.selectbox(
                    "Select Parameter (R / k)", valid_params_p2, index=default_p2_idx
                )
            comp_df = comp_df[
                (comp_df["Params"] == sel_param_p2) | (comp_df["Params"] == "None")
            ].copy()

    if not comp_df.empty:
        fig_comp = px.bar(
            comp_df,
            x="Particles_Str",
            y="Time_ms",
            color="Precision",
            barmode="group",
            log_y=True,
            category_orders={"Precision": ["double", "int32", "int64"]},
            facet_col="Device" if comp_df["Device"].nunique() > 1 else None,
            title=f"Execution Time Comparison for {sel_test} (Periodic BC: {sel_periodic}) [Log Scale]",
            labels=labels_dict,
            color_discrete_map={
                "double": "#1f77b4",
                "int32": "#2ca02c",
                "int64": "#ff7f0e",
            },
        )
        fig_comp.update_layout(height=500, legend=legend_dict)
        st.plotly_chart(fig_comp, width="stretch")

        # Compute speedup table relative to double
        st.subheader(
            "Speedup Relative to Double Precision (`double` time / `int` time)",
            divider="gray",
        )
        pivot_df = comp_df.pivot_table(
            index=["Device", "Particles_Str", "Params"],
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
