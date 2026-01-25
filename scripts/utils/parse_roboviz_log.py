#!/usr/bin/env python3
import argparse
import re
import sys


RE_TIME = re.compile(r"\(time\s+([0-9.+-eE]+)\)")
RE_HALF = re.compile(r"\(half\s+([0-9]+)\)")
RE_PLAY_MODE = re.compile(r"\(play_mode\s+([0-9]+)\)")
RE_RDS = re.compile(r"\(RDS\s+([0-9]+)\s+([0-9]+)\)")

STOP_HALF = 2


def parse_log(path):
    start_time = None
    half_time_time = None

    latest_time = None
    latest_half = None
    latest_play_mode = None
    latest_score_left = None
    latest_score_right = None

    with open(path, "r", errors="ignore") as f:
        for line in f:
            time_match = RE_TIME.search(line)
            half_match = RE_HALF.search(line)
            play_mode_match = RE_PLAY_MODE.search(line)
            rds_match = RE_RDS.search(line)

            if time_match and start_time is None:
                try:
                    start_time = float(time_match.group(1))
                except ValueError:
                    pass

            if rds_match:
                try:
                    latest_score_left = int(rds_match.group(1))
                    latest_score_right = int(rds_match.group(2))
                except ValueError:
                    pass

            if not (time_match and half_match and play_mode_match):
                continue

            try:
                time_val = float(time_match.group(1))
                half_val = int(half_match.group(1))
                play_mode_val = int(play_mode_match.group(1))
            except ValueError:
                continue

            latest_time = time_val
            latest_half = half_val
            latest_play_mode = play_mode_val

            if half_time_time is None and half_val == STOP_HALF:
                half_time_time = time_val

    return {
        "start_time": start_time,
        "half_time_time": half_time_time,
        "latest_time": latest_time,
        "latest_half": latest_half,
        "latest_play_mode": latest_play_mode,
        "latest_score_left": latest_score_left,
        "latest_score_right": latest_score_right,
    }


def main():
    parser = argparse.ArgumentParser(description="Parse a RoboViz logfile for match events.")
    parser.add_argument("logfile", help="Path to a RoboViz log file")
    args = parser.parse_args()

    data = parse_log(args.logfile)

    start_time = data["start_time"]
    half_time_time = data["half_time_time"]
    latest_time = data["latest_time"]
    latest_half = data["latest_half"]
    latest_play_mode = data["latest_play_mode"]
    latest_score_left = data["latest_score_left"]
    latest_score_right = data["latest_score_right"]

    start_str = start_time if start_time is not None else "NOT_FOUND"
    half_str = half_time_time if half_time_time is not None else "NOT_FOUND"
    latest_time_str = latest_time if latest_time is not None else "NOT_FOUND"
    latest_half_str = latest_half if latest_half is not None else "NOT_FOUND"
    latest_play_mode_str = (
        latest_play_mode if latest_play_mode is not None else "NOT_FOUND"
    )
    latest_score_left_str = (
        latest_score_left if latest_score_left is not None else "NOT_FOUND"
    )
    latest_score_right_str = (
        latest_score_right if latest_score_right is not None else "NOT_FOUND"
    )

    if latest_score_left is not None and latest_score_right is not None:
        score_latest = f"{latest_score_left},{latest_score_right}"
    else:
        score_latest = "NOT_FOUND"

    print(f"start_time={start_str}")
    print(f"half_time_time={half_str}")
    print(f"score_latest={score_latest}")
    print("goal_event_count=NOT_FOUND")
    print(f"latest_time={latest_time_str}")
    print(f"latest_half={latest_half_str}")
    print(f"latest_play_mode={latest_play_mode_str}")
    print(f"latest_score_left={latest_score_left_str}")
    print(f"latest_score_right={latest_score_right_str}")
    print(
        f"summary: start_time={start_str} half_time={half_str} goals=NOT_FOUND score={score_latest}"
    )

    if half_time_time is not None and latest_score_left is not None and latest_score_right is not None:
        return 0
    return 2


if __name__ == "__main__":
    sys.exit(main())
