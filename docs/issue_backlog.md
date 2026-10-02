# Ready-to-file engineering issues

These are repository issue drafts, not claims that GitHub issues have already been opened. Use the included issue templates to track reproduction and measured fixes.

## Correct LaserScan reference geometry and timing

Current ranges use scan-start-relative heading but are published in the moving laser frame at end-of-scan time.360 bins also conflict with an inclusive +π endpoint. Reproject to an explicitly chosen frame/origin and define acquisition timestamps. Acceptance: endpoint consistency checks plus comparison to a measured static scene and matching point cloud.

## Restore reproducible EKF integration

The development account reports an EKF, but its configuration is absent. The combined reader currently supplies TF without an Odometry message and cannot disable that TF. Acceptance: restore/test raw odometry messages, justified covariances and config, one dynamic TF owner, and raw/fused trial logs.

## Verify one-metre completion with signed encoders

The historical one-metre sketch drives at fixed PWM and compares negative raw right-forward counts to a positive threshold. Acceptance: explicitly select physical-distance stopping semantics, test both encoder directions and failed-read behavior, and report repeated physical distance/lateral errors.

## Measure timing in the all-sensor straight controller

RPM conversion divides by nominal 100 ms even when sensor calls delay control. Acceptance: log measured dt, compare resulting RPM at fixed PWM, then repeat synchronization/straight tests before changing gains.

## Repair historical bus declaration and pin mapping

`closedloopmotorcontrol_microROS` uses undeclared `I2C_Left`. Acceptance: compile with the selected board/core, verify each bus/address physically and document the correction as a new revision without changing archived claims.

## Record reproduction environment and evidence

Pin board/core/library versions and match each future video/log to a Git revision. Restore any missing original RViz/EKF files if available. Acceptance: reproducible build/start procedure and retained raw results, not only screenshots.
