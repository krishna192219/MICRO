"""Single-reader scan/IMU bring-up for the modular MIC_ROS_AMR firmware.

Repository packaging revision; requires hardware validation on the ROS host.
"""
from pathlib import Path

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    rviz_config = str(Path(get_package_share_directory('robot_mapping')) /
                      'rviz' / 'sensors.rviz')
    return LaunchDescription([
        DeclareLaunchArgument('rviz', default_value='true'),
        DeclareLaunchArgument('madgwick', default_value='true'),
        Node(package='robot_mapping', executable='scan_imu_odom',
             name='combined_serial', output='screen'),
        Node(package='imu_filter_madgwick', executable='imu_filter_madgwick_node',
             name='imu_filter', output='screen',
             condition=IfCondition(LaunchConfiguration('madgwick')),
             parameters=[{'use_mag': False, 'world_frame': 'enu',
                          'publish_tf': False}],
             remappings=[('imu/data_raw', '/imu/data_raw'),
                         ('imu/data', '/imu/data')]),
        Node(package='rviz2', executable='rviz2', name='rviz2',
             arguments=['-d', rviz_config],
             condition=IfCondition(LaunchConfiguration('rviz'))),
    ])
