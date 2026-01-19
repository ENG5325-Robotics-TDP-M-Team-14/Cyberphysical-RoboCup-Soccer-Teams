#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
ENV2D_DIR=$(cd "${SCRIPT_DIR}/.." && pwd)
AGENT_DIR="${SCRIPT_DIR}/Agent/src"
RCSSSERVER_BIN="${ENV2D_DIR}/rcssserver-19.0.0/build/rcssserver"
RESULTS_CSV="${SCRIPT_DIR}/strategy_benchmark_results.csv"

MATCH_SECONDS="${MATCH_SECONDS:-420}"
START_DELAY="${START_DELAY:-2}"
SIDE_DELAY="${SIDE_DELAY:-2}"

SERVER_PID=""

cleanup_match() {
  if [[ -n "${SERVER_PID}" ]]; then
    kill -INT "${SERVER_PID}" >/dev/null 2>&1 || true
    wait "${SERVER_PID}" >/dev/null 2>&1 || true
    SERVER_PID=""
  fi
  killall sample_player sample_coach >/dev/null 2>&1 || true
}

cleanup() {
  cleanup_match
}

trap cleanup EXIT INT TERM

if [[ ! -x "${RCSSSERVER_BIN}" ]]; then
  echo "rcssserver not found/executable at ${RCSSSERVER_BIN}" >&2
  exit 1
fi

if [[ ! -x "${AGENT_DIR}/start-4players.sh" ]]; then
  echo "start-4players.sh not found at ${AGENT_DIR}/start-4players.sh" >&2
  exit 1
fi

if [[ ! -f "${RESULTS_CSV}" ]]; then
  echo "pair_id,left_team,right_team,left_goals,right_goals,timestamp" > "${RESULTS_CSV}"
fi

run_match() {
  local pair_id="$1"
  local left_team="$2"
  local right_team="$3"
  local log_file
  log_file="$(mktemp)"

  cleanup_match

  "${RCSSSERVER_BIN}" > "${log_file}" 2>&1 &
  SERVER_PID=$!

  sleep "${START_DELAY}"
  "${AGENT_DIR}/start-4players.sh" -t "${left_team}" >/dev/null 2>&1
  sleep "${SIDE_DELAY}"
  "${AGENT_DIR}/start-4players.sh" -t "${right_team}" >/dev/null 2>&1

  sleep "${MATCH_SECONDS}"
  kill -INT "${SERVER_PID}" >/dev/null 2>&1 || true
  wait "${SERVER_PID}" >/dev/null 2>&1 || true
  SERVER_PID=""

  local score_line
  score_line="$(awk '/Score:/{print $2","$4}' "${log_file}" | tail -n 1)"
  if [[ -z "${score_line}" ]]; then
    echo "Failed to parse score from ${log_file}" >&2
    exit 1
  fi

  local left_goals
  local right_goals
  left_goals="${score_line%,*}"
  right_goals="${score_line#*,}"

  local timestamp
  timestamp="$(date -u +"%Y-%m-%dT%H:%M:%SZ")"
  echo "${pair_id},${left_team},${right_team},${left_goals},${right_goals},${timestamp}" >> "${RESULTS_CSV}"

  rm -f "${log_file}"
}

pairs=(
  "BASIC_NOISE:BASIC:NOISE"
  "BASIC_DEFLOCK:BASIC:DEFLOCK"
  "BASIC_HIPRESS:BASIC:HIPRESS"
  "BASIC_DIRECT:BASIC:DIRECT"
  "BASIC_AGGRO:BASIC:AGGRO"
)

for pair in "${pairs[@]}"; do
  pair_id="${pair%%:*}"
  rest="${pair#*:}"
  left="${rest%%:*}"
  right="${rest#*:}"

  for side in 0 1; do
    for rep in 1 2 3 4 5; do
      if [[ "${side}" -eq 0 ]]; then
        run_match "${pair_id}" "${left}" "${right}"
      else
        run_match "${pair_id}" "${right}" "${left}"
      fi
    done
  done
done
