# 03. Encoder characterization

[All milestones](../README.md) · [Previous](../02_scan_only_prototype/README.md) · [Next](../04_gyro_calibration/README.md)

## Engineering goal

Resolve signs, wraparound, motor mismatch and raw/filtered speed behavior.

## Actual source

- [motor_test_50_90](../../firmware/experiments/motor_test_50_90/motor_test_50_90.ino)
- [encoder_RAWTEST_MOTOR](../../firmware/experiments/encoder_RAWTEST_MOTOR/encoder_RAWTEST_MOTOR.ino)
- [left_right_FILTER_UNFILTER_MOTOR_RPM_TES](../../firmware/experiments/left_right_FILTER_UNFILTER_MOTOR_RPM_TES/left_right_FILTER_UNFILTER_MOTOR_RPM_TES.ino)
- [raw_vs_filter_data_encoder_fix_pwm](../../firmware/experiments/raw_vs_filter_data_encoder_fix_pwm/raw_vs_filter_data_encoder_fix_pwm.ino)
- [encoder_test_microROS](../../firmware/experiments/encoder_test_microROS/encoder_test_microROS.ino)

## What this version establishes

Comparison experiments are present; residual RPM fluctuations remain a documented investigation.

Each sketch directory contains a source-specific run note. Use the [setup guide](../../docs/setup.md) and [serial pairing matrix](../../docs/serial_protocol.md) for ROS integration. Do not start separate same-port readers together.

## Problems, testing and next step

The [controller walkthrough](../../docs/motor_control.md), [debugging history](../../docs/troubleshooting.md) and [source review](../../docs/code_review.md) explain how this stage fits the whole system. Add new experiments with source revision, hardware geometry, procedure and raw measurements. Existing media demonstrates behavior but does not establish numeric accuracy or a specific code revision.

[Browse actual demonstration media](../../media/README.md). The next milestone is a learning progression, not a claim that every program directly compiles into the next one.
