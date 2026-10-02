# Build the system step by step

These 13 stages link to the original source, rather than empty placeholders. They provide a learning progression through the project; original chronological ordering is not asserted where it is unknown. The all-sensor square and modular scan firmware are distinct integration branches.

| Stage | Goal |
| --- | --- |
| [01. Bluetooth and motor bring-up](01_bluetooth_car/README.md) | Establish basic motor commands and physical directions before adding feedback. |
| [02. First encoder-free scanner](02_scan_only_prototype/README.md) | Reconstruct angular range samples using a stepper-driven ToF sweep. |
| [03. Encoder characterization](03_encoder_testing/README.md) | Resolve signs, wraparound, motor mismatch and raw/filtered speed behavior. |
| [04. Gyro bias correction](04_gyro_calibration/README.md) | Apply measured offsets before feeding inertial data into ROS. |
| [05. Wheel-speed feedback](05_closed_loop_motor_control/README.md) | Progress from speed matching to separate wheel targets and feed-forward correction. |
| [06. One-metre travel](06_straight_line_1m/README.md) | Use encoder distance to stop a physical travel task. |
| [07. Full turns and quarter-turns](07_controlled_turns/README.md) | Use count targets, staged slowdown and later wheel synchronization during turns. |
| [08. Differential-drive targets](08_differential_drive/README.md) | Convert body velocity constants into wheel RPM targets. |
| [09. ToF and pose transport](09_tof_scan_rviz/README.md) | Bring pose-associated distance records into PC visualization. |
| [10. Square motion and odometry](10_odometry_square_rviz/README.md) | Combine distance and heading changes and inspect the estimated trajectory. |
| [11. Raw IMU to orientation](11_madgwick_imu_rviz/README.md) | Convert units, publish raw IMU and estimate orientation on the PC. |
| [12. Rotating LaserScan reconstruction](12_laserscan_rviz/README.md) | Rotate the robot with the modular scanner and publish cloud/scan output. |
| [13. Combined sensor visualization](13_combined_sensors_rviz/README.md) | Use one host serial reader for raw IMU, scan, cloud and wheel-pose TF. |

[Complete source catalog](../docs/source_catalog.md) · [Setup](../docs/setup.md) · [Demonstrations](../media/README.md)
