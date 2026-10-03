#include "rclcpp/rclcpp.hpp"
#include "my_interfaces/srv/add_two_ints.hpp"

using AddTwoInts = my_interfaces::srv::AddTwoInts;

class ServiceClient01 : public rclcpp::Node {
public:
  ServiceClient01(std::string name) : Node(name) {
    RCLCPP_INFO(this->get_logger(), "节点已启动：%s.", name.c_str());
    client_ = this->create_client<AddTwoInts>("add_two_ints");
  }

  void send_request(int64_t a, int64_t b) {
    while (!client_->wait_for_service(std::chrono::seconds(3))) {
      if (!rclcpp::ok()) {
        RCLCPP_ERROR(this->get_logger(), "等待服务时被中断");
        return;
      }
      RCLCPP_INFO(this->get_logger(), "等待服务端上线...");
    }

    auto request = std::make_shared<AddTwoInts::Request>();
    request->a = a;
    request->b = b;

    auto future = client_->async_send_request(request);

    if (rclcpp::spin_until_future_complete(
            this->get_node_base_interface(), future) ==
        rclcpp::FutureReturnCode::SUCCESS) {
      RCLCPP_INFO(this->get_logger(), "收到响应: %ld + %ld = %ld",
                  a, b, future.get()->sum);
    } else {
      RCLCPP_ERROR(this->get_logger(), "请求失败");
    }
  }

private:
  rclcpp::Client<AddTwoInts>::SharedPtr client_;
};

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<ServiceClient01>("service_client_01");
  node->send_request(3, 5);
  rclcpp::shutdown();
  return 0;
}
