# Separate wheel RPM regulation

[Original sketch](CLOSED_LOOP_MOTOR_CONTROL_MICROS.ino) · [Source catalog](../../../docs/source_catalog.md)

Target 100 RPM, base 90/125, Kp 1 Ki 0.2 Kd 0, alpha 0.25 filtering and nominal 50 ms controller.

## Running this experiment

Check signed RPM convention for the original hardware revision. Initial timing includes startup delay; log settling separately.

Open the sketch in its identically named folder, select the actual ESP32 board and install the libraries used by its includes. See the [build guide](../../../docs/setup.md). Inspect this file's pin assignments and automatic-start behavior before applying motor power. Serial baud is 115200 unless stated otherwise above.

## Evidence and interpretation

The source is preserved byte-for-byte from the supplied archive. Hardware testing was reported by the owner; [media](../../../media/README.md) is indexed by visible behavior, without invented source-revision matches. [Code review](../../../docs/code_review.md) records issues found during repository preparation.
