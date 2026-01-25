#!/usr/bin/env python3
import argparse
import re
import sys


RE_TIME = re.compile(r"\(time\s+([0-9.+-eE]+)\)")
RE_HALF = re.compile(r"\(half\s+([0-9]+)\)")
RE_RDS = re.compile(r"\(RDS\s+([0-9]+)\s+([0-9]+)\)")

STOP_HALF = 2


def parse_log(path):
    start_time = None
    half_time_time = None
    score_left = None
    score_right = None

    with open(path, "r", errors="ignore") as f:
        for line in f:
            stripped = line.lstrip()
            if not stripped.startswith("((time "):
                continue

            time_match = RE_TIME.search(line)
            half_match = RE_HALF.search(line)
            rds_match = RE_RDS.search(line)
            if not (time_match and half_match and rds_match):
                continue

            try:
                time_val = float(time_match.group(1))
                half_val = int(half_match.group(1))
                new_left = int(rds_match.group(1))
                new_right = int(rds_match.group(2))
            except ValueError:
                continue

            if start_time is None:
                start_time = time_val

            if half_time_time is None and half_val == STOP_HALF:
                half_time_time = time_val

            score_left = new_left
            score_right = new_right

    return {
        "start_time": start_time,
        "half_time_time": half_time_time,
        "score_left": score_left,
        "score_right": score_right,
    }


def main():
    parser = argparse.ArgumentParser(description="Parse a RoboViz logfile for match events.")
    parser.add_argument("logfile", help="Path to a RoboViz log file")
    args = parser.parse_args()

    data = parse_log(args.logfile)

    start_time = data["start_time"]
    half_time_time = data["half_time_time"]
    score_left = data["score_left"]
    score_right = data["score_right"]

    start_str = start_time if start_time is not None else "NOT_FOUND"
    half_str = half_time_time if half_time_time is not None else "NOT_FOUND"
    if score_left is not None and score_right is not None:
        score_str = f"{score_left},{score_right}"
    else:
        score_str = "NOT_FOUND"

    print(f"start_time={start_str}")
    print(f"half_time_time={half_str}")
    print(f"score_latest={score_str}")
    print("goal_event_count=NA")
    print(
        f"summary: start_time={start_str} half_time={half_str} goals=NA score={score_str}"
    )

    if half_time_time is not None and score_left is not None and score_right is not None:
        return 0
    return 2


if __name__ == "__main__":
    sys.exit(main())
