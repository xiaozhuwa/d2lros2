#include "rclcpp/rclcpp.hpp"
#include "example_ros2_interfaces/msg/robot_status.hpp"
#include "example_ros2_interfaces/srv/move_robot.hpp"

/*
 * 控制节点：调用机器人的移动服务，同时订阅机器人的实时状态
 */
class ExampleInterfacesControl : public rclcpp::Node {
private:
    rclcpp::Client<example_ros2_interfaces::srv::MoveRobot>::SharedPtr client_;
    rclcpp::Subscription<example_ros2_interfaces::msg::RobotStatus>::SharedPtr robot_status_subscribe_;

public:
    ExampleInterfacesControl(std::string name) : Node(name) {
        RCLCPP_INFO(this->get_logger(), "%s节点已经启动.", name.c_str());
        // ① 创建客户端
        client_ = this->create_client<example_ros2_interfaces::srv::MoveRobot>("move_robot");
        // ② 创建订阅者
        robot_status_subscribe_ = this->create_subscription<example_ros2_interfaces::msg::RobotStatus>(
            "robot_status", 10, std::bind(&ExampleInterfacesControl::robot_status_callback_, this, std::placeholders::_1));
    }

    // 发送移动请求
    void move_robot(float distance) {
        RCLCPP_INFO(this->get_logger(), "请求让机器人移动%f", distance);
        // 等待服务端上线
        while (!client_->wait_for_service(std::chrono::seconds(1))) {
            if (!rclcpp::ok()) {
                RCLCPP_ERROR(this->get_logger(), "等待服务的过程中出现问题.");
                return;
            }
            RCLCPP_INFO(this->get_logger(), "等待服务端上线中");
        }
        // 构造请求并发送（异步，结果回来时自动调用 result_callback_）
        auto request = std::make_shared<example_ros2_interfaces::srv::MoveRobot::Request>();
        request->distance = distance;
        client_->async_send_request(request, std::bind(&ExampleInterfacesControl::result_callback_, this, std::placeholders::_1));
    }

private:
    // 服务结果回调
    void result_callback_(rclcpp::Client<example_ros2_interfaces::srv::MoveRobot>::SharedFuture result_future) {
        auto response = result_future.get();
        RCLCPP_INFO(this->get_logger(), "收到移动结果：%f", response->pose);
    }

    // 机器人状态回调
    void robot_status_callback_(const example_ros2_interfaces::msg::RobotStatus::SharedPtr msg) {
        RCLCPP_INFO(this->get_logger(), "收到状态数据，位置：%f 状态：%d", msg->pose, msg->status);
    }
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ExampleInterfacesControl>("example_interfaces_control_01");
    node->move_robot(5.0);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}