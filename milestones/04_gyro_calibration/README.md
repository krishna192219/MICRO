# 04. Gyro bias correction

[All milestones](../README.md) · [Previous](../03_encoder_testing/README.md) · [Next](../05_closed_loop_motor_control/README.md)

## Engineering goal

Apply measured offsets before feeding inertial data into ROS.

## Actual source

- [gyro_callibration_microROS](../../firmware/experiments/gyro_callibration_microROS/gyro_callibration_microROS.ino)

## What this version establishes

This supplied sketch tests existing biases rather than estimating new ones.

Each sketch directory contains a source-specific run note. Use the [setup guide](../../docs/setup.md) and [serial pairing matrix](../../docs/serial_protocol.md) for ROS integration. Do not start separate same-port readers together.

## Problems, testing and next step

The [controller walkthrough](../../docs/motor_control.md), [debugging history](../../docs/troubleshooting.md) and [source review](../../docs/code_review.md) explain how this stage fits the whole system. Add new experiments with source revision, hardware geometry, procedure and raw measurements. Existing media demonstrates behavior but does not establish numeric accuracy or a specific code revision.

[Browse actual demonstration media](../../media/README.md). The next milestone is a learning progression, not a claim that every program directly compiles into the next one.
