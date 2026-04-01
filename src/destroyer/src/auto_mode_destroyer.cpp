#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <stdlib.h>

class AutoNode : public rclcpp::Node {
public:
    AutoNode() : Node("auto_mode_destroyer") {
        pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/autonomous_destroyer", 10);
        
        // Timer 2Hz
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(500),
            std::bind(&AutoNode::kirim_data_acak, this));
    }

private:
    void kirim_data_acak() {
        auto pesan = geometry_msgs::msg::Twist();
        
        // linear
        pesan.linear.x = (rand() % 11) - 5; 
        pesan.linear.y = (rand() % 11) - 5;
        // angular
        pesan.angular.z = (rand() % 3) - 1;

        pub_->publish(pesan);
    }
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<AutoNode>());
    rclcpp::shutdown();
    return 0;
}