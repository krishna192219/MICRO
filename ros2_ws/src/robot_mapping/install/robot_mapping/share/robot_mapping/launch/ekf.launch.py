"""Experimental wheel/gyro EKF bring-up; use instead of amr_sensor.launch.py."""
from pathlib import Path

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    share = Path(get_package_share_directory('robot_mapping'))
    return LaunchDescription([
        DeclareLaunchArgument('port', default_value='/dev/ttyUSB0'),
        DeclareLaunchArgument('rviz', default_value='true'),
        Node(package='robot_mapping', executable='ekf_serial_bridge',
             name='ekf_serial_bridge', output='screen',
             parameters=[str(share / 'config' / 'ekf_bridge.yaml'),
                         {'port': LaunchConfiguration('port')}]),
        Node(package='robot_localization', executable='ekf_node',
             name='ekf_filter_node', output='screen',
             parameters=[str(share / 'config' / 'ekf.yaml')]),
        Node(package='rviz2', executable='rviz2', name='rviz2',
             condition=IfCondition(LaunchConfiguration('rviz')),
             arguments=['-d', str(share / 'rviz' / 'ekf.rviz')]),
    ])
