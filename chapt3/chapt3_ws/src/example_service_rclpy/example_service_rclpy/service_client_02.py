import rclpy
from rclpy.node import Node
from example_interfaces.srv import AddTwoInts

class ServiceClient02(Node):
    def __init__(self, name):
        super().__init__(name)
        self.get_logger().info("大家好，我是%s!" % name)
        # 创建客户端，连接 "add_two_ints" 服务
        self.client = self.create_client(AddTwoInts, "add_two_ints")

    def send_request(self, a, b):
        # 等待服务端上线，最多等 3 秒
        while not self.client.wait_for_service(timeout_sec=3.0):
            self.get_logger().info("等待服务端上线...")

        request = AddTwoInts.Request()
        request.a = a
        request.b = b

        # 异步发送请求
        future = self.client.call_async(request)
        # 等待响应
        rclpy.spin_until_future_complete(self, future)

        if future.result() is not None:
            self.get_logger().info("收到响应: %d + %d = %d" % (a, b, future.result().sum))
        else:
            self.get_logger().error("请求失败")

def main(args=None):
    rclpy.init(args=args)
    node = ServiceClient02("service_client_02")
    node.send_request(3, 5)
    rclpy.shutdown()
