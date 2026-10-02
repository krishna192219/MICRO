# robot_mapping

Six original serial executables are retained. The default launch uses `scan_imu_odom` with the modular scanner, Madgwick and a new starter RViz view. See the [setup guide](../../../docs/setup.md), [pairing matrix](../../../docs/serial_protocol.md) and [source review](../../../docs/code_review.md).

Do not start multiple serial executables on one device. This package does not yet provide a reproducible EKF, SLAM or navigation launch.
