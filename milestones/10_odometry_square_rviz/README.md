# 10. Square motion and odometry

[All milestones](../README.md) · [Previous](../09_tof_scan_rviz/README.md) · [Next](../11_madgwick_imu_rviz/README.md)

## Engineering goal

Combine distance and heading changes and inspect the estimated trajectory.

## Actual source

- [micro_ros_square](../../firmware/experiments/micro_ros_square/micro_ros_square.ino)
- [ALL_SENSOR_COMBINED_MICROS](../../firmware/experiments/ALL_SENSOR_COMBINED_MICROS/ALL_SENSOR_COMBINED_MICROS.ino)
- [ROS odom_serial](../../ros2_ws/src/robot_mapping/robot_mapping/odom_serial.py)

## What this version establishes

Only the all-sensor variant emits cumulative ENC for odom_serial; check the 95/105 mm mismatch.

Each sketch directory contains a source-specific run note. Use the [setup guide](../../docs/setup.md) and [serial pairing matrix](../../docs/serial_protocol.md) for ROS integration. Do not start separate same-port readers together.

## Problems, testing and next step

The [controller walkthrough](../../docs/motor_control.md), [debugging history](../../docs/troubleshooting.md) and [source review](../../docs/code_review.md) explain how this stage fits the whole system. Add new experiments with source revision, hardware geometry, procedure and raw measurements. Existing media demonstrates behavior but does not establish numeric accuracy or a specific code revision.

[Browse actual demonstration media](../../media/README.md). The next milestone is a learning progression, not a claim that every program directly compiles into the next one.
