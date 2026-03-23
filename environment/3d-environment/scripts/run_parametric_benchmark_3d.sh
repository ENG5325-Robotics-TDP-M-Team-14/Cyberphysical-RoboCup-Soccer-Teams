#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: run_parametric_benchmark_3d.sh [options]

Options:
  --parameter PARAM             One of: press_threshold, shoot_range, formation
  --mode MODE                   One of: 4v4, 2v2, 1v1 (default: 4v4)
  --repeats N                   Replicates per side per level (default: 5)
  --levels CSV                  Comma-separated levels for selected parameter
                                press_threshold/shoot_range: low,baseline,high
                                formation: baseline,def,off
  --half-time-timeout-sec N     Timeout forwarded to run_strategy_benchmark_3d.sh
  --out-csv PATH                Optional mirror path for final aggregated results CSV
  -h, --help                    Show this help
EOF
}

parameter="press_threshold"
mode="4v4"
repeats=5
levels_override=""
results_csv_mirror=""
half_time_timeout_sec=420

while [[ $# -gt 0 ]]; do
  case "$1" in
    --parameter)
      parameter="$2"
      shift 2
      ;;
    --mode)
      mode="$2"
      shift 2
      ;;
    --repeats)
      repeats="$2"
      shift 2
      ;;
    --levels)
      levels_override="$2"
      shift 2
      ;;
    --half-time-timeout-sec)
      half_time_timeout_sec="$2"
      shift 2
      ;;
    --out-csv)
      results_csv_mirror="$2"
      shift 2
      ;;
    -h|--help)
      usage
      exit 0
      ;;
    *)
      echo "Unknown argument: $1" >&2
      usage
      exit 1
      ;;
  esac
done

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ENV_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
STRATEGY_BENCH="${SCRIPT_DIR}/run_strategy_benchmark_3d.sh"
SIMULATOR_ID="3d"
BENCHMARK_KIND="parametric"
BASELINE_CONTROLLER="BASIC"

if [[ ! -x "${STRATEGY_BENCH}" ]]; then
  echo "Strategy benchmark script not found at ${STRATEGY_BENCH}" >&2
  exit 1
fi

normalize_level() {
  printf '%s' "$1" | tr '[:upper:]' '[:lower:]'
}

level_to_strategy() {
  local level
  level="$(normalize_level "$1")"

  case "${parameter}" in
    press_threshold)
      case "${level}" in
        low) echo "PRESLOW" ;;
        baseline) echo "PRESBASE" ;;
        high) echo "PRESHIGH" ;;
        *) return 1 ;;
      esac
      ;;
    shoot_range)
      case "${level}" in
        low) echo "SHOTLOW" ;;
        baseline) echo "SHOTBASE" ;;
        high) echo "SHOTHIGH" ;;
        *) return 1 ;;
      esac
      ;;
    formation)
      case "${level}" in
        baseline) echo "FORMBASE" ;;
        def|def_121) echo "FORMDEF" ;;
        off|off_112) echo "FORMOFF" ;;
        *) return 1 ;;
      esac
      ;;
    *)
      return 1
      ;;
  esac
}

level_to_value() {
  local level
  level="$(normalize_level "$1")"

  case "${parameter}" in
    press_threshold)
      case "${level}" in
        low) echo "2" ;;
        baseline) echo "3" ;;
        high) echo "4" ;;
        *) return 1 ;;
      esac
      ;;
    shoot_range)
      case "${level}" in
        low) echo "20.0" ;;
        baseline) echo "25.0" ;;
        high) echo "30.0" ;;
        *) return 1 ;;
      esac
      ;;
    formation)
      case "${level}" in
        baseline) echo "BASELINE" ;;
        def|def_121) echo "DEF_121" ;;
        off|off_112) echo "OFF_112" ;;
        *) return 1 ;;
      esac
      ;;
    *)
      return 1
      ;;
  esac
}

next_run_index() {
  local root="$1"
  local max_idx=0
  local d
  mkdir -p "${root}"
  for d in "${root}"/run_*; do
    [[ -d "${d}" ]] || continue
    local bn idx
    bn="$(basename "${d}")"
    idx="${bn#run_}"
    if [[ "${idx}" =~ ^[0-9]{4}$ ]]; then
      if (( 10#${idx} > max_idx )); then
        max_idx=$((10#${idx}))
      fi
    fi
  done
  printf "%04d" $((max_idx + 1))
}

case "${parameter}" in
  press_threshold|shoot_range|formation)
    ;;
  *)
    echo "Invalid --parameter '${parameter}' (expected: press_threshold, shoot_range, formation)" >&2
    exit 1
    ;;
esac

declare -a levels=()
if [[ -n "${levels_override}" ]]; then
  IFS=',' read -r -a levels <<< "${levels_override}"
else
  case "${parameter}" in
    press_threshold|shoot_range)
      levels=(low baseline high)
      ;;
    formation)
      levels=(baseline def off)
      ;;
  esac
fi

for lvl in "${levels[@]}"; do
  if ! level_to_strategy "${lvl}" >/dev/null; then
    echo "Invalid level '${lvl}' for parameter '${parameter}'" >&2
    exit 1
  fi
  if ! level_to_value "${lvl}" >/dev/null; then
    echo "Could not map level '${lvl}' to concrete value for parameter '${parameter}'" >&2
    exit 1
  fi
done

declare -a mode_args=()
case "${mode}" in
  4v4)
    mode_args=()
    ;;
  2v2)
    # 2v2 uses goalkeeper (unum 1) + one field robot (unum 2) per side.
    mode_args=(--unums 1,2)
    ;;
  1v1)
    # 1v1 is a no-goalie duel using only field robot 2 per side.
    mode_args=(--unums 2)
    ;;
  *)
    echo "Invalid --mode '${mode}' (expected: 4v4, 2v2 or 1v1)" >&2
    exit 1
    ;;
esac

OUTPUT_ROOT="${ENV_ROOT}/benchmark_outputs/${SIMULATOR_ID}/${BENCHMARK_KIND}/mode_${mode}/baseline_${BASELINE_CONTROLLER}/parameter_${parameter}"
RUN_INDEX="$(next_run_index "${OUTPUT_ROOT}")"
RUN_ID="run_${RUN_INDEX}"
RUN_DIR="${OUTPUT_ROOT}/${RUN_ID}"
LEVEL_ROOT="${RUN_DIR}/levels"
RESULTS_CSV="${RUN_DIR}/results.csv"
LEVELS_CSV="${RUN_DIR}/levels.csv"
RUN_METADATA_JSON="${RUN_DIR}/run_metadata.json"
METRICS_CATALOG_CSV="${RUN_DIR}/metrics_catalog.csv"
BEHAVIOURAL_SCAFFOLD_CSV="${RUN_DIR}/behavioural_metrics_scaffold.csv"

mkdir -p "${RUN_DIR}" "${LEVEL_ROOT}"

echo "parameter_level,parameter_value,variant_controller" > "${LEVELS_CSV}"
for lvl in "${levels[@]}"; do
  normalized_level="$(normalize_level "${lvl}")"
  echo "${normalized_level},$(level_to_value "${normalized_level}"),$(level_to_strategy "${normalized_level}")" >> "${LEVELS_CSV}"
done

cat > "${METRICS_CATALOG_CSV}" <<'EOF'
metric_name,status,scope,source,notes
left_goals,available,match,run_strategy_benchmark_3d aggregated CSV,Final left score from strategy benchmark row
right_goals,available,match,run_strategy_benchmark_3d aggregated CSV,Final right score from strategy benchmark row
match_status,available,match,run_strategy_benchmark_3d aggregated CSV,ok/timeout/error
error_reason,available,match,run_strategy_benchmark_3d aggregated CSV,timeout/server_dead/agent_dead/parser_fail/unknown
timestamp_utc,available,match,run_strategy_benchmark_3d aggregated CSV,Row timestamp in UTC
shot_distance_m,scaffolded,match,placeholder,Requires event-level kick telemetry
shot_location_x,scaffolded,match,placeholder,Requires event-level kick telemetry
shot_location_y,scaffolded,match,placeholder,Requires event-level kick telemetry
press_initiation_distance_m,scaffolded,match,placeholder,Requires press trigger instrumentation
time_to_shot_s,scaffolded,match,placeholder,Requires shot event timing instrumentation
time_to_engagement_s,scaffolded,match,placeholder,Requires engagement event timing instrumentation
spatial_occupancy_json,scaffolded,match,placeholder,Requires positional time-series logs
trajectory_path_length_m,scaffolded,match,placeholder,Requires positional time-series logs
trajectory_curvature_mean,scaffolded,match,placeholder,Requires positional time-series logs
EOF

echo "run_id,benchmark_type,simulator,mode,baseline_controller,parameter_name,parameter_level,parameter_value,match_id,pair_id,left_team,right_team,left_goals,right_goals,timestamp,status,error_reason" > "${RESULTS_CSV}"
echo "run_id,match_id,simulator,mode,baseline_controller,parameter_name,parameter_level,parameter_value,shot_distance_m,shot_location_x,shot_location_y,press_initiation_distance_m,time_to_shot_s,time_to_engagement_s,spatial_occupancy_json,trajectory_path_length_m,trajectory_curvature_mean,availability_shot_distance,availability_press_distance,availability_time_to_shot,availability_time_to_engagement,availability_spatial_occupancy,availability_trajectory,notes" > "${BEHAVIOURAL_SCAFFOLD_CSV}"

export RUN_METADATA_JSON LEVELS_CSV RUN_ID BENCHMARK_KIND SIMULATOR_ID mode BASELINE_CONTROLLER parameter OUTPUT_ROOT RUN_DIR
python3 - <<'PY'
import csv
import json
import os
from datetime import datetime, timezone

levels = []
with open(os.environ["LEVELS_CSV"], newline="", encoding="utf-8") as f:
    for row in csv.DictReader(f):
        levels.append(row)

metadata = {
    "run_id": os.environ["RUN_ID"],
    "benchmark_type": os.environ["BENCHMARK_KIND"],
    "simulator": os.environ["SIMULATOR_ID"],
    "mode": os.environ["mode"],
    "baseline_controller": os.environ["BASELINE_CONTROLLER"],
    "parameter_name": os.environ["parameter"],
    "levels": levels,
    "output_root": os.environ["OUTPUT_ROOT"],
    "run_dir": os.environ["RUN_DIR"],
    "created_at_utc": datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
    "notes": {
        "behavioural_metrics": "Scaffolded placeholders are emitted; requested behavioural metrics are not computed from current logs yet."
    },
}

with open(os.environ["RUN_METADATA_JSON"], "w", encoding="utf-8") as f:
    json.dump(metadata, f, indent=2, sort_keys=True)
PY

append_level_results() {
  local level_csv="$1"
  local normalized_level="$2"
  local parameter_value="$3"

  if [[ ! -f "${level_csv}" ]]; then
    echo "[PARAM] warning: missing strategy results CSV: ${level_csv}" >&2
    return 1
  fi

  while IFS=, read -r match_id pair_id left_team right_team left_goals right_goals timestamp status error_reason; do
    [[ -z "${match_id}" || "${match_id}" == "match_id" ]] && continue
    echo "${RUN_ID},${BENCHMARK_KIND},${SIMULATOR_ID},${mode},${BASELINE_CONTROLLER},${parameter},${normalized_level},${parameter_value},${match_id},${pair_id},${left_team},${right_team},${left_goals},${right_goals},${timestamp},${status},${error_reason}" >> "${RESULTS_CSV}"

    echo "${RUN_ID},${match_id},${SIMULATOR_ID},${mode},${BASELINE_CONTROLLER},${parameter},${normalized_level},${parameter_value},,,,,,,,,,not_available_from_current_logs,not_available_from_current_logs,not_available_from_current_logs,not_available_from_current_logs,not_available_from_current_logs,not_available_from_current_logs,requires_event_level_telemetry_for_shots_press_and_trajectories" >> "${BEHAVIOURAL_SCAFFOLD_CSV}"
  done < "${level_csv}"
}

overall_status=0
for lvl in "${levels[@]}"; do
  normalized_level="$(normalize_level "${lvl}")"
  parameter_value="$(level_to_value "${normalized_level}")"
  strategy_name="$(level_to_strategy "${normalized_level}")"
  level_dir="${LEVEL_ROOT}/level_${normalized_level}"
  level_csv="${level_dir}/strategy_results.csv"
  level_log_dir="${level_dir}/strategy_logs"
  level_stdout="${level_dir}/strategy_stdout.log"

  rm -rf "${level_dir}"
  mkdir -p "${level_log_dir}"

  cmd=(
    "${STRATEGY_BENCH}"
    --repeats "${repeats}"
    --pairs "${BASELINE_CONTROLLER},${strategy_name}"
    --out-csv "${level_csv}"
    --half-time-timeout-sec "${half_time_timeout_sec}"
    --log-dir "${level_log_dir}"
  )
  if [[ "${#mode_args[@]}" -gt 0 ]]; then
    cmd+=("${mode_args[@]}")
  fi

  echo "[PARAM] Running ${parameter}=${normalized_level} (value=${parameter_value}) with strategy ${strategy_name} (mode=${mode})"
  set +e
  "${cmd[@]}" | tee "${level_stdout}"
  level_status=$?
  set -e

  if [[ "${level_status}" -ne 0 ]]; then
    echo "[PARAM] warning: strategy benchmark exited with status ${level_status} for level ${normalized_level}" >&2
  fi

  if ! append_level_results "${level_csv}" "${normalized_level}" "${parameter_value}"; then
    overall_status=1
    continue
  fi

  if [[ "${level_status}" -ne 0 ]]; then
    overall_status="${level_status}"
  fi
done

if [[ -n "${results_csv_mirror}" ]]; then
  mkdir -p "$(dirname "${results_csv_mirror}")"
  cp "${RESULTS_CSV}" "${results_csv_mirror}"
fi

echo "Parametric 3D benchmark finished."
echo "Run directory: ${RUN_DIR}"
echo "Results CSV: ${RESULTS_CSV}"
echo "Run metadata: ${RUN_METADATA_JSON}"
if [[ -n "${results_csv_mirror}" ]]; then
  echo "Mirrored results CSV: ${results_csv_mirror}"
fi

exit "${overall_status}"
