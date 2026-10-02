# Modular sensor scanner

Open [MIC_ROS_AMR.ino](MIC_ROS_AMR.ino) with all companion source/header files in this directory. Original source bytes are preserved.

The actual sketch runs one encoder-synchronized 360° scan automatically after startup. It reads IMU and integrates wheel odometry during rotation, sends scan lifecycle markers and samples, then keeps publishing stationary POSE/IMU. This is the main firmware paired with `scan_imu_odom` in the new launch.

This folder also contains constants for straight movement, but that controller is not called here. The separate all-sensor square sketch contains `driveStraight()` and `rotate90()`. See [control details](../../../docs/motor_control.md), [startup instructions](../../../docs/setup.md) and [open findings](../../../docs/code_review.md).
