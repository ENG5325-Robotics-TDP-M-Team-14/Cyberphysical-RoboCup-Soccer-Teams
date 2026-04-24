# API Documentation

This repository includes a root `Doxyfile` for source navigation across the
project-specific 2D agent code, 3D FC Portugal adaptations, benchmark scripts,
and architecture notes.

## Generate Docs

Install Doxygen on Linux, then run:

```bash
doxygen Doxyfile
```

The generated HTML is written to:

```text
docs/api/html/
```

That directory is ignored by Git. Regenerate it locally whenever the source
changes.

## GitHub Pages

The repository also includes a GitHub Actions workflow at
`.github/workflows/doxygen-pages.yml`. On pushes to `develop` or `main`, and on
manual dispatch, the workflow installs Doxygen, builds `docs/api/html/`, and
publishes the generated HTML through GitHub Pages when the ref is `main`.

Published documentation:

<https://eng5325-robotics-tdp-m-team-14.github.io/Cyberphysical-RoboCup-Soccer-Teams/>

The generated HTML should stay out of Git. Source files, Markdown
documentation, `Doxyfile`, and the workflow are the maintained documentation
inputs.

## Coverage

The Doxygen configuration indexes:

- `README.md`
- `docs/architecture/`
- `docs/benchmarks/`
- `docs/development/`
- `environment/2d-environment/starter-stack/Agent/src/`
- `environment/3d-environment/FCPCodebase/agent/`
- `environment/3d-environment/FCPCodebase/strategy/`
- `environment/3d-environment/FCPCodebase/world/commons/`
- `environment/3d-environment/scripts/`
- `scripts/`

It intentionally excludes generated outputs, simulator build folders, virtual
environments, caches, and benchmark run directories.

## Notes

The inherited simulator source trees are large and include upstream code. The
default Doxygen scope therefore focuses on strategy wiring, role behaviour,
benchmark orchestration, and project documentation.
