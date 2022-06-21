#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <std_msgs/msg/bool.hpp>
#include "std_msgs/msg/string.hpp"

#define MY_DEBUG_MSG

using std::placeholders::_1;

auto col_flag_state = std_msgs::msg::Bool();
auto cmd_vel = geometry_msgs::msg::Twist();

class Stop_OnCollision : public rclcpp::Node
{
public:
  Stop_OnCollision()
      : Node("Stop_OnCollision")
  {
    col_flag_sub = this->create_subscription<std_msgs::msg::Bool>(
        "col_flag", 10, std::bind(&Stop_OnCollision::col_flag_sub_callback, this, _1));

    cmd_vel_sub = this->create_subscription<geometry_msgs::msg::Twist>(
        "cmd_vel", 10, std::bind(&Stop_OnCollision::cmd_vel_sub_callback, this, _1));

    cmd_vel_SOC_pub = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel_SOC", 10);
  }

private:
  void col_flag_sub_callback(const std_msgs::msg::Bool::SharedPtr msg) const
  {
    //RCLCPP_INFO(this->get_logger(), "col_flag: '%d'", msg->data);
    if (msg->data == 1)
    {

      RCLCPP_INFO(this->get_logger(), "Collision Detected");
    }
    col_flag_state.data = msg->data;
  }
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr col_flag_sub;

  void cmd_vel_sub_callback(const geometry_msgs::msg::Twist::SharedPtr msg) const
  {
    //RCLCPP_INFO(this->get_logger(), "cmd_vel: lin x='%f', y='%f', z='%f' ang x='%f', y='%f', z='%f'", msg->linear.x, msg->linear.y, msg->linear.z, msg->angular.x, msg->angular.y, msg->angular.z);
    cmd_vel.linear.x = msg->linear.x;
    cmd_vel.angular.z = msg->angular.z;

    if (col_flag_state.data == 0)
    {
      cmd_vel_SOC_pub->publish(cmd_vel);

#ifdef MY_DEBUG_MSG
      RCLCPP_INFO(this->get_logger(), "cmd_vel: lin x='%f', y='%f', z='%f' ang x='%f', y='%f', z='%f'", msg->linear.x, msg->linear.y, msg->linear.z, msg->angular.x, msg->angular.y, msg->angular.z);
#endif
    }

    if (col_flag_state.data == 1)
    {
      if (msg->linear.x > 0)
      {
        cmd_vel.linear.x = 0;
        cmd_vel.angular.z = 0;
        cmd_vel_SOC_pub->publish(cmd_vel);
      }

      if (msg->linear.x < 0)
      {
        cmd_vel_SOC_pub->publish(cmd_vel);
        col_flag_state.data = 0;
#ifdef MY_DEBUG_MSG
        RCLCPP_INFO(this->get_logger(), "cmd_vel: lin x='%f', y='%f', z='%f' ang x='%f', y='%f', z='%f'", msg->linear.x, msg->linear.y, msg->linear.z, msg->angular.x, msg->angular.y, msg->angular.z);
#endif
      }
    }
  }
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_sub;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_SOC_pub;
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Stop_OnCollision>());
  rclcpp::shutdown();
  return 0;
}
