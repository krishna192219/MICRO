# From one range sensor to a reconstructed scan

The current scanner fixes the VL53L0X to the robot, approximately 60 mm forward. The robot turns left-forward/right-backward, updates wheel pose and reads ranges with `rangingTest()`. RangeStatus 0 or 2 is accepted experimentally. `SCAN_START` clears buffers and `SCAN_END` triggers the ROS output.

The combined ROS node builds two different representations:

1. **Point cloud in `odom`:** sensor position is `robot position + 0.060*[cos(heading),sin(heading)]`; the return adds `range*[cos(heading),sin(heading)]`. This accounts for reported robot position and the sensor offset for each sample.
2. **LaserScan in `laser`:** direct ranges go into 360 bins using heading relative to scan start. The nearest accepted range wins per bin. Empty bins are `inf`; accepted range is greater than 0.03 m and at most 2 m.

The approximately 100 points per turn at PWM 40 are an owner-reported prototype observation, not a fixed sampling guarantee. One-degree bins do not create additional measurements.

## Current limitations visible in source

- For 360 bins at one-degree spacing starting at -π, the last center is π−π/180. The code sets `angle_max=π`; metadata is inconsistent by one increment.
- LaserScan uses scan-start-relative bearings but a moving `laser` frame and an end-of-scan timestamp. It does not reproject the assembled ranges into one chosen frame/origin. A static mounting transform alone cannot remove that mismatch.
- `scan_time` and `time_increment` are zero in the marker-based nodes; there are no per-sample acquisition timestamps.
- LaserScan and cloud publish once at scan end with default volatile QoS; subscribers started afterward may miss the result.
- The nominal 50 ms schedule can be delayed by blocking range reads, serial output and other sensor work.
- A timeout still emits `SCAN_END` and a completion label; that marker does not prove a complete revolution.

The cloud and scan can therefore look different. A recognizable RViz boundary is a useful prototype result, but SLAM needs stronger geometric and temporal consistency.

The [official LaserScan definition](https://github.com/ros2/common_interfaces/blob/rolling/sensor_msgs/msg/LaserScan.msg) describes scan angles in the message frame and timestamps the first acquired ray. Use that contract when revising reconstruction. Original runtime source remains unchanged here so historical behavior is reproducible.

## Next test

Capture timestamped raw samples in a static scene; compare projected cloud and scan against measured walls/corners; quantify scan duration and repeated-turn consistency. Resolve reference origin, endpoint indexing and timing before claiming `slam_toolbox` suitability.
