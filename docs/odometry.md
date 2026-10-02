# Wheel odometry

The modular ESP32 firmware uses diameter 0.044 m, separation 0.105 m and 4096 counts/revolution. For raw signed increments nL and nR:

```text
k = π*0.044/4096 metres/count
sL = k*nL
sR = -k*nR          # right forward is negative in raw counts
s = (sL+sR)/2
dtheta = (sR-sL)/0.105
theta_mid = theta+dtheta/2
x += s*cos(theta_mid)
y += s*sin(theta_mid)
theta = wrap(theta+dtheta) into [-π,+π]
```

Raw signs are retained in `Encoders.cpp`. A stale comment says the right sign is inverted there, but the actual expression is `delta=delta`; the effective correction occurs in `Odometry.cpp`.

`POSE` transmits the resulting state. The combined ROS node turns it into TF, not a nav_msgs/Odometry message. `odom_serial` instead integrates ENC counts with a historical 0.095 m separation; `odom_scan` publishes pose from SCAN records. Neither standalone odometry publisher supplies a measured velocity estimate/covariance model suitable for blindly configuring EKF inputs.

The square RViz video documents an estimated trajectory. Wheel odometry cannot independently prove physical square closure or distance accuracy. Encoder counts can be precise while slip, wheel geometry, backlash and surface contact still introduce error.
