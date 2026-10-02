# 06. One-metre travel

[All milestones](../README.md) · [Previous](../05_closed_loop_motor_control/README.md) · [Next](../07_controlled_turns/README.md)

## Engineering goal

Use encoder distance to stop a physical travel task.

## Actual source

- [micro_ros_1meter_test](../../firmware/experiments/micro_ros_1meter_test/micro_ros_1meter_test.ino)

## What this version establishes

Fixed-duty drive with feedback stop; sign-dependent stop condition requires review before asserting accuracy.

Each sketch directory contains a source-specific run note. Use the [setup guide](../../docs/setup.md) and [serial pairing matrix](../../docs/serial_protocol.md) for ROS integration. Do not start separate same-port readers together.

## Problems, testing and next step

The [controller walkthrough](../../docs/motor_control.md), [debugging history](../../docs/troubleshooting.md) and [source review](../../docs/code_review.md) explain how this stage fits the whole system. Add new experiments with source revision, hardware geometry, procedure and raw measurements. Existing media demonstrates behavior but does not establish numeric accuracy or a specific code revision.

[Browse actual demonstration media](../../media/README.md). The next milestone is a learning progression, not a claim that every program directly compiles into the next one.
