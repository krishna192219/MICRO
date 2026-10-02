# Madgwick, EKF and what the archive supports

Madgwick processes `/imu/data_raw` into `/imu/data`; the repository launch configures it without a magnetometer. This is distinct from robot state estimation.

Earlier development notes report `robot_localization` EKF working/integrated. The supplied ZIP contains no EKF YAML, EKF launch, `/odometry/filtered` publisher implementation or recorded filtered-odometry result. Treat EKF as reported development work whose reproducible configuration still needs restoration, not a working out-of-box feature of this snapshot.

The intended EKF combines wheel odometry and inertial information probabilistically. To restore it, first provide a consistent wheel Odometry message from the single-reader route, measured time intervals and justified covariances; select fusion variables to avoid double-counting correlated pose/velocity; then configure one owner for `odom → base_link`. The current combined node broadcasts that transform unconditionally and has no disable parameter.

Do not launch an EKF alongside the existing raw TF publisher without changing ownership. The package currently launches Madgwick only. SLAM and Nav2 remain planned.
