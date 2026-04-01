#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <termios.h>
#include <unistd.h>
#include <thread>

class DriveNode : public rclcpp::Node {
public:
    DriveNode() : Node("drive_mode_destroyer") {
        pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/drive_destroyer", 10);
        
        RCLCPP_INFO(this->get_logger(), "=============================================");
        RCLCPP_INFO(this->get_logger(), "  DESTROYER MARK II - DRIVE MODE ACTIVATED   ");
        RCLCPP_INFO(this->get_logger(), "=============================================");
        RCLCPP_INFO(this->get_logger(), "KONTROL:");
        RCLCPP_INFO(this->get_logger(), "  ↑ : Bergerak maju dengan kecepatan 5m/s");
        RCLCPP_INFO(this->get_logger(), "  ↓ : Bergerak mundur dengan kecepatan 5m/s");
        RCLCPP_INFO(this->get_logger(), "  → : Bergerak ke kanan dengan kecepatan 5m/s");
        RCLCPP_INFO(this->get_logger(), "  ← : Bergerak ke kiri dengan kecepatan 5m/s");
        RCLCPP_INFO(this->get_logger(), "  R : Rotasi ke kanan dengan kecepatan sudut 1 rad/s");
        RCLCPP_INFO(this->get_logger(), "  L : Rotasi ke kiri dengan kecepatan sudut 1 rad/s");
        RCLCPP_INFO(this->get_logger(), "  B : Melakukan rem/berhenti (kecepatan 0m/s)");
        RCLCPP_INFO(this->get_logger(), "=============================================");
        thread_input_ = std::thread(&DriveNode::baca_keyboard, this);
    }

    ~DriveNode() {
        if (thread_input_.joinable()) thread_input_.join();
    }

private:
    void baca_keyboard() {
        struct termios mode_lama, mode_baru;
        tcgetattr(STDIN_FILENO, &mode_lama);
        mode_baru = mode_lama;
        mode_baru.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &mode_baru);

        char tombol;
        while (rclcpp::ok()) {
            auto msg = geometry_msgs::msg::Twist();
            bool valid = true;
            std::string info_aksi = "";
            read(STDIN_FILENO, &tombol, 1);

            if (tombol == '\033') { 
                read(STDIN_FILENO, &tombol, 1);
                read(STDIN_FILENO, &tombol, 1);
                if (tombol == 'A') { // up
                    msg.linear.x = 5.0; 
                    info_aksi = "Gerak maju dengan kecepatan 5m/s";
                }
                else if (tombol == 'B') { // down
                    msg.linear.x = -5.0; 
                    info_aksi = "Gerak mundur dengan kecepatan 5m/s"; 
                }
                else if (tombol == 'C') { // right
                    msg.linear.y = -5.0; 
                    info_aksi = "Gerak ke kanan dengan kecepatan 5m/s"; 
                }
                else if (tombol == 'D') { // left
                    msg.linear.y = 5.0; 
                    info_aksi = "Gerak ke kiri dengan kecepatan 5m/s"; 
                }
                else valid = false;
            } 
            else if (tombol == 'b' || tombol == 'B') { // rem
                msg.linear.x = 0; msg.linear.y = 0; msg.angular.z = 0;
                info_aksi = "REM!";
            }
            else if (tombol == 'r' || tombol == 'R') { // putar kanan
                msg.angular.z = -1.0; 
                info_aksi = "Putar ke kanan dengan kecepatan sudut 1 rad/s"; 
            }
            else if (tombol == 'l' || tombol == 'L') {
                msg.angular.z = 1.0; 
                info_aksi = "Rotasi ke kiri dengan kecepatan sudut 1 rad/s";
            }
            else valid = false;

            if (valid) {
                pub_->publish(msg);
                RCLCPP_INFO(this->get_logger(), "PUBLISH: %s", info_aksi.c_str());
            }
        }
        tcsetattr(STDIN_FILENO, TCSANOW, &mode_lama);
    }

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_;
    std::thread thread_input_;
};

int main(int argc, char ** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DriveNode>());
    rclcpp::shutdown();
    return 0;
}
