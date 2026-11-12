import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/krishna/mic_ros_amr/ros2_ws/src/robot_mapping/install/robot_mapping'
