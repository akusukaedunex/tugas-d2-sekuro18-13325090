#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/string.hpp"

class MoveNode : public rclcpp::Node {
public:
    MoveNode() : Node("move_destroyer") {
        sub_power_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "/power_destroyer", 10, std::bind(&MoveNode::callback_power, this, std::placeholders::_1));
        sub_type_ = this->create_subscription<std_msgs::msg::String>(
            "/move_type", 10, std::bind(&MoveNode::callback_type, this, std::placeholders::_1));
    }

private:
    std::string tipe_aktif = "Belum Ada";

    void callback_type(const std_msgs::msg::String::SharedPtr msg) {
        tipe_aktif = msg->data;
    }

    void callback_power(const geometry_msgs::msg::Twist::SharedPtr msg) {
        RCLCPP_INFO(this->get_logger(), "Tipe Kendali Aktif: %s", tipe_aktif.c_str());
        std::string gerak = "";
        if (msg->linear.x > 0) gerak += "Maju ";
        if (msg->linear.x < 0) gerak += "Mundur ";
        if (msg->linear.y > 0) gerak += "Geser Kiri ";
        if (msg->linear.y < 0) gerak += "Geser Kanan ";
        if (msg->angular.z > 0) gerak += "Berputar Kiri";
        if (msg->angular.z < 0) gerak += "Berputar Kanan";
        if (gerak == "") gerak = "Berhenti";
        RCLCPP_INFO(this->get_logger(), "Orientasi Gerak : %s", gerak.c_str());
        RCLCPP_INFO(this->get_logger(), "-----------------------------------");
    }

    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_power_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_type_;
};

int main(int argc, char ** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MoveNode>());
    rclcpp::shutdown();
    return 0;
}