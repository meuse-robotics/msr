# pyqt GUI
import sys
from PyQt5.QtWidgets import QMainWindow, QApplication, QCheckBox
from PyQt5.QtCore import Qt, QTimer
from controller import Ui_MainWindow

# ros
import rclpy
from rclpy.node import Node
from std_msgs.msg import Int16MultiArray

class MinimalPublisher(QMainWindow):
    def __init__(self, parent=None):
        # pyqt GUI
        super(MinimalPublisher, self).__init__(parent)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        # ros
        rclpy.init()
        self.node = Node('talker')
        self.pub = self.node.create_publisher(Int16MultiArray, '/topic_pc2rp', 10)
        
    def servoControl(self, x):
    	for i in range(1, 10):
            checkbox = self.findChild(QCheckBox, f"checkBox_{i}")
            if checkbox.isChecked(): # publish only to selected robots
                array = []
                array.append(i) # robot number
                array.append(self.sender().property("sliderNum")) # joint number
                array.append(x) # angle data
                msg = Int16MultiArray(data=array)
                self.pub.publish(msg) # publish topic
        
if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MinimalPublisher()
    window.show()
    sys.exit(app.exec_())
