#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/int16.h>
#include <std_msgs/msg/int16_multi_array.h>

static rcl_allocator_t allocator;
static rclc_support_t support;
static rcl_node_t node;
static rcl_subscription_t subscriber;
static rclc_executor_t executor;
static rcl_timer_t timer;
static std_msgs__msg__Int16 msg;

static std_msgs__msg__Int16MultiArray msgin;


#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

//int Arr[90];

void error_loop(){
  while(1){
    digitalWrite(LED0, !digitalRead(LED0));
    delay(100);
  }
}

void subscription_callback(const void * msgin)
{
  const std_msgs__msg__Int16MultiArray * servoMsg = (const std_msgs__msg__Int16MultiArray *)msgin;
  
    int value0 = servoMsg->data.data[0]; // robot number
    int value1 = servoMsg->data.data[1]; // servo number
    int value2 = servoMsg->data.data[2]; // angle data
    analogWrite(3, value2);
}

void setup() {
  set_microros_transports();
  delay(2000);

  msgin.data.capacity = 3; 
  msgin.data.size = 0;
  msgin.data.data = (int16_t*) malloc(msgin.data.capacity * sizeof(int16_t));
  set_microros_transports();
  allocator = rcl_get_default_allocator();

  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));   
digitalWrite(LED1, HIGH);
  // create node
  RCCHECK(rclc_node_init_default(&node, "my_node_serial", "", &support));

  // create subscriber
  RCCHECK(rclc_subscription_init_default(
    &subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int16MultiArray),
    "/topic_rp2sp"));

  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &msg, &subscription_callback, ON_NEW_DATA));

  digitalWrite(LED1, HIGH);  
}

void loop() {
  delay(100);
  RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
}
