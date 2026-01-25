#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
env_root="$(cd "${script_dir}/.." && pwd)"

fcp_dir="${env_root}/FCPCodebase"
venv_activate="${fcp_dir}/.venv/bin/activate"

use_setsid=0
if command -v setsid >/dev/null 2>&1; then
  use_setsid=1
fi

last_pid=""
start_cmd() {
  if [ "$use_setsid" -eq 1 ]; then
    setsid "$@" &
  else
    "$@" &
  fi
  last_pid=$!
}

terminate_pid() {
  local pid="$1"
  if [ -z "$pid" ]; then
    return
  fi
  if [ "$use_setsid" -eq 1 ]; then
    kill -TERM -- "-${pid}" 2>/dev/null || true
  else
    kill -TERM "$pid" 2>/dev/null || true
    if command -v pkill >/dev/null 2>&1; then
      pkill -TERM -P "$pid" 2>/dev/null || true
    fi
  fi
}

kill_pid() {
  local pid="$1"
  if [ -z "$pid" ]; then
    return
  fi
  if [ "$use_setsid" -eq 1 ]; then
    kill -KILL -- "-${pid}" 2>/dev/null || true
  else
    kill -KILL "$pid" 2>/dev/null || true
    if command -v pkill >/dev/null 2>&1; then
      pkill -KILL -P "$pid" 2>/dev/null || true
    fi
  fi
}

agent_pids=()
launcher_pid=""

cleanup() {
  set +e
  for pid in "${agent_pids[@]}"; do
    terminate_pid "$pid"
  done
  terminate_pid "$launcher_pid"

  sleep 1

  for pid in "${agent_pids[@]}"; do
    kill_pid "$pid"
  done
  kill_pid "$launcher_pid"
}

trap cleanup EXIT INT TERM

start_cmd "${script_dir}/run_rcssserver3d_and_RoboViz.sh"
launcher_pid=$last_pid

wait_for_port() {
  local port="$1"
  local tries=40
  while [ "$tries" -gt 0 ]; do
    if ss -lnt 2>/dev/null | grep -q ":${port}"; then
      return 0
    fi
    sleep 0.25
    tries=$((tries - 1))
  done
  return 1
}

if ! wait_for_port 3200; then
  echo "[SMOKE] warning: monitor port 3200 not detected; starting agents anyway"
else
  echo "[SMOKE] monitor port 3200 open"
fi

send_trainer_cmd() {
  local name="$1"
  local payload="$2"
  python3 - "$payload" <<'PY'
import socket, struct, sys
host = "127.0.0.1"
port = 3200
msg = sys.argv[1]
data = msg.encode("ascii")
hdr = struct.pack("!I", len(data))
with socket.create_connection((host, port), timeout=2) as s:
    s.sendall(hdr + data)
PY
  echo "[SMOKE] trainer_sent=${name} raw=${payload}"
}

for u in 1 2 3 4; do
  start_cmd bash -c "cd \"${fcp_dir}\" && source \"${venv_activate}\" && python Run_Player.py -t Home -u ${u} --strategy BASIC"
  agent_pids+=("$last_pid")
  sleep 0.3
done
sleep 0.5
for u in 1 2 3 4; do
  start_cmd bash -c "cd \"${fcp_dir}\" && source \"${venv_activate}\" && python Run_Player.py -t Away -u ${u} --strategy AGGRO"
  agent_pids+=("$last_pid")
  sleep 0.3
done

echo "[SMOKE] launcher_pid=${launcher_pid}"
echo "[SMOKE] agent_pids=${agent_pids[*]}"

sleep 1
send_trainer_cmd "drop_ball" "(dropBall)"

sleep 10
