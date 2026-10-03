#include "rclcpp/rclcpp.hpp"
#include "example_ros2_interfaces/msg/robot_status.hpp"
#include "example_ros2_interfaces/srv/move_robot.hpp"
#include <thread>

/*
 * 机器人节点：提供服务（被调用移动），同时定时发布自己的状态
 */
class ExampleInterfacesRobot : public rclcpp::Node {
private:
    /* 声明一个机器人类（纯C++类，模拟真实机器人硬件，与ROS2解耦） */
    class Robot {
    public:
        // 移动机器人：每步0.01，循环移动直到走完distance
        float move_distance(float distance) {
            status_moveing_ = true;
            while (fabs(distance) > 0.01f) {
                float step = distance / fabs(distance) * 0.01f; // 保持方向
                current_pose_ += step;
                distance -= step;
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
            status_moveing_ = false;
            return current_pose_;
        }
        float get_current_pose() { return current_pose_; }
        int get_status() {
            return status_moveing_ ? example_ros2_interfaces::msg::RobotStatus::STATUS_MOVEING
                                   : example_ros2_interfaces::msg::RobotStatus::STATUS_STOP;
        }
        void set_target(float target) { target_pose_ = target; }

    private:
        float current_pose_ = 0.0f;
        float target_pose_ = 0.0f;
        bool status_moveing_ = false;
    };

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<example_ros2_interfaces::msg::RobotStatus>::SharedPtr robot_status_publisher_;
    rclcpp::Service<example_ros2_interfaces::srv::MoveRobot>::SharedPtr move_robot_server_;
    Robot robot;

public:
    ExampleInterfacesRobot(std::string name) : Node(name) {
        RCLCPP_INFO(this->get_logger(), "%s节点已经启动.", name.c_str());
        // ① 创建状态话题发布者
        robot_status_publisher_ = this->create_publisher<example_ros2_interfaces::msg::RobotStatus>("robot_status", 10);
        // ② 创建定时器，500ms周期发布一次状态
        timer_ = this->create_wall_timer(std::chrono::milliseconds(500), std::bind(&ExampleInterfacesRobot::timer_callback, this));
        // ③ 创建移动服务
        move_robot_server_ = this->create_service<example_ros2_interfaces::srv::MoveRobot>(
            "move_robot", std::bind(&ExampleInterfacesRobot::handle_move_robot, this,
                                    std::placeholders::_1, std::placeholders::_2));
    }

private:
    // 定时回调：发布当前位置和状态
    void timer_callback() {
        example_ros2_interfaces::msg::RobotStatus message;
        message.status = robot.get_status();
        message.pose = robot.get_current_pose();
        RCLCPP_INFO(this->get_logger(), "Publishing: %f", message.pose);
        robot_status_publisher_->publish(message);
    }

    // 服务回调：收到移动请求，让机器人移动并返回最终位置
    void handle_move_robot(const std::shared_ptr<example_ros2_interfaces::srv::MoveRobot::Request> request,
                           std::shared_ptr<example_ros2_interfaces::srv::MoveRobot::Response> response) {
        RCLCPP_INFO(this->get_logger(), "收到请求信息，目标位置%f", request->distance);
        robot.set_target(request->distance);
        response->pose = robot.move_distance(request->distance - robot.get_current_pose());
    }
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ExampleInterfacesRobot>("example_interfaces_robot_01");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}