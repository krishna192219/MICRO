# Complete source catalog

Every original Arduino sketch and ROS runtime node is retained. Names containing `microROS` are historical filenames; the source uses serial text rather than the micro-ROS runtime. No source revision is inferred from a video filename.

## Standalone Arduino experiments

| Source | Purpose | Actual behavior/settings |
| --- | --- | --- |
| [esp32_blueT_car](../firmware/experiments/esp32_blueT_car/esp32_blueT_car.ino) | Bluetooth driving | Commands F/B/L/R/S over BluetoothSerial device ESP 32_CAR; speed duties 90/65; uses an early pin naming scheme. |
| [esp32_CAR_MAPING](../firmware/experiments/esp32_CAR_MAPING/esp32_CAR_MAPING.ino) | Encoder-free stepper scanner | AccelStepper HALF 4 WIRE, button GPIO 4, ToF on GPIO 21/22; about ±120° sweep at nominal 4096 steps/revolution; angle,distance output at 921600 baud. |
| [motor_test_microROS](../firmware/experiments/motor_test_microROS/motor_test_microROS.ino) | Basic PWM motor bring-up | Starts at PWM 150; serial input accepts a duty number 0–255. |
| [motor_test_50_90](../firmware/experiments/motor_test_50_90/motor_test_50_90.ino) | Motor characterization | PWM 50/70/90, one-second settling then five-second accumulated-count windows and I²C error reporting. |
| [encoder_RAWTEST_MOTOR](../firmware/experiments/encoder_RAWTEST_MOTOR/encoder_RAWTEST_MOTOR.ino) | Raw and filtered encoder RPM | Fixed PWM 100, 100 ms read window, measured microsecond dt, alpha 0.20; CSV angle and RPM output. |
| [left_right_FILTER_UNFILTER_MOTOR_RPM_TES](../firmware/experiments/left_right_FILTER_UNFILTER_MOTOR_RPM_TES/left_right_FILTER_UNFILTER_MOTOR_RPM_TES.ino) | Filter comparison | PWM 63, accumulated counts, 200 ms RPM window, alpha=1. |
| [raw_vs_filter_data_encoder_fix_pwm](../firmware/experiments/raw_vs_filter_data_encoder_fix_pwm/raw_vs_filter_data_encoder_fix_pwm.ino) | Fixed-PWM count logging | PWM 63, 1000 ms reports, cumulative count differences and magnitude RPM. |
| [encoder_test_microROS](../firmware/experiments/encoder_test_microROS/encoder_test_microROS.ino) | Early encoder odometry | Wheel diameter 44 mm, track 100 mm, fixed PWM 115/150, pose printed as human-readable text. |
| [closedloopmotorcontrol_microROS](../firmware/experiments/closedloopmotorcontrol_microROS/closedloopmotorcontrol_microROS.ino) | Wheel-speed matching experiment | PI correction of leftRPM-rightRPM around duty 50 at nominal 100 ms; not a fixed RPM target. |
| [CLOSED_LOOP_MOTOR_CONTROL_MICROS](../firmware/experiments/CLOSED_LOOP_MOTOR_CONTROL_MICROS/CLOSED_LOOP_MOTOR_CONTROL_MICROS.ino) | Separate wheel RPM regulation | Target 100 RPM, base 90/125, Kp 1 Ki 0.2 Kd 0, alpha 0.25 filtering and nominal 50 ms controller. |
| [closed_loop_motor_400rpm](../firmware/experiments/closed_loop_motor_400rpm/closed_loop_motor_400rpm.ino) | 50 RPM feed-forward/P controller | Target 50 RPM, base 63/63, Kp 1.5, nominal 2000 ms control, cap 120 and slew 5. |
| [micro_ros_1meter_test](../firmware/experiments/micro_ros_1meter_test/micro_ros_1meter_test.ino) | One-metre distance task | Fixed PWM 63, count-based stop, diameter 44 mm,10 s timeout. |
| [micro_ros_360_turn](../firmware/experiments/micro_ros_360_turn/micro_ros_360_turn.ino) | Decelerating full-turn task | Target 360°, track 90 mm, PWM 63/45/30/20 stages, both-count completion,5 s timeout. |
| [micro_ros_square](../firmware/experiments/micro_ros_square/micro_ros_square.ino) | Four-sided motion task | Four 0.50 m sides and 90° turns; track 95 mm; distance-based PWM scheduling. |
| [differential_drive_closed_loop_motor_micROS](../firmware/experiments/differential_drive_closed_loop_motor_micROS/differential_drive_closed_loop_motor_micROS.ino) | Differential-drive wheel targets | v=0.230 m/s, omega=0, track 100 mm; converts to wheel target RPM and uses PI wheel feedback. |
| [gyro_callibration_microROS](../firmware/experiments/gyro_callibration_microROS/gyro_callibration_microROS.ino) | Calibrated IMU test | Reads MPU registers and subtracts stored degree/s biases; outputs IMU records every nominal 100 ms. |
| [main_test_microROS](../firmware/experiments/main_test_microROS/main_test_microROS.ino) | Early pose and ToF acquisition | POSE/SCAN records, no motor control; WHEEL_TRACK=0.95. |
| [sensor_test_microROS_nomotor](../firmware/experiments/sensor_test_microROS_nomotor/sensor_test_microROS_nomotor.ino) | Manual sensor/odometry test | POSE/SCAN records, track 100 mm, no motor control. |
| [mapping_microROS](../firmware/experiments/mapping_microROS/mapping_microROS.ino) | Range/odometry integration experiment | SCAN records, track 145 mm,60 ms range scheduling and custom ToF settings. |
| [ALL_SENSOR_COMBINED_MICROS](../firmware/experiments/ALL_SENSOR_COMBINED_MICROS/ALL_SENSOR_COMBINED_MICROS.ino) | All-sensor square controller | 105 mm track,50 RPM straight target, count sync, staged synchronized 90° turns, ENC/IMU/SCAN output. |

## Modular scanner

[Open MIC_ROS_AMR](../firmware/integrated/MIC_ROS_AMR/MIC_ROS_AMR.ino). Companion modules: Config, I2CBuses, Motors, Encoders, Odometry and imu. The original mixed filename case is preserved. Performs one synchronized 360° scan after startup, then stationary POSE/IMU updates; ENC publisher exists but is not called.

## ROS executables

| Executable/source | Input tags | Output | Notes |
| --- | --- | --- | --- |
| [serial_scan_node](../ros2_ws/src/robot_mapping/robot_mapping/serial_scan_node.py) | SCAN only | /scan | Accumulated nearest ranges with 1.2 m cap; no TF, no reset between turns. |
| [odom_scan](../ros2_ws/src/robot_mapping/robot_mapping/odom_scan.py) | SCAN | /odom, /tof_points, TF | Uses supplied pose, zero offset and 1.2 m cap; cloud up to 10000 points. |
| [odom_serial](../ros2_ws/src/robot_mapping/robot_mapping/odom_serial.py) | ENC | /odom, TF | Integrates counts with 95 mm separation and right-distance inversion. |
| [imu_serial](../ros2_ws/src/robot_mapping/robot_mapping/imu_serial.py) | IMU | /imu/data_raw, static imu TF | g/degree conversions and raw orientation marked unavailable. |
| [scan_serial_node](../ros2_ws/src/robot_mapping/robot_mapping/scan_serial_node.py) | POSE, SCAN, lifecycle markers | /scan, /scan_points, TF | Marker-based scanner;60 mm offset in cloud, no IMU publisher. |
| [scan_imu_odom](../ros2_ws/src/robot_mapping/robot_mapping/scan_imu_odom.py) | POSE, IMU, SCAN, lifecycle markers | /imu/data_raw, /scan, /scan_points, TF | Combined single reader; no ENC handling or /odom message publisher. |

The [pairing guide](serial_protocol.md) explains which programs can actually be used together. Original ROS launch/package files are retained in [archive](../archive/original_ros_packaging/).
