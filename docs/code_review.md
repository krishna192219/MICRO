# Source review: open engineering findings

These findings come from reading the supplied source. They are not claimed as newly reproduced hardware failures. Firmware and ROS runtime nodes remain unchanged; packaging fixes are listed separately.

| Priority | Finding | Implication / next action |
| --- | --- | --- |
| Before reuse | `closedloopmotorcontrol_microROS` references undeclared `I2C_Left` | Historical sketch will not compile as supplied; restore intended bus object and check pins |
| Before motion tests | Several sketches auto-start; modular scanner proceeds after sensor-init errors | Confirm hardware initialization and startup behavior; add fail-closed gating before unattended use |
| Before accuracy claims | One-metre stop checks positive raw right count although forward right is negative | Left threshold governs normal completion; decide average/both-wheel signed-distance policy |
| Before EKF | No EKF files in archive; combined node publishes no `/odom` | Restore configuration and a timestamped odometry input; disable raw TF when EKF takes ownership |
| Before SLAM | LaserScan frame, origin, timestamps and last angle disagree with accumulated data | Reproject consistently and validate against known geometry |
| Integration | Square all-sensor firmware has no POSE or scan lifecycle markers | Do not pair it with marker-based combined scanner and expect complete outputs |
| Integration | Modular `publishEncoderData()` never called | Startup text lists ENC, but no live ENC stream on that route |
| Timing | All-sensor straight controller divides by nominal 100 ms despite blocking sensing | Use actual dt and log loop jitter before retuning gains |
| Historical geometry | 0.090/0.095/0.100/0.145 m variants; `main_test_microROS` has 0.95 m | Preserve as history; current hardware is 0.105 m |
| Historical initialization | `main_test_microROS` initializes previous readings from opposite bus objects to update | Initial count jump possible; fix bus correspondence before use |
| Signed velocity | Differential-drive experiment clamps forward-only PWM | Negative wheel targets are not a complete reverse-drive implementation |
| Measurement integrity | Some older I²C readers return 0 on error | An error can masquerade as a real angle; use explicit validity |
| Scan completion | Timeout emits SCAN_END and completion text | Distinguish partial scan from completed revolution |
| Sensor identity | Firmware comment mentions WHO_AM_I 0x70 | Confirm exact IMU silicon/module identity |
| Host robustness | Hardcoded port, timeout-based readline, broad exception handling | Parameterize, buffer complete records and implement reconnect behavior |
| Results delivery | One-shot scan publication with default QoS | Start RViz before scan; consider deliberate replay/latching design |
| Calibration | Gyro calibration-named sketch only subtracts constants | Add actual bias acquisition procedure and raw data |

## Fixed for this distribution

- ROS package now installs launch and RViz assets.
- Runtime dependencies are declared, including serial and point-cloud helper modules.
- Default launch uses package `robot_mapping`, one combined serial reader, one owner per TF edge, and optional Madgwick/RViz processes.
- Original launch and package metadata are retained for traceability.

## Preserved intentionally

No control gains, pin assignments, odometry signs or scan algorithms have been silently changed. These are hardware-sensitive experiments. Their limitations are documented so the repository remains an honest account of the work and a useful starting point for measured improvements.
