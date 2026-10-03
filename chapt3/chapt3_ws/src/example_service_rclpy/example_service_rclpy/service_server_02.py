import rclpy
from rclpy.node import Node
from example_interfaces.srv import AddTwoInts


class ServiceServer02(Node):
    def __init__(self, name):
        super().__init__(name)
        self.get_logger().info("大家好，我是%s!" % name)
        # 创建服务端，服务名 "add_two_ints"，回调 handle_add
        self.srv = self.create_service(AddTwoInts, "add_two_ints", self.handle_add)

    def handle_add(self, request, response):
        response.sum = request.a + request.b
        self.get_logger().info("收到请求: a=%d, b=%d, 返回 sum=%d" % (request.a, request.b, response.sum))
        return response


def main(args=None):
    rclpy.init(args=args)
    node = ServiceServer02("service_server_02")
    rclpy.spin(node)
    rclpy.shutdown()
