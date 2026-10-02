# Troubleshooting: actual problems and responses

## Faulty motor and curved motion

**Observed:** one motor was problematic/stuck; mismatched wheel speeds caused curved motion.

**Response:** replace the defective motor, match the pair more closely, characterize encoder RPM, and add feed-forward/P correction and wheel synchronization.

**Result:** replacement and control changes implemented. No quantified straight-line deviation is supplied. Revisit mechanical behavior before treating every mismatch as a gain problem.

## Encoder sign mismatch

**Observed:** physical forward produced opposite raw encoder signs.

**Response:** test each wheel physically; establish left-positive and right-negative raw forward; compensate the right direction in firmware.

**Result:** convention determined. Check serial count semantics before applying ROS correction to avoid reversing twice.

## AS5600 fluctuations

**Observed:** apparent 15–30 RPM changes despite visually smooth running; magnetic alignment was questioned.

**Response:** investigate centering, filtering/alpha, motor replacement, fixed PWM and mechanical-versus-measured behavior.

**Result:** favourable approximately 1 RPM behavior in some conditions, but no universal fix established. Preserve this as an open characterization issue.

## Sluggish low PWM

**Observed:** slow/laggy startup before speeding up at very low PWM.

**Response:** operate in a practical region instead of relying on extremely low PWM.

**Result:** prototype workaround; stall threshold has not been quantified.

## ROS Python import error

Incorrect:

```python
from sensor_msgs_py import point_cloud2, LaserScan
```

Correct:

```python
from sensor_msgs.msg import PointCloud2, LaserScan
from sensor_msgs_py import point_cloud2
```

Message classes and point-cloud helper functions come from different modules.

## Scan variable scope

**Observed:** a scan-processing variable was referenced outside its valid conditional scope.

**Response:** move scan-point calculations into scanning-state logic so the value exists when used.

**Result:** corrected. Check start/end and non-scan record paths when adding the actual source.

## `/scan` exists but RViz shows nothing

First inspect data and TF independently:

```bash
ros2 topic echo /scan --once
ros2 run tf2_ros tf2_echo base_link laser
```

The actual failure had finite ranges around 0.19–0.20 m but no `base_link → laser`. The following temporary diagnostic made the scan visible:

```bash
ros2 run tf2_ros static_transform_publisher 0.06 0 0 0 0 0 base_link laser
```

The permanent fix was a `StaticTransformBroadcaster` inside the scan ROS node. Stop the temporary publisher once the permanent broadcaster is active. Inspect RViz fixed frame and timestamp reachability if a similar symptom returns.

## Rotation/heading signs

**Observed:** rotation behavior needed careful sign checking.

**Response:** test wheel directions, encoder signs and heading through an approximately 360° in-place scan.

**Result:** heading changed correctly while X/Y stayed approximately stable. No drift metric is available. Do not infer scan geometry accuracy from stable center pose alone.

## Packaging findings after source review

The supplied launch referred to missing package `amr_odometry` and would start multiple readers of the same port. The packaged launch now selects the existing `robot_mapping/scan_imu_odom` reader, with Madgwick and an optional RViz view. Original launch source is retained in `archive/original_ros_packaging/`.

The detailed [source review](code_review.md) distinguishes these newly found issues from the historical failures above. Runtime control algorithms are preserved; the packaging changes have not been retested on hardware.
