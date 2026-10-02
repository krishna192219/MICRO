# Build and run

Target: Ubuntu 22.04 and ROS 2 Humble. The selected integration is the modular scanner plus the combined ROS reader. The source is present; hardware runs and a complete ROS build still need to be repeated on the robot's host.

## 1. Select and upload firmware

Open `firmware/integrated/MIC_ROS_AMR/MIC_ROS_AMR.ino` in Arduino IDE with its companion files in the same folder. Select the actual ESP32 board. The code uses the pin-based `ledcAttach` API associated with Arduino-ESP32 3.x; the exact previously tested core version was not recorded. Install Adafruit VL53L0X and its required dependencies. The historical stepper sketch additionally needs AccelStepper; the Bluetooth sketch needs a board supporting its BluetoothSerial implementation.

See the [Espressif migration guide](https://docs.espressif.com/projects/arduino-esp32/en/latest/migration_guides/2.x_to_3.0.html) for the LEDC API change. Record the exact successful board/core/library versions after reproducing the build.

**Startup behavior:** the modular firmware automatically attempts one scan after initialization and a two-second delay, then streams pose/IMU while stationary. Reset triggers another attempt. It logs initialization errors but does not block all motion on those errors. Start with wheels clear while verifying direction and sensor readiness; use a clear test area for the scan.

## 2. Build the ROS package

Assume this repository is at `~/mic_ros_amr`, ROS Humble/colcon/rosdep are installed and rosdep initialized:

```bash
source /opt/ros/humble/setup.bash
cd ~/mic_ros_amr/ros2_ws
rosdep install --from-paths src --ignore-src -r -y
colcon build --symlink-install
source install/setup.bash
ros2 pkg executables robot_mapping
```

Declared dependencies include `rclpy`, message packages, `tf2_ros`, `sensor_msgs_py`, `python3-serial`, launch support, RViz and `imu_filter_madgwick`.

## 3. Start the combined route

Close Arduino Serial Monitor and other serial readers. Confirm the actual port is `/dev/ttyUSB0` and that the current user has serial-device access. The preserved scripts hardcode that path and 115200 baud; edit the chosen script if your device path differs.

```bash
ros2 launch robot_mapping amr_sensor.launch.py
```

This repository-authored launch starts:

- `robot_mapping/scan_imu_odom`: the only serial reader, sensor/static TF and raw wheel-pose TF owner.
- Madgwick: raw IMU to filtered orientation, `use_mag=false`, `publish_tf=false`.
- RViz: a new starter view for TF, point cloud and scan.

Optional switches:

```bash
ros2 launch robot_mapping amr_sensor.launch.py rviz:=false
ros2 launch robot_mapping amr_sensor.launch.py madgwick:=false
```

Wait for ROS/RViz subscribers to be ready, then reset the robot for the scan. If opening serial resets the ESP32 before RViz is ready, reset once more after the display connects. Scan/cloud publish once at SCAN_END, so subscribing after completion can miss them. The starter RViz view uses `odom`; `/scan_points` and `/scan` have different geometry limitations described in the scan guide.

## 4. Inspect

```bash
ros2 topic list
ros2 topic echo /imu/data_raw --once
ros2 topic echo /imu/data --once
ros2 run tf2_ros tf2_echo odom base_link
ros2 run tf2_ros tf2_echo base_link laser
ros2 run tf2_ros tf2_echo base_link imu_link
```

Start `ros2 topic echo /scan --once` before triggering the scan. Expected combined topics: `/scan`, `/scan_points`, `/imu/data_raw`, `/imu/data`, `/tf`, `/tf_static`. `/odom` and `/odometry/filtered` are not published by this route.

## Standalone modes

```bash
# Calibrated IMU sketch only; do not run beside the combined reader
ros2 run robot_mapping imu_serial

# Marker-based scanner, no IMU publishing
ros2 run robot_mapping scan_serial_node

# ENC-based historical odometry; check wheel separation first
ros2 run robot_mapping odom_serial
```

Use the [pairing matrix](serial_protocol.md) for other modes. None of the supplied ROS readers understands the stepper prototype's untagged angle/distance stream. Madgwick visualization alone does not require an EKF. No EKF/SLAM/Nav2 startup command is provided because their tested configuration is absent.
