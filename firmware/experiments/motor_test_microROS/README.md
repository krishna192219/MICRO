# Basic PWM motor bring-up

[Original sketch](motor_test_microROS.ino) · [Source catalog](../../../docs/source_catalog.md)

Starts at PWM 150; serial input accepts a duty number 0–255.

## Running this experiment

115200 baud; auto-starts in setup. Send 0 to stop. Check this version’s direction pin definitions.

Open the sketch in its identically named folder, select the actual ESP32 board and install the libraries used by its includes. See the [build guide](../../../docs/setup.md). Inspect this file's pin assignments and automatic-start behavior before applying motor power. Serial baud is 115200 unless stated otherwise above.

## Evidence and interpretation

The source is preserved byte-for-byte from the supplied archive. Hardware testing was reported by the owner; [media](../../../media/README.md) is indexed by visible behavior, without invented source-revision matches. [Code review](../../../docs/code_review.md) records issues found during repository preparation.
