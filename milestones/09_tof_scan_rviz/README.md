# 09. ToF and pose transport

[All milestones](../README.md) · [Previous](../08_differential_drive/README.md) · [Next](../10_odometry_square_rviz/README.md)

## Engineering goal

Bring pose-associated distance records into PC visualization.

## Actual source

- [main_test_microROS](../../firmware/experiments/main_test_microROS/main_test_microROS.ino)
- [sensor_test_microROS_nomotor](../../firmware/experiments/sensor_test_microROS_nomotor/sensor_test_microROS_nomotor.ino)
- [mapping_microROS](../../firmware/experiments/mapping_microROS/mapping_microROS.ino)
- [ROS odom_scan](../../ros2_ws/src/robot_mapping/robot_mapping/odom_scan.py)
- [ROS serial_scan_node](../../ros2_ws/src/robot_mapping/robot_mapping/serial_scan_node.py)

## What this version establishes

Use compatible SCAN parsers; these early versions do not emit full-turn lifecycle markers.

Each sketch directory contains a source-specific run note. Use the [setup guide](../../docs/setup.md) and [serial pairing matrix](../../docs/serial_protocol.md) for ROS integration. Do not start separate same-port readers together.

## Problems, testing and next step

The [controller walkthrough](../../docs/motor_control.md), [debugging history](../../docs/troubleshooting.md) and [source review](../../docs/code_review.md) explain how this stage fits the whole system. Add new experiments with source revision, hardware geometry, procedure and raw measurements. Existing media demonstrates behavior but does not establish numeric accuracy or a specific code revision.

[Browse actual demonstration media](../../media/README.md). The next milestone is a learning progression, not a claim that every program directly compiles into the next one.
