# Development extension notes

This page is for contributors who already understand the basic runtime and benchmark workflows from the root [README.md](../../README.md).

## 2D code entry points

Main behavior files:

- `environment/2d-environment/starter-stack/Agent/src/sample_player.cpp`
- `environment/2d-environment/starter-stack/Agent/src/bhv_basic_move.cpp`
- `environment/2d-environment/starter-stack/Agent/src/bhv_basic_offensive_kick.cpp`
- `environment/2d-environment/starter-stack/Agent/src/start-4players.sh`

Benchmark entrypoints:

- `environment/2d-environment/starter-stack/run_strategy_benchmark_2d.sh`
- `environment/2d-environment/starter-stack/run_parametric_benchmark_2d.sh`

Compatibility bridge:

- `environment/2d-environment/starter-stack/link_starteragent2d_v2_compat_2d.sh`

Strategy and parameter mapping currently rely on team-name driven aliases in the 2D runtime. Keep that in mind before changing team-name conventions.

## 3D code entry points

Main runtime files:

- `environment/3d-environment/FCPCodebase/Run_Player.py`
- `environment/3d-environment/FCPCodebase/agent/Agent.py`
- `environment/3d-environment/FCPCodebase/strategy/strategy_registry.py`
- `environment/3d-environment/FCPCodebase/strategy/press_mapping.py`
- `environment/3d-environment/FCPCodebase/strategy/shoot_mapping.py`

Benchmark entrypoints:

- `environment/3d-environment/scripts/run_strategy_benchmark_3d.sh`
- `environment/3d-environment/scripts/run_parametric_benchmark_3d.sh`

Parsing utility:

- `scripts/utils/parse_roboviz_log.py`

## Extending benchmarks

When you add a new controller preset or parameter level:

1. update the runtime mapping first,
2. then update the relevant benchmark script,
3. then update [../benchmarks/workflows.md](../benchmarks/workflows.md) and the root README if the user-facing workflow changed.

Keep the root README as the canonical user entrypoint. Deep implementation notes belong here or in the more focused docs pages.

## Offline pipeline

The imitation-learning project remains separate:

- `behaviour_algorithm/rcss2d-opp-imitation-main/`

If you extend it, keep its subproject README focused on local component details rather than turning it into another top-level project guide.
