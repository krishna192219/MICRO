# Serial protocols and compatible pairs

Most files use 115200 baud and host `/dev/ttyUSB0`. The stepper prototype uses 921600 baud with untagged angle/distance lines, so it is incompatible with the supplied ROS scan parsers.

| Record | Units / meaning |
| --- | --- |
| `ENC,L,R` | Cumulative raw-direction wheel counts; sign policy depends on firmware |
| `POSE,x,y,heading` | Metres and radians, already integrated on ESP32 |
| `IMU,ax,ay,az,gx,gy,gz` | g and degrees/s; gyro bias already subtracted |
| `SCAN,x,y,heading,distance` | Pose in metres/radians, range in millimetres |
| `SCAN_START`, `SCAN_END` | Full-revolution acquisition lifecycle |
| `SCAN_INVALID` and debug text | Not sensor records; parsers ignore them |

## Pairing matrix

| Firmware | ROS executable | Behavior / caveat |
| --- | --- | --- |
| `MIC_ROS_AMR` | `scan_imu_odom` | Main combined scan route: consumes POSE/IMU/SCAN and markers |
| `MIC_ROS_AMR` | `scan_serial_node` | Same scanner without IMU publication |
| `gyro_callibration_microROS` | `imu_serial` | Calibrated raw IMU only, can feed Madgwick |
| `ALL_SENSOR_COMBINED_MICROS` | `odom_serial` | ENC input; ROS uses historical 0.095 m separation while firmware uses 0.105 m; align and test before comparing |
| `ALL_SENSOR_COMBINED_MICROS` or SCAN-producing experiments | `odom_scan` | SCAN-derived odometry/cloud; old zero sensor offset and 1.2 m limit |
| SCAN-producing firmware | `serial_scan_node` | Accumulated bins, no TF or scan reset; inspect in laser fixed frame |
| `esp32_CAR_MAPING` | None supplied | Untagged angle/distance at 921600; matching bridge not included |

Run only one row/serial reader at a time. The standalone scripts all open the same device; starting several does not broadcast the serial stream to each. The combined reader solves that for the marker-based scanner, but does not parse ENC or emit `/odom`.

## Actual modular emission

`publishEncoderData()` exists and is rate-limited to 50 ms, but the current sketch never calls it. Consequently the modular route emits POSE, IMU, SCAN and markers, not a live ENC stream despite the startup text listing ENC. This is why `odom_serial` is not the selected reader for that route.

The combined node stamps data at host receipt/publication time. Firmware does not send measurement timestamps. `readline()` uses a timeout and partial-line buffering is not explicitly handled; reconnection and robust buffering are future improvements.
