# Development progression

This is a logical reconstruction from the supplied source and the owner's account. Folder numbering is a learning order, not an invented commit chronology.

| Stage | Engineering progression |
| --- | --- |
| Basic bring-up | Manual PWM and Bluetooth motor control |
| Encoder-free scanning | Stepper-driven ToF sweep with button control and angle/range serial output |
| Wheel feedback | AS5600 wrap handling, direction checks, fixed-PWM characterization and noise experiments |
| Speed control | Wheel matching, target RPM, feed-forward/P and separate PI experiments |
| Motion tasks | Encoder-stopped one-metre drive, decelerating 360° turn and 0.50 m square |
| Differential drive | Linear/angular velocity constants converted to wheel targets |
| IMU | Register reads, stored gyro-bias correction and Madgwick host pipeline |
| ROS visualization | Wheel odometry, point clouds, LaserScan and TF debugging |
| Integration | All-sensor square sketch and modular one-revolution scanner represent different integration branches |
| Next | Reproducible EKF, geometrically consistent scan, SLAM and navigation |

The supplied TF capture filenames span August–October 2026, but they do not establish dates for every development stage. Two captures contain no TF data; they are preserved as unsuccessful diagnostics. Exact media-to-code revisions were not recorded.

The [milestones](../milestones/README.md) and [source catalog](source_catalog.md) connect these stages to the actual files.
