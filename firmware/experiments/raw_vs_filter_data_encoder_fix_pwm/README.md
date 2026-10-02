# Fixed-PWM count logging

[Original sketch](raw_vs_filter_data_encoder_fix_pwm.ino) · [Source catalog](../../../docs/source_catalog.md)

PWM 63, 1000 ms reports, cumulative count differences and magnitude RPM.

## Running this experiment

Despite its name this version has no smoothing filter. Record CSV for comparison with the other tests.

Open the sketch in its identically named folder, select the actual ESP32 board and install the libraries used by its includes. See the [build guide](../../../docs/setup.md). Inspect this file's pin assignments and automatic-start behavior before applying motor power. Serial baud is 115200 unless stated otherwise above.

## Evidence and interpretation

The source is preserved byte-for-byte from the supplied archive. Hardware testing was reported by the owner; [media](../../../media/README.md) is indexed by visible behavior, without invented source-revision matches. [Code review](../../../docs/code_review.md) records issues found during repository preparation.
