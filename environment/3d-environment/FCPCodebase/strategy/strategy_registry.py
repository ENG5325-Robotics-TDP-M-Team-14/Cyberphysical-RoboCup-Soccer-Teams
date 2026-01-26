from strategy.strategy_config import StrategyConfig


_BASIC = StrategyConfig(
    name="BASIC",
    formation_id="BASELINE",
    press_threshold=3,
    shoot_range=25.0,
)
_DEFLOCK = StrategyConfig(
    name="DEFLOCK",
    formation_id="DEF_121",
    press_threshold=2,
    shoot_range=20.0,
)
_HIPRESS = StrategyConfig(
    name="HIPRESS",
    formation_id="DEF_121",
    press_threshold=4,
    shoot_range=20.0,
)
_DIRECT = StrategyConfig(
    name="DIRECT",
    formation_id="OFF_112",
    press_threshold=2,
    shoot_range=30.0,
)
_AGGRO = StrategyConfig(
    name="AGGRO",
    formation_id="OFF_112",
    press_threshold=4,
    shoot_range=30.0,
)

_ORDERED_NAMES = ("BASIC", "NOISE", "DEFLOCK", "HIPRESS", "DIRECT", "AGGRO")
_STRATEGIES = {
    "BASIC": _BASIC,
    "NOISE": _BASIC,  # Phase 1 alias of BASIC.
    "DEFLOCK": _DEFLOCK,
    "HIPRESS": _HIPRESS,
    "DIRECT": _DIRECT,
    "AGGRO": _AGGRO,
}


def list_strategies() -> list[str]:
    return list(_ORDERED_NAMES)


def get_strategy(name: str) -> StrategyConfig:
    if not isinstance(name, str):
        valid = ", ".join(list_strategies())
        raise ValueError(f"Unknown strategy name '{name}'. Valid names: {valid}")

    key = name.strip().upper()
    if key in _STRATEGIES:
        return _STRATEGIES[key]

    valid = ", ".join(list_strategies())
    raise ValueError(f"Unknown strategy name '{name}'. Valid names: {valid}")
