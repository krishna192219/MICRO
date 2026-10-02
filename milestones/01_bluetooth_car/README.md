# 01. Bluetooth and motor bring-up

[All milestones](../README.md) · [Next](../02_scan_only_prototype/README.md)

## Engineering goal

Establish basic motor commands and physical directions before adding feedback.

## Actual source

- [motor_test_microROS](../../firmware/experiments/motor_test_microROS/motor_test_microROS.ino)
- [esp32_blueT_car](../../firmware/experiments/esp32_blueT_car/esp32_blueT_car.ino)

## What this version establishes

Manual direction/PWM control is implemented; historical pins differ from the current modular version.

Each sketch directory contains a source-specific run note. Use the [setup guide](../../docs/setup.md) and [serial pairing matrix](../../docs/serial_protocol.md) for ROS integration. Do not start separate same-port readers together.

## Problems, testing and next step

The [controller walkthrough](../../docs/motor_control.md), [debugging history](../../docs/troubleshooting.md) and [source review](../../docs/code_review.md) explain how this stage fits the whole system. Add new experiments with source revision, hardware geometry, procedure and raw measurements. Existing media demonstrates behavior but does not establish numeric accuracy or a specific code revision.

[Browse actual demonstration media](../../media/README.md). The next milestone is a learning progression, not a claim that every program directly compiles into the next one.
