#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_srvs/SetBool.h"
#include "master_node/motors.h"


ros::ServiceClient client;

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());

  /*
master_node::arduino_srv srv;
srv.request.cmd = 4;
srv.request.param = 20;


if( client.call(srv)){
ROS_INFO("OK");
}else{
ROS_INFO("ERROR");
}

ROS_INFO("Move robot, move!");
*/
}



bool  btn1(std_srvs::SetBool::Request  &req, std_srvs::SetBool::Response &res)
{
res.success = true;
res.message = "Button 1 was pressed";
ROS_INFO("request: x=%d", (int)req.data);
  
/*
master_node::arduino_srv srv;
  srv.request.cmd = 4;
  srv.request.param = 20;
  client.call(srv);

ROS_INFO("Move robot, move!");
*/
  return true;
}

bool fake(master_node::motors::Request  &req, master_node::motors::Response &res)
{

res.success = true;
res.message = "Robot was offended and go away";
ROS_INFO("CMD=%d  VEL=%d ", req.cmd, req.param);
  return true;
}


int main(int argc, char **argv)
{
ros::init(argc, argv, "master_robot_node");
ros::NodeHandle n;
client = n.serviceClient<master_node::motors>("i_am_fake_robot",true);

ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
ros::ServiceServer server = n.advertiseService("btn1", btn1);
ros::ServiceServer server2 = n.advertiseService("i_am_fake_robot", fake);

ros::spin();

//master_node::arduino_srv srv;


/*
ros::Rate r(1); // 10 hz
while (ros::ok())
{
master_node::arduino_srv srv;

  srv.request.cmd = 4;
  srv.request.param = 20;
ROS_INFO("I am here");  
client.call(srv);

  r.sleep();
}*/

  return 0;
}
