from dataclasses import dataclass


@dataclass(frozen=True)
class StrategyConfig:
    name: str
    formation_id: str
    press_threshold: int
    shoot_range: float
