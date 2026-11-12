"""One serial owner for wheel/IMU EKF inputs; never broadcasts dynamic TF.

Consumes the original modular firmware's POSE and IMU records. Scan records
are ignored in this initial state-estimation mode. No motor commands are sent.
"""
import math
import time

import serial
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import TransformStamped
from nav_msgs.msg import Odometry
from sensor_msgs.msg import Imu
from tf2_ros.static_transform_broadcaster import StaticTransformBroadcaster

from robot_mapping.ekf_inputs import LineBuffer, PoseVelocity, parse_record


class EKFSerialBridge(Node):
    def __init__(self):
        super().__init__('ekf_serial_bridge')
        defaults = {
            'port': '/dev/ttyUSB0', 'baud': 115200,
            'wheel_vx_variance': 0.0025,
            'wheel_vy_variance': 0.0025,
            'wheel_wz_variance': 0.04,
            'gyro_variance': 0.01,
            'accel_variance': 0.1,
            'min_pose_dt': 0.03, 'max_pose_dt': 0.5,
            'max_linear_speed': 0.6, 'max_yaw_rate': 6.0,
            'imu_roll': 0.0, 'imu_pitch': 0.0, 'imu_yaw': 0.0,
        }
        for name, value in defaults.items():
            self.declare_parameter(name, value)
        self.settings = {name: self.get_parameter(name).value for name in defaults}
        for key, value in self.settings.items():
            if isinstance(value, (int, float)) and not math.isfinite(value):
                raise ValueError(f'{key} must be finite')
            if (key.endswith('variance') or key.startswith('max_') or
                    key == 'min_pose_dt' or key == 'baud') and value <= 0:
                raise ValueError(f'{key} must be positive')
        if self.settings['min_pose_dt'] >= self.settings['max_pose_dt']:
            raise ValueError('min_pose_dt must be less than max_pose_dt')
        self.motion = PoseVelocity(
            self.settings['min_pose_dt'], self.settings['max_pose_dt'],
            self.settings['max_linear_speed'], self.settings['max_yaw_rate'])
        self.lines = LineBuffer()
        self.odom_pub = self.create_publisher(Odometry, '/odom', 10)
        self.imu_pub = self.create_publisher(Imu, '/imu/data_raw', 10)
        self.static_tf = StaticTransformBroadcaster(self)
        self.publish_mount()
        self.ser = serial.Serial(self.settings['port'], self.settings['baud'],
                                 timeout=0, exclusive=True)
        self.faulted = False
        self.started = False
        self.last_seen = {}
        self.last_warning = {}
        self.start_time = time.monotonic()
        self.timer = self.create_timer(0.005, self.poll)
        self.health_timer = self.create_timer(1.0, self.check_health)
        self.get_logger().info('EKF serial inputs ready; only EKF may publish odom -> base_link')

    def warn(self, key, message):
        now = time.monotonic()
        if now - self.last_warning.get(key, -1e9) > 5.0:
            self.get_logger().warning(message)
            self.last_warning[key] = now

    def publish_mount(self):
        roll = self.settings['imu_roll'] / 2
        pitch = self.settings['imu_pitch'] / 2
        yaw = self.settings['imu_yaw'] / 2
        cr, sr = math.cos(roll), math.sin(roll)
        cp, sp = math.cos(pitch), math.sin(pitch)
        cy, sy = math.cos(yaw), math.sin(yaw)
        tf = TransformStamped()
        tf.header.stamp = self.get_clock().now().to_msg()
        tf.header.frame_id = 'base_link'
        tf.child_frame_id = 'imu_link'
        tf.transform.rotation.x = sr*cp*cy - cr*sp*sy
        tf.transform.rotation.y = cr*sp*cy + sr*cp*sy
        tf.transform.rotation.z = cr*cp*sy - sr*sp*cy
        tf.transform.rotation.w = cr*cp*cy + sr*sp*sy
        self.static_tf.sendTransform(tf)

    def poll(self):
        if self.faulted:
            return
        try:
            waiting = self.ser.in_waiting
            if waiting > 4096:
                self.warn('backlog', 'Serial backlog: host-time velocity may be unreliable; '
                          'reduce firmware debug output or add acquisition timestamps.')
            # Bound callback work, keep complete-line buffering and publish only the
            # newest POSE/IMU in this read. Old samples have no acquisition times.
            data = self.ser.read(min(waiting, 16384)) if waiting else b''
            latest = {}
            for line in self.lines.feed(data):
                if 'AMR CONTROLLER' in line or 'SYSTEM READY' in line:
                    if self.started:
                        self.faulted = True
                        self.get_logger().error('ESP32 restarted during estimation. '
                                                'Restart the entire EKF launch.')
                        return
                    continue
                record = parse_record(line)
                if record:
                    latest[record[0]] = record[1]
            now = time.monotonic()
            stamp = self.get_clock().now().to_msg()
            for tag, values in latest.items():
                self.started = True
                self.last_seen[tag] = now
                if tag == 'POSE':
                    motion = self.motion.update(values, now)
                    if motion:
                        self.publish_odom(motion, stamp)
                    elif self.motion.reason not in ('first sample', 'accumulating'):
                        self.warn('pose', 'Wheel sample skipped: ' + self.motion.reason)
                else:
                    self.publish_imu(values, stamp)
        except (serial.SerialException, OSError) as exc:
            self.faulted = True
            self.get_logger().error(f'Serial input stopped: {exc}. Restart the EKF launch.')

    def publish_odom(self, motion, stamp):
        msg = Odometry()
        msg.header.stamp = stamp
        msg.header.frame_id = 'odom'
        msg.child_frame_id = 'base_link'
        msg.pose.pose.position.x = motion.x
        msg.pose.pose.position.y = motion.y
        msg.pose.pose.orientation.z = math.sin(motion.yaw / 2)
        msg.pose.pose.orientation.w = math.cos(motion.yaw / 2)
        # Pose is for comparison, NOT fused. Large covariance reflects that fact.
        for i in range(6):
            msg.pose.covariance[7*i] = 1e6
            msg.twist.covariance[7*i] = 1e6
        msg.twist.twist.linear.x = motion.vx
        msg.twist.twist.linear.y = 0.0  # nonholonomic assumption, not a measurement
        msg.twist.twist.angular.z = motion.yaw_rate
        msg.twist.covariance[0] = self.settings['wheel_vx_variance']
        msg.twist.covariance[7] = self.settings['wheel_vy_variance']
        msg.twist.covariance[35] = self.settings['wheel_wz_variance']
        self.odom_pub.publish(msg)

    def publish_imu(self, values, stamp):
        msg = Imu()
        msg.header.stamp = stamp
        msg.header.frame_id = 'imu_link'
        ax, ay, az, gx, gy, gz = values
        msg.linear_acceleration.x = ax * 9.80665
        msg.linear_acceleration.y = ay * 9.80665
        msg.linear_acceleration.z = az * 9.80665
        msg.angular_velocity.x = math.radians(gx)
        msg.angular_velocity.y = math.radians(gy)
        msg.angular_velocity.z = math.radians(gz)
        msg.orientation_covariance[0] = -1.0
        for i in (0, 4, 8):
            msg.angular_velocity_covariance[i] = self.settings['gyro_variance']
            msg.linear_acceleration_covariance[i] = self.settings['accel_variance']
        self.imu_pub.publish(msg)

    def check_health(self):
        now = time.monotonic()
        for tag in ('POSE', 'IMU'):
            if now - self.last_seen.get(tag, self.start_time) > 1.0:
                self.warn(tag, f'No recent {tag}: filtered output alone does not '
                          'prove healthy measurements.')

    def destroy_node(self):
        if hasattr(self, 'ser') and self.ser.is_open:
            self.ser.close()
        super().destroy_node()


def main(args=None):
    rclpy.init(args=args)
    node = None
    try:
        node = EKFSerialBridge()
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        if node is not None:
            node.destroy_node()
        rclpy.shutdown()
