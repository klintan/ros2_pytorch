//
//  ros2_pytorch.cpp
//
//  Created by Andreas Klintberg on 11/17/18.
//  Copyright © 2018 Andreas Klintberg. All rights reserved.
//

#include <chrono>
#include <cstdio>
#include <memory>
#include <string>

#include <torch/script.h> // One-stop header.

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/image.hpp"

#include "ros2_pytorch.hpp"
using std::placeholders::_1;

class PyTorchNode : public rclcpp::Node
{
public:
    PyTorchNode()
    : Node("pytorch_node")
    {
        subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
                                                                         "topic_in", std::bind(&PyTorchNode::topic_callback, this, _1));
        publisher_ = this->create_publisher<std_msgs::msg::String>("topic_out");

    }
    
private:
    void topic_callback(const sensor_msgs::msg::Image::SharedPtr msg)
    {
        
        RCLCPP_INFO(this->get_logger(), "We got an image");
        module_ = torch::jit::load("model.pt");
        
        
        auto message = std_msgs::msg::String();
        message.data = "Hello, world!";
        RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
        publisher_->publish(message);

        
    }
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    std::shared_ptr<torch::jit::script::Module> module_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    

    rclcpp::spin(std::make_shared<PyTorchNode>());
    rclcpp::shutdown();
    return 0;
}
