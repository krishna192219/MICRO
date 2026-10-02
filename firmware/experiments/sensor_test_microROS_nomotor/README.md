# Manual sensor/odometry test

[Original sketch](sensor_test_microROS_nomotor.ino) · [Source catalog](../../../docs/source_catalog.md)

POSE/SCAN records, track 100 mm, no motor control.

## Running this experiment

Useful as a historical manual-motion test. Sign convention and origin differ from the later modular scanner; no scan lifecycle markers.

Open the sketch in its identically named folder, select the actual ESP32 board and install the libraries used by its includes. See the [build guide](../../../docs/setup.md). Inspect this file's pin assignments and automatic-start behavior before applying motor power. Serial baud is 115200 unless stated otherwise above.

## Evidence and interpretation

The source is preserved byte-for-byte from the supplied archive. Hardware testing was reported by the owner; [media](../../../media/README.md) is indexed by visible behavior, without invented source-revision matches. [Code review](../../../docs/code_review.md) records issues found during repository preparation.
