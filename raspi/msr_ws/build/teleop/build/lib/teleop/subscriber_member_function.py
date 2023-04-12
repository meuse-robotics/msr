import rclpy
from rclpy.node import Node

from std_msgs.msg import Int16MultiArray

class MinimalSubscriber(Node):
    nodeID = 1
    def __init__(self):
        super().__init__('minimal_subscriber')
        self.subscription = self.create_subscription(
            Int16MultiArray, # receive array of int16
            '/topic_pc2rp', # topic from PC to Raspi
            self.listener_callback, # called when receive
            10)
        self.subscription  # prevent unused variable warning
        self.publisher = self.create_publisher( # publish received data to spresense
            Int16MultiArray, # receive array of int16
            '/topic_rp2sp', # topic from Raspi to Spresense
            10)

    def listener_callback(self, msg):
        if msg.data[0] == self.nodeID:
            self.publisher.publish(msg)
            self.get_logger().info(
            'I heard: %d , %d , %d' %
            (msg.data[0], msg.data[1], msg.data[2]))

def main(args=None):
    rclpy.init(args=args)

    minimal_subscriber = MinimalSubscriber()

    rclpy.spin(minimal_subscriber)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    minimal_subscriber.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
