from setuptools import find_packages, setup
from glob import glob

package_name = 'robot_mapping'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        ('share/' + package_name + '/launch', glob('launch/*.launch.py')),
        ('share/' + package_name + '/rviz', glob('rviz/*.rviz')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='krishna',
    maintainer_email='krishna@example.com',
    description='ESP32 serial sensing, wheel odometry and rotating ToF scan experiments',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
    'console_scripts': [
        'serial_scan_node = robot_mapping.serial_scan_node:main',
        'odom_scan = robot_mapping.odom_scan:main',
        'odom_serial = robot_mapping.odom_serial:main',
        'imu_serial = robot_mapping.imu_serial:main',
        'scan_serial_node = robot_mapping.scan_serial_node:main',
        'scan_imu_odom = robot_mapping.scan_imu_odom:main',
        ],
    },
)
