from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():

    return LaunchDescription([

        # --------------------------------------------------
        # Wheel Encoder Odometry
        # Publishes:
        #   /odom
        # TF:
        #   odom -> base_link
        # --------------------------------------------------
        Node(
            package='amr_odometry',
            executable='odom_serial',
            name='wheel_odometry',
            output='screen',
        ),

        # --------------------------------------------------
        # IMU Serial Driver
        # Publishes:
        #   /imu/data_raw
        # Frame:
        #   imu_link
        # --------------------------------------------------
        Node(
            package='amr_odometry',
            executable='imu_serial',
            name='imu_serial',
            output='screen',
        ),

        # --------------------------------------------------
        # Madgwick IMU Orientation Filter
        #
        # Input:
        #   /imu/data_raw
        #
        # Output:
        #   /imu/data
        #
        # We are NOT using the magnetometer yet.
        # Therefore yaw will be relative and can drift.
        # --------------------------------------------------
        Node(
            package='imu_filter_madgwick',
            executable='imu_filter_madgwick_node',
            name='imu_filter',
            output='screen',
            parameters=[
                {
                    'use_mag': False,
                    'world_frame': 'enu',
                    'publish_tf': False,
                }
            ],
            remappings=[
                ('imu/data_raw', '/imu/data_raw'),
                ('imu/data', '/imu/data'),
            ],
        ),

        # --------------------------------------------------
        # Static transform:
        #
        # base_link -> imu_link
        #
        # Identity transform for now.
        # We will replace this with the actual physical
        # mounting position/orientation later.
        # --------------------------------------------------
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            name='imu_static_tf',
            arguments=[
                '0', '0', '0',
                '0', '0', '0',
                'base_link',
                'imu_link',
            ],
            output='screen',
        ),

    ])   