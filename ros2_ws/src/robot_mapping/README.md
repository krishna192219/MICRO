# robot_mapping

Six original serial executables are retained. The default launch uses `scan_imu_odom` with the modular scanner, Madgwick and a new starter RViz view. See the [setup guide](../../../docs/setup.md), [pairing matrix](../../../docs/serial_protocol.md) and [source review](../../../docs/code_review.md).

Do not start multiple serial executables on one device. An experimental `ekf.launch.py` is now included: see [EKF setup](../../../docs/ekf_setup.md). Use it instead of the original scan launch. SLAM/navigation remain future work.
