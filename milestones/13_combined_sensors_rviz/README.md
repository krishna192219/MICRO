# 13. Combined sensor visualization

[All milestones](../README.md) · [Previous](../12_laserscan_rviz/README.md)

## Engineering goal

Use one host serial reader for raw IMU, scan, cloud and wheel-pose TF.

## Actual source

- [Modular scanner](../../firmware/integrated/MIC_ROS_AMR/MIC_ROS_AMR.ino)
- [ROS scan_imu_odom](../../ros2_ws/src/robot_mapping/robot_mapping/scan_imu_odom.py)

## What this version establishes

scan_imu_odom plus modular scanner; Madgwick optional. This route does not publish /odom or EKF output.

Each sketch directory contains a source-specific run note. Use the [setup guide](../../docs/setup.md) and [serial pairing matrix](../../docs/serial_protocol.md) for ROS integration. Do not start separate same-port readers together.

## Problems, testing and next step

The [controller walkthrough](../../docs/motor_control.md), [debugging history](../../docs/troubleshooting.md) and [source review](../../docs/code_review.md) explain how this stage fits the whole system. Add new experiments with source revision, hardware geometry, procedure and raw measurements. Existing media demonstrates behavior but does not establish numeric accuracy or a specific code revision.

[Browse actual demonstration media](../../media/README.md). The next milestone is a learning progression, not a claim that every program directly compiles into the next one.
