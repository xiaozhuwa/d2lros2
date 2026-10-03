// 包含ROS2客户端库头文件
#include "rclcpp/rclcpp.hpp"

// 创建一个简单的ROS2节点类，继承自rclcpp::Node
class SimpleNode : public rclcpp::Node {
public:
  SimpleNode() : Node("simple_node") {
    // 构造函数中打印一条日志消息
    RCLCPP_INFO(this->get_logger(), "你好，ROS2！我是一个简单的节点。");
  }
};

int main(int argc, char **argv) {
  // 初始化ROS2
  rclcpp::init(argc, argv);
  // 创建节点实例
  auto node = std::make_shared<SimpleNode>();
  // 保持节点运行，等待关闭
  rclcpp::spin(node);
  // 关闭ROS2
  rclcpp::shutdown();
  return 0;
}
