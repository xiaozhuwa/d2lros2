#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using std::placeholders::_1;

class TopicSubscribe01 : public rclcpp::Node
{
public:
    TopicSubscribe01(std::string name) : Node(name)
    {
        RCLCPP_INFO(this->get_logger(), "大家好，我是%s.", name.c_str());
        // 创建订阅者，订阅 command 话题
        command_subscribe_ = this->create_subscription<std_msgs::msg::String>(
            "command", 10, std::bind(&TopicSubscribe01::command_callback, this, _1));
    }

private:
    void command_callback(const std_msgs::msg::String::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "收到命令: '%s'", msg->data.c_str());
    }
    // 声明话题订阅者指针
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr command_subscribe_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TopicSubscribe01>("topic_subscribe_01");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
