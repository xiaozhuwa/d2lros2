# 导入rclpy库
import rclpy
from rclpy.node import Node

class Node02(Node):
    def __init__(self, name):
        super().__init__(name)
        self.get_logger().info("你好，ROS2！我是Python节点！")

def main():
    rclpy.init()
    node = Node02("node_02")
    rclpy.spin(node)
    rclpy.shutdown()