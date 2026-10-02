# How the motion controllers evolved

Source paths below refer to [firmware/experiments](../firmware/experiments/) unless stated otherwise. PWM is an actuator duty command; RPM is feedback computed from count change and elapsed time. Most later sketches use 20 kHz/8-bit LEDC. The real speed at a duty value depends on supply, load and mechanics.

## 1. Characterize before tuning

`motor_test_50_90` runs PWM 50, 70 and 90 for five-second measurement windows, with a one-second settling delay. It accumulates wrap-corrected counts and reports average RPM plus read errors. Historical notes record approximately (-40.48, 37.17) RPM at PWM 50 and (-62.93, 56.11) RPM at PWM 70 using that experiment's sign convention. These are reported observations; no raw numeric log accompanies them.

`encoder_RAWTEST_MOTOR`, `left_right_FILTER_UNFILTER_MOTOR_RPM_TES` and `raw_vs_filter_data_encoder_fix_pwm` explore sample windows and raw-versus-filtered behavior. One version has alpha=1, which passes the latest magnitude through without smoothing. One motor was replaced during the hardware investigation.

## 2. Speed matching and target RPM are different tasks

`closedloopmotorcontrol_microROS` uses error `leftRPM-rightRPM` to trim a PWM 50 baseline. It declares PI gains 1.3/0.01, but references undeclared `I2C_Left`; this historical snapshot needs repair before compilation.

`CLOSED_LOOP_MOTOR_CONTROL_MICROS` has a target of 100 RPM, separate wheel PI terms (Kp=1, Ki=0.2, Kd=0), base PWMs 90/125 and alpha=0.25 filtering over an accumulated 0.2 s measurement window. Its signed RPM assumptions must be checked against the hardware revision.

`closed_loop_motor_400rpm` targets 50 RPM with feed-forward 63/63, Kp=1.5, PWM limit 120 and maximum PWM change 5. The actual control interval is 2000 ms. KI=0.5 is declared but not used: the executed expression is `FF + KP * error`, not a PI/PID update.

## 3. Straight motion with speed and position synchronization

In `ALL_SENSOR_COMBINED_MICROS::driveStraight()`:

```text
start each PWM at 63
RPM = abs(count increment) / 4096 * 60000 / CONTROL_PERIOD
error = 50 - measured RPM
sync = clamp(0.005 * (abs(leftCount)-abs(rightCount)), -15, +15)
left desired  = previous left PWM  + int(0.5*left error)  - int(sync)
right desired = previous right PWM + int(0.5*right error) + int(sync)
clamp each PWM into [0,90]; limit each update to ±5
```

This is an incremental correction around the previous command, initialized by feed-forward. It is not exactly the same equation as the fixed `FF+Kp*error` experiment. The 100 ms denominator is nominal; blocking ToF/serial work can delay updates and bias the RPM calculation. Measuring actual elapsed time is an open improvement.

Completion uses the average of absolute segment counts for a 0.50 m target, with an 8 s timeout. Segment reset keeps cumulative totals for odometry, but rebasing raw angles can discard motion between the preceding sample and reset. The controller demonstrates a feedback structure; no universal straightness bound is established.

## 4. Distance feedback without RPM regulation

`micro_ros_1meter_test` starts both wheels at fixed PWM 63 and stops on an encoder threshold. It does not continuously regulate wheel RPM. Its right forward counts are negative, yet the stop expression checks `leftCounts >= TARGET_COUNTS || rightCounts >= TARGET_COUNTS`; under that convention the left threshold governs completion. Preserve this experiment as a development step, and correct/test the stop policy before reuse. The physical tape-test video is qualitative evidence; its exact source revision is unknown.

## 5. Turning: staged slowdown plus count synchronization

`micro_ros_360_turn` targets 360° with 90 mm separation, takes the smaller absolute wheel count and selects PWM 63/45/30/20 at remaining-count bands >500/>200/>60/otherwise. It stops after both wheel targets or 5 s. Old serial messages still say 90°; the configured numeric target is 360°.

`micro_ros_square` uses 95 mm separation, 0.50 m sides and encoder-based PWM stages for both straight motion and 90° turns. It applies the same duty to both wheels and has no explicit wheel-mismatch synchronization term.

The later `ALL_SENSOR_COMBINED_MICROS::rotate90()` uses 105 mm separation and a 20 ms nominal controller:

```text
remaining = min(target-leftAbs, target-rightAbs)
base PWM = 63 if remaining > 500; 40 if > 200; otherwise 30
trim = clamp(0.06 * (leftAbs-rightAbs), -10, +10)
left PWM  = clamp(base-trim, 20, 80)
right PWM = clamp(base+trim, 20, 80)
```

It rotates left-forward/right-backward and stops after both targets, or a 5 s timeout. The faster wheel is reduced and slower wheel increased, but an individual wheel is not separately stopped when it reaches target. Overshoot and rotation-center drift require measurement.

## 6. Modular scanner rotation

`firmware/integrated/MIC_ROS_AMR/MIC_ROS_AMR.ino::rotate360AndScan()` runs one full-turn task after startup. It uses absolute movement from starting cumulative counts, Kp_sync=0.06, trim limit 10, base PWM 40, PWM limits 20–50 and nominal synchronization every 50 ms. Both wheels must reach `(0.105/0.044)*4096` counts, approximately 9775 counts. Timeout is 15 s.

This routine synchronizes encoder travel; it does not run the straight-line RPM controller, even though `Config.h` still contains straight-motion constants. `TURN_SLOW_PWM=45` is present but unused by this routine. Reading sensors continues during rotation.

## 7. Differential-drive velocity experiment

`differential_drive_closed_loop_motor_micROS` computes `vL=v−ωb/2`, `vR=v+ωb/2` and converts each to RPM using wheel circumference. The constants are v=0.230 m/s, ω=0, b=0.10 m. It uses wheel PI terms, but forward-only clamped motor helpers limit signed/reverse commands. There is no ROS command subscription or serial velocity parser.

## What to measure next

Repeat distance and turns using an independent physical reference; record supply voltage, surface, load, wheel geometry, actual sample intervals and final error. Keep speed tracking, path straightness and stop accuracy as separate results. Apparent motion accuracy and estimated RViz pose do not prove the same thing.
