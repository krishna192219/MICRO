# 05. Wheel-speed feedback

[All milestones](../README.md) · [Previous](../04_gyro_calibration/README.md) · [Next](../06_straight_line_1m/README.md)

## Engineering goal

Progress from speed matching to separate wheel targets and feed-forward correction.

## Actual source

- [closedloopmotorcontrol_microROS](../../firmware/experiments/closedloopmotorcontrol_microROS/closedloopmotorcontrol_microROS.ino)
- [CLOSED_LOOP_MOTOR_CONTROL_MICROS](../../firmware/experiments/CLOSED_LOOP_MOTOR_CONTROL_MICROS/CLOSED_LOOP_MOTOR_CONTROL_MICROS.ino)
- [closed_loop_motor_400rpm](../../firmware/experiments/closed_loop_motor_400rpm/closed_loop_motor_400rpm.ino)

## What this version establishes

Different versions use different gains, target speeds and timing; one historical bus declaration is missing.

Each sketch directory contains a source-specific run note. Use the [setup guide](../../docs/setup.md) and [serial pairing matrix](../../docs/serial_protocol.md) for ROS integration. Do not start separate same-port readers together.

## Problems, testing and next step

The [controller walkthrough](../../docs/motor_control.md), [debugging history](../../docs/troubleshooting.md) and [source review](../../docs/code_review.md) explain how this stage fits the whole system. Add new experiments with source revision, hardware geometry, procedure and raw measurements. Existing media demonstrates behavior but does not establish numeric accuracy or a specific code revision.

[Browse actual demonstration media](../../media/README.md). The next milestone is a learning progression, not a claim that every program directly compiles into the next one.
