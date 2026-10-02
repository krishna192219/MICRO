# 07. Full turns and quarter-turns

[All milestones](../README.md) · [Previous](../06_straight_line_1m/README.md) · [Next](../08_differential_drive/README.md)

## Engineering goal

Use count targets, staged slowdown and later wheel synchronization during turns.

## Actual source

- [micro_ros_360_turn](../../firmware/experiments/micro_ros_360_turn/micro_ros_360_turn.ino)
- [micro_ros_square](../../firmware/experiments/micro_ros_square/micro_ros_square.ino)
- [ALL_SENSOR_COMBINED_MICROS](../../firmware/experiments/ALL_SENSOR_COMBINED_MICROS/ALL_SENSOR_COMBINED_MICROS.ino)

## What this version establishes

360° standalone target; 90° routines in square sketches. Preserve each geometry/gain set.

Each sketch directory contains a source-specific run note. Use the [setup guide](../../docs/setup.md) and [serial pairing matrix](../../docs/serial_protocol.md) for ROS integration. Do not start separate same-port readers together.

## Problems, testing and next step

The [controller walkthrough](../../docs/motor_control.md), [debugging history](../../docs/troubleshooting.md) and [source review](../../docs/code_review.md) explain how this stage fits the whole system. Add new experiments with source revision, hardware geometry, procedure and raw measurements. Existing media demonstrates behavior but does not establish numeric accuracy or a specific code revision.

[Browse actual demonstration media](../../media/README.md). The next milestone is a learning progression, not a claim that every program directly compiles into the next one.
