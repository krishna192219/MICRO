# Encoder development and characterization

Each AS5600 supplies a 12-bit absolute angle. The software computes current minus previous, subtracting 4096 above +2048 and adding 4096 below -2048. This shortest-step method requires less than half a revolution between valid reads. Acquisition and publication intervals are different; retries or blocking work can affect the acquisition interval.

The real investigation included a faulty motor replacement, encoder/motor direction tests, magnet alignment/off-centering, fixed PWM and raw/filtered RPM comparisons. Earlier notes report approximately 1 RPM behavior in favorable conditions and 15–30 RPM fluctuations at other times. These are observations without a reference instrument/error distribution, not an encoder accuracy specification.

| Sketch | Purpose / actual setting |
| --- | --- |
| `encoder_RAWTEST_MOTOR` | PWM 100, samples every nominal 100 ms, alpha 0.20, CSV raw/filtered RPM |
| `raw_vs_filter_data_encoder_fix_pwm` | PWM 63, continuous count accumulation, nominal 1 s reporting; despite name, no smoothing stage |
| `left_right_FILTER_UNFILTER_MOTOR_RPM_TES` | PWM 63, nominal 200 ms speed window, alpha=1 disables smoothing |
| `motor_test_50_90` | Five-second averages at PWM 50/70/90, read-error counts |
| `encoder_test_microROS` | Early wheel-distance/pose experiment at PWM 115/150, 0.100 m track |

Some early readers return zero on I²C failure, indistinguishable from a valid zero angle. Later readers use -1/failure status and retries. Record error events along with timing before interpreting every fluctuation as mechanical speed change.

Future data should include time, raw angles, wrapped deltas, sign convention, measured dt, PWM, voltage, load and filter coefficient. No raw calibration CSV was supplied.
