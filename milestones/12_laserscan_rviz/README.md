# 12. Rotating LaserScan reconstruction

[All milestones](../README.md) · [Previous](../11_madgwick_imu_rviz/README.md) · [Next](../13_combined_sensors_rviz/README.md)

## Engineering goal

Rotate the robot with the modular scanner and publish cloud/scan output.

## Actual source

- [Modular scanner](../../firmware/integrated/MIC_ROS_AMR/MIC_ROS_AMR.ino)
- [ROS scan_serial_node](../../ros2_ws/src/robot_mapping/robot_mapping/scan_serial_node.py)

## What this version establishes

scan_serial_node handles markers and pose; frame/timing issues are documented before SLAM.

Each sketch directory contains a source-specific run note. Use the [setup guide](../../docs/setup.md) and [serial pairing matrix](../../docs/serial_protocol.md) for ROS integration. Do not start separate same-port readers together.

## Problems, testing and next step

The [controller walkthrough](../../docs/motor_control.md), [debugging history](../../docs/troubleshooting.md) and [source review](../../docs/code_review.md) explain how this stage fits the whole system. Add new experiments with source revision, hardware geometry, procedure and raw measurements. Existing media demonstrates behavior but does not establish numeric accuracy or a specific code revision.

[Browse actual demonstration media](../../media/README.md). The next milestone is a learning progression, not a claim that every program directly compiles into the next one.
