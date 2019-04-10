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
#include <iostream>

#include <torch/script.h> // One-stop header.
#include <ATen/ATen.h>


#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/image.hpp"

#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "cv_bridge/cv_bridge.h"

#include "ros2_pytorch.hpp"
using std::placeholders::_1;

class PyTorchNode : public rclcpp::Node
{
public:
    PyTorchNode()
    : Node("pytorch_node")
    {
        subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
                                                                         "/image_raw", std::bind(&PyTorchNode::topic_callback, this, _1));
        publisher_ = this->create_publisher<std_msgs::msg::String>("topic_out");

    }
    
private:
    void topic_callback(const sensor_msgs::msg::Image::SharedPtr msg)
    {
        std::cout << "We got an image" << std::endl;

        /* inspirattion from here https://gist.github.com/zeryx/526dbc05479e166ca7d512a670e6b82d */
        RCLCPP_INFO(this->get_logger(), "We got an image");
        
        
        std::shared_ptr<cv_bridge::CvImage> image_ = cv_bridge::toCvCopy(msg, "bgr8");

        at::TensorOptions options(at::ScalarType::Byte);
        std::vector<int64_t> sizes = {1, 3, msg->height, msg->width};
        at::Tensor tensor_image = torch::from_blob(image_->image.data, at::IntList(sizes), options);
        
        std::vector<torch::jit::IValue> inputs;
        //inputs.push_back(torch::ones({1, 3, 224, 224}));
        
        tensor_image = tensor_image.toType(at::kFloat);
        inputs.emplace_back(tensor_image);

        // Execute the model and turn its output into a tensor.
        at::Tensor output = module_->forward(inputs).toTensor();
        
        std::cout << output.slice(/*dim=*/1, /*start=*/0, /*end=*/5) << '\n';
        
        auto message = std_msgs::msg::String();
        message.data = "Prediction done";
        RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
        publisher_->publish(message);

        
    }
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    std::shared_ptr<torch::jit::script::Module> module_ = torch::jit::load("model.pt");
    
    cv_bridge::CvImage img_bridge;

};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    std::cout << "Starting pytorch node" << std::endl;
    rclcpp::spin(std::make_shared<PyTorchNode>());
    rclcpp::shutdown();
    return 0;
}
