cmake_minimum_required(VERSION 3.8)
project(example_ros2_interfaces)

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# 生成 msg 和 srv 接口代码
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/RobotStatus.msg"
  "srv/MoveRobot.srv"
)

ament_package()