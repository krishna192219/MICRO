# 02. First encoder-free scanner

[All milestones](../README.md) · [Previous](../01_bluetooth_car/README.md) · [Next](../03_encoder_testing/README.md)

## Engineering goal

Reconstruct angular range samples using a stepper-driven ToF sweep.

## Actual source

- [esp32_CAR_MAPING](../../firmware/experiments/esp32_CAR_MAPING/esp32_CAR_MAPING.ino)

## What this version establishes

Source confirms a stepper and button with about 240° sweep, not wheel-derived 360° heading. Matching ROS bridge is absent.

Each sketch directory contains a source-specific run note. Use the [setup guide](../../docs/setup.md) and [serial pairing matrix](../../docs/serial_protocol.md) for ROS integration. Do not start separate same-port readers together.

## Problems, testing and next step

The [controller walkthrough](../../docs/motor_control.md), [debugging history](../../docs/troubleshooting.md) and [source review](../../docs/code_review.md) explain how this stage fits the whole system. Add new experiments with source revision, hardware geometry, procedure and raw measurements. Existing media demonstrates behavior but does not establish numeric accuracy or a specific code revision.

[Browse actual demonstration media](../../media/README.md). The next milestone is a learning progression, not a claim that every program directly compiles into the next one.
