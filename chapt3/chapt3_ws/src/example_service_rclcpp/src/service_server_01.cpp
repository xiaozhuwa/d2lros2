#include "rclcpp/rclcpp.hpp"
#include "my_interfaces/srv/add_two_ints.hpp"

using AddTwoInts = my_interfaces::srv::AddTwoInts;

class ServiceServer01 : public rclcpp::Node {
public:
  ServiceServer01(std::string name) : Node(name) {
    RCLCPP_INFO(this->get_logger(), "节点已启动：%s.", name.c_str());
    // 创建服务端，服务名 "add_two_ints"，回调 handle_add
    service_ = this->create_service<AddTwoInts>(
        "add_two_ints",
        std::bind(&ServiceServer01::handle_add, this,
                  std::placeholders::_1, std::placeholders::_2));
  }

private:
  void handle_add(const std::shared_ptr<AddTwoInts::Request> request,
                  std::shared_ptr<AddTwoInts::Response> response) {
    response->sum = request->a + request->b;
    RCLCPP_INFO(this->get_logger(), "收到请求: a=%ld, b=%ld, 返回 sum=%ld",
                request->a, request->b, response->sum);
  }
  rclcpp::Service<AddTwoInts>::SharedPtr service_;
};

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<ServiceServer01>("service_server_01");
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
