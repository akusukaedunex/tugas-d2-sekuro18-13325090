#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/string.hpp"

class CheckMoveDestroyer : public rclcpp::Node {
public:
    CheckMoveDestroyer() : Node("check_move_destroyer") {
        pub_power_ = this->create_publisher<geometry_msgs::msg::Twist>("/power_destroyer", 10);
        pub_type_ = this->create_publisher<std_msgs::msg::String>("/move_type", 10);

        sub_auto_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "/autonomous_destroyer", 10, std::bind(&CheckMoveDestroyer::auto_callback, this, std::placeholders::_1));
        sub_drive_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "/drive_destroyer", 10, std::bind(&CheckMoveDestroyer::drive_callback, this, std::placeholders::_1));
    }

private:
    rclcpp::Time last_drive_time_ = rclcpp::Time(0, 0, RCL_ROS_TIME);

    void auto_callback(const geometry_msgs::msg::Twist::SharedPtr msg) {
        // cek 1 detik terakhir
        if ((this->now() - last_drive_time_).seconds() > 1.0) {
            publish_data(msg, "Autonomous");
            RCLCPP_INFO(this->get_logger(), "Received from Autonomous: linear.x=%.2f, linear.y=%.2f, yaw=%.2f", msg->linear.x, msg->linear.y, msg->angular.z);
        }
    }

    void drive_callback(const geometry_msgs::msg::Twist::SharedPtr msg) {
        last_drive_time_ = this->now(); // update waktu driver
        publish_data(msg, "Driver");
        RCLCPP_INFO(this->get_logger(), "Received from Driver: linear.x=%.2f, linear.y=%.2f, yaw=%.2f", msg->linear.x, msg->linear.y, msg->angular.z);
    }

    void publish_data(const geometry_msgs::msg::Twist::SharedPtr msg, const std::string& type) {
        pub_power_->publish(*msg);
        auto type_msg = std_msgs::msg::String();
        type_msg.data = type;
        pub_type_->publish(type_msg);
    }

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_power_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_type_;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_auto_;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_drive_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CheckMoveDestroyer>());
    rclcpp::shutdown();
    return 0;
}