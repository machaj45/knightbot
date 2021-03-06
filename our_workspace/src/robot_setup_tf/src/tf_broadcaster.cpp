#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "robot_tf_publisher");
  ros::NodeHandle n;

  ros::Rate r(100);

  tf::TransformBroadcaster broadcaster;

  tf::Quaternion orientation;
  tf::Vector3 trans  = tf::Vector3(0.055, 0.0325, 0.32);
  orientation.setRPY( 0, (-34/180)*3.14159, 0 );
  while(n.ok()){
    broadcaster.sendTransform(tf::StampedTransform(tf::Transform(orientation, trans),ros::Time::now(),"base_link", "camera_link"));
    r.sleep();
  }
}	
