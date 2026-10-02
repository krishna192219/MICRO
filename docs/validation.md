# Validation and evidence boundary

## Distribution checks

- All original firmware, six ROS runtime nodes, photos, videos, chassis STL and TF captures are retained byte-for-byte. Original packaging files are retained separately because distributable setup/manifest/launch were revised.
- Python source and launch files parse successfully.
- Six console entry points resolve to actual modules and main functions.
- Setup data-file selection includes real launch and RViz assets.
- Package XML, issue-form YAML and starter RViz YAML parse successfully.
- Relative Markdown links and fenced blocks are checked with `python3 tools/check_repository.py`.
- All 10 video containers were inspected and three frames per clip sampled; descriptions avoid claiming exact source revisions. Original clips remain intact.
- Binary STL length matches 1344 declared triangles; mesh units/printability are not established.

## Tests not performed here

Arduino CLI/ESP32 build dependencies, ROS 2/colcon and robot hardware are unavailable in the repository-preparation environment. No Arduino compile, colcon build, live launch/RViz test, serial replay or physical motion test was performed. Syntax/packaging checks do not establish runtime correctness.

## Known issues retained

The source review records an undeclared bus in one historical sketch, geometry differences, a one-metre stop-sign issue, scan metadata/frame limitations and missing EKF configuration. The new launch/RViz packaging still requires a host/hardware trial. Original project template lint/copyright tests are preserved but have not been run; their presence is not a claim that lint passes.

## Evidence levels

Source inspection establishes implemented logic; supplied media shows qualitative behavior; the owner reports historical tuning/tests. No measured mapping/localization accuracy, repeated physical error distribution, autonomous goal result or loop-closure result is supplied.
