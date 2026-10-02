# Changes made while preparing this repository

## Preserved

All 21 Arduino sketches (20 standalone and one modular sketch), modular companion files, all six ROS runtime nodes, original photos/videos, chassis mesh and TF captures are copied unchanged. `source_manifest.csv` records original paths, destination paths and source SHA-256 hashes. Preview JPEGs are extracted from real videos; no synthetic results are used.

## Packaging changes

The distributable `setup.py` now installs launch and RViz files and replaces the generic package description. `package.xml` adds missing runtime/build dependencies and a meaningful description. Original maintainer attribution and undecided licensing remain. Original setup, manifest and launch files are copied to `archive/original_ros_packaging/`.

The default `amr_sensor.launch.py` is rewritten to use the existing combined reader from `robot_mapping`. The supplied launch targeted absent `amr_odometry`, started separate same-port readers and duplicated IMU static TF. The new launch uses one reader plus optional Madgwick and RViz. A starter RViz configuration is newly authored; no saved `.rviz` file existed in the uploaded archive.

Firmware and ROS runtime algorithms are not modified. Known defects and missing EKF reproduction files are documented in the code review rather than disguised as verified fixes. Generated build/install/log/cache trees are omitted; their paths are listed in `excluded_generated_files.txt`. The original archive remains the source for those disposable artifacts.

## Validation boundary

The package was checked for source preservation, Python syntax, XML/YAML readability, installed data-file selection, entry points and relative links. ROS runtime, Arduino compilation and physical behavior were not tested in this environment. See `validation.md` for the final check results.
