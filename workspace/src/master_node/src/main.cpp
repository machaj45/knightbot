#include <ros/ros.h>
#include <nodelet/nodelet.h>
#include <pluginlib/class_list_macros.h>

#include "std_msgs/String.h"
#include "std_msgs/Int16MultiArray.h"

ros::Publisher pub_motor;

//Accepting information from motor encoders
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  	ROS_INFO("I heard: [%s]", msg->data.c_str());
	std_msgs::Int16MultiArray output;
	output.data.resize(2);
	for(int i = 0; i<4; i++){
		output.data[0]=4;
		output.data[1]=100;
		pub_motor.publish(output);
		output.data[0]=7;
		output.data[1]=90;
		pub_motor.publish(output);
	}
}




namespace master_node
{
class Main : public nodelet::Nodelet {
public:
    virtual void onInit() {
	ROS_ERROR("START");
        ROS_INFO("-------Main nodelet ready!--------");
        // Initialize ROS
        ros::NodeHandle& nh = getPrivateNodeHandle();
	ros::Subscriber sub = nh.subscribe("chatter", 1000, chatterCallback);
	pub_motor = nh.advertise<std_msgs::Int16MultiArray>("motor_control",1);


        // Spin
        ros::spin();
                
/*         ros::Rate loop_rate(1);
    while (ros::ok()) {
      loop_rate.sleep();
}*/
    }
};
}
PLUGINLIB_EXPORT_CLASS(master_node::Main, nodelet::Nodelet)
