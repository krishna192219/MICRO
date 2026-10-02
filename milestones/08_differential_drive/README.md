# 08. Differential-drive targets

[All milestones](../README.md) · [Previous](../07_controlled_turns/README.md) · [Next](../09_tof_scan_rviz/README.md)

## Engineering goal

Convert body velocity constants into wheel RPM targets.

## Actual source

- [differential_drive_closed_loop_motor_micROS](../../firmware/experiments/differential_drive_closed_loop_motor_micROS/differential_drive_closed_loop_motor_micROS.ino)

## What this version establishes

Forward-only helper limitation and no ROS velocity command bridge.

Each sketch directory contains a source-specific run note. Use the [setup guide](../../docs/setup.md) and [serial pairing matrix](../../docs/serial_protocol.md) for ROS integration. Do not start separate same-port readers together.

## Problems, testing and next step

The [controller walkthrough](../../docs/motor_control.md), [debugging history](../../docs/troubleshooting.md) and [source review](../../docs/code_review.md) explain how this stage fits the whole system. Add new experiments with source revision, hardware geometry, procedure and raw measurements. Existing media demonstrates behavior but does not establish numeric accuracy or a specific code revision.

[Browse actual demonstration media](../../media/README.md). The next milestone is a learning progression, not a claim that every program directly compiles into the next one.
