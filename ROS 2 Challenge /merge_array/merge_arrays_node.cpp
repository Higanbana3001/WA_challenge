#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32_multi_array.hpp"
#include <vector>
#include <algorithm>

class MergeArraysNode : public rclcpp::Node
{
public:
    MergeArraysNode() : Node("merge_arrays_node")
    {
        sub1 = this->create_subscription<std_msgs::msg::Int32MultiArray>(
            "/input/array1", 1, std::bind(&MergeArraysNode::array1Callback, this, std::placeholders::_1));

        sub2 = this->create_subscription<std_msgs::msg::Int32MultiArray>(
            "/input/array2", 1, std::bind(&MergeArraysNode::array2Callback, this, std::placeholders::_1));

        pub = this->create_publisher<std_msgs::msg::Int32MultiArray>("/output/array", 1);
    }

    void array1Callback(const std_msgs::msg::Int32MultiArray::SharedPtr msg)
    {
        array1 = msg->data;
        mergeAndPublish();
    }

    void array2Callback(const std_msgs::msg::Int32MultiArray::SharedPtr msg)
    {
        array2 = msg->data;
        mergeAndPublish();
    }

    void mergeAndPublish()
    {
        std::vector<int32_t> mergedArray;
        std::merge(array1.begin(), array1.end(), array2.begin(), array2.end(), std::back_inserter(mergedArray));

        auto mergedMsg = std_msgs::msg::Int32MultiArray();
        mergedMsg.data = mergedArray;

        pub->publish(mergedMsg);
    }

private:
    rclcpp::Subscription<std_msgs::msg::Int32MultiArray>::SharedPtr sub1;
    rclcpp::Subscription<std_msgs::msg::Int32MultiArray>::SharedPtr sub2;
    rclcpp::Publisher<std_msgs::msg::Int32MultiArray>::SharedPtr pub;
    std::vector<int32_t> array1;
    std::vector<int32_t> array2;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<MergeArraysNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
