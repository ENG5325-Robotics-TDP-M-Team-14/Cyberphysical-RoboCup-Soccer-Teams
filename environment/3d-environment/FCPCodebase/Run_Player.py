import argparse
import sys

from scripts.commons.Script import Script
from strategy.strategy_registry import get_strategy
from strategy.press_mapping import press_threshold_to_margin
from strategy.shoot_mapping import shoot_range_to_goal_shot_dist

_strategy_parser = argparse.ArgumentParser(add_help=False)
_strategy_parser.add_argument("--strategy", default="BASIC")
_strategy_args, _remaining_args = _strategy_parser.parse_known_args()
sys.argv = [sys.argv[0]] + _remaining_args
script = Script(cpp_builder_unum=1) # Initialize: load config file, parse arguments, build cpp modules
a = script.args

if a.P: # penalty shootout
    from agent.Agent_Penalty import Agent
else: # normal agent
    from agent.Agent import Agent

strategy_config = get_strategy(_strategy_args.strategy)
press_margin = press_threshold_to_margin(strategy_config.press_threshold)
goal_shot_dist_thresh_m = shoot_range_to_goal_shot_dist(strategy_config.shoot_range)
print(
    f"[STRATEGY] Team={a.t} Uniform={a.u} Strategy={strategy_config.name} "
    f"F={strategy_config.formation_id} P={strategy_config.press_threshold} "
    f"S={strategy_config.shoot_range}"
)
print(
    f"[PRESS] Team={a.t} Uniform={a.u} threshold={strategy_config.press_threshold} "
    f"margin_m={press_margin}"
)
print(
    f"[SHOOT] Team={a.t} Uniform={a.u} shoot_range={strategy_config.shoot_range} "
    f"goal_shot_dist_thresh_m={goal_shot_dist_thresh_m}"
)

# Args: Server IP, Agent Port, Monitor Port, Uniform No., Team name, Enable Log, Enable Draw, Wait for Server, is magmaFatProxy
if a.D: # debug mode
    if a.P:
        player = Agent(a.i, a.p, a.m, a.u, a.t, True, True, False, a.F)
    else:
        player = Agent(
            a.i,
            a.p,
            a.m,
            a.u,
            a.t,
            True,
            True,
            False,
            a.F,
            strategy_config.formation_id,
            press_margin,
            goal_shot_dist_thresh_m,
        )
else:
    if a.P:
        player = Agent(a.i, a.p, None, a.u, a.t, False, False, False, a.F)
    else:
        player = Agent(
            a.i,
            a.p,
            None,
            a.u,
            a.t,
            False,
            False,
            False,
            a.F,
            strategy_config.formation_id,
            press_margin,
            goal_shot_dist_thresh_m,
        )

player.strategy_config = strategy_config

while True:
    player.think_and_send()
    player.scom.receive()
