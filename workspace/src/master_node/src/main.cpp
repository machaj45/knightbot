#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_srvs/SetBool.h"

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}

bool  btn1(std_srvs::SetBool::Request  &req, std_srvs::SetBool::Response &res)
{
res.success = true;
res.message = "Button 1 was pressed";
ROS_INFO("request: x=%d", (int)req.data);
  return true;
}

int main(int argc, char **argv)
{
ros::init(argc, argv, "listener");
ros::NodeHandle n;
ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
ros::ServiceServer client = n.advertiseService("btn1", btn1);
  
ros::spin();

  return 0;
}
