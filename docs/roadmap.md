# Roadmap

1. **Reproduce the current scan route.** Build on the ROS host, confirm one serial owner, start RViz before scan, capture topic/TF output and source revision.
2. **Measure motion.** Resolve historical pin/geometry differences, test one-metre stop logic and repeat distance/turn trials using independent physical references. Log actual dt and supply behavior.
3. **Harden acquisition.** Add initialization gating, explicit read validity, serial buffering and timestamps. Distinguish scan timeout from success.
4. **Validate scan reconstruction.** Correct angular endpoints, choose a reference pose/origin, resolve moving laser-frame effects and measure repeated scan consistency.
5. **Restore EKF.** Add verified odometry messages/covariances and the tested robot_localization configuration. Transfer `odom → base_link` ownership to the filter.
6. **Test slam_toolbox.** Preserve actual maps, configuration, bags and loop-closure evidence before claiming mapping success.
7. **Integrate Nav2.** Implement a real command bridge, signed wheel commands and stop behavior, then test localization and goal execution.

Success is demonstrated by reproducible results; a running node or attractive RViz view alone does not establish physical accuracy.
