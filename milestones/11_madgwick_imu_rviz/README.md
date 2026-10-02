# 11. Raw IMU to orientation

[All milestones](../README.md) · [Previous](../10_odometry_square_rviz/README.md) · [Next](../12_laserscan_rviz/README.md)

## Engineering goal

Convert units, publish raw IMU and estimate orientation on the PC.

## Actual source

- [gyro_callibration_microROS](../../firmware/experiments/gyro_callibration_microROS/gyro_callibration_microROS.ino)
- [ROS imu_serial](../../ros2_ws/src/robot_mapping/robot_mapping/imu_serial.py)

## What this version establishes

imu_serial plus Madgwick; magnetometer disabled. IMU orientation is distinct from EKF robot state.

Each sketch directory contains a source-specific run note. Use the [setup guide](../../docs/setup.md) and [serial pairing matrix](../../docs/serial_protocol.md) for ROS integration. Do not start separate same-port readers together.

## Problems, testing and next step

The [controller walkthrough](../../docs/motor_control.md), [debugging history](../../docs/troubleshooting.md) and [source review](../../docs/code_review.md) explain how this stage fits the whole system. Add new experiments with source revision, hardware geometry, procedure and raw measurements. Existing media demonstrates behavior but does not establish numeric accuracy or a specific code revision.

[Browse actual demonstration media](../../media/README.md). The next milestone is a learning progression, not a claim that every program directly compiles into the next one.
