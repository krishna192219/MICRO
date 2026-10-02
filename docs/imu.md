# IMU acquisition and orientation estimation

The firmware directly reads the MPU9250 module at `0x68` on GPIO16/23. It configures ±2 g and ±250 degrees/s, using 16384 LSB/g and 131 LSB/(degrees/s). A 14-byte burst beginning at `0x3B` contains accelerometer, temperature and gyro data. WHO_AM_I is read at `0x75` and wakeup uses `0x6B`.

Biases X=-2.889328, Y=-1.042649 and Z=-0.273710 are subtracted after scaling gyro readings into degrees/s. `gyro_callibration_microROS` applies these existing constants; it does not calculate new biases through stationary averaging. A new bias-estimation procedure/raw calibration dataset is not present.

`imu_serial` and `scan_imu_odom` convert g by 9.80665 and degrees/s to radians/s. Raw orientation is unavailable (`orientation_covariance[0]=-1`), angular-rate covariance diagonal is 0.01 and acceleration covariance diagonal is 0.1. These are source settings, not measured noise estimates.

Madgwick runs on the PC with `use_mag=false`, `world_frame=enu`, and `publish_tf=false`. The magnetometer is unused. `/imu/data` is an orientation estimate; it does not provide absolute heading without an appropriate reference. Identity IMU mounting must be checked physically.

The firmware comment reports WHO_AM_I 0x70 and accepts several IDs. Retain the owner's MPU9250 module description, but verify exact silicon identity before relying on device-specific behavior.
