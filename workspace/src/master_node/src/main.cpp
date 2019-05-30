#include <ros/ros.h>
#include <nodelet/nodelet.h>
#include <pluginlib/class_list_macros.h>

//For poincloud
#include <sensor_msgs/PointCloud2.h>
#include <pcl/filters/voxel_grid.h>//voxel grid
#include <pcl/filters/radius_outlier_removal.h>//radius filtering
#include <pcl_conversions/pcl_conversions.h>
#include <pcl_ros/transforms.h>
#include "pcl_ros/point_cloud.h"
#include <pcl/point_types.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl/conversions.h>
#include <pcl_ros/transforms.h>

//For services
#include "std_msgs/String.h"
#include "std_srvs/SetBool.h"
#include "master_node/motors.h"

ros::Publisher pub_cloud, test_pub;
ros::ServiceClient client;

/**
 * Care about downsampling
 * */
void downsampling(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloudPtr, float leaf_size){
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr tmp(new pcl::PointCloud<pcl::PointXYZRGB>);
    pcl::VoxelGrid<pcl::PointXYZRGB> sor;
    sor.setInputCloud (cloudPtr); //setting input cloud
    sor.setLeafSize (leaf_size, leaf_size, leaf_size); //seting downsampling parameters
    sor.filter (*tmp); //saving
    *cloudPtr=*tmp;
}

/**
 *  r [m] - radius in which neighbours should be
 *  kNeigbours - how many neighbours should should point has at least
 * */
void kN_filtering(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloudPtr, double r, int kNeigbours){
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr tmp(new pcl::PointCloud<pcl::PointXYZRGB>);
    pcl::RadiusOutlierRemoval<pcl::PointXYZRGB> outrem;
    // build the filter
    outrem.setInputCloud(cloudPtr);
    outrem.setRadiusSearch(r);
    outrem.setMinNeighborsInRadius (kNeigbours);
    // apply filter
    outrem.filter (*tmp);
    *cloudPtr=*tmp;
}

void pointCloudCallback(const sensor_msgs::PointCloud2ConstPtr& cloud_msg) {
    test_pub.publish(cloud_msg);

}

//Accepting information from motor encoders
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}

//Information from btn, if it was released
bool  btn1(std_srvs::SetBool::Request  &req, std_srvs::SetBool::Response &res)
{
res.success = true;
res.message = "Button 1 was pressed";
ROS_INFO("request: x=%d", (int)req.data);
  return true;
}

//Simulating robot behavior
bool fake(master_node::motors::Request  &req, master_node::motors::Response &res)
{

res.success = true;
res.message = "Robot was offended and go away";
ROS_INFO("CMD=%d  VEL=%d ", req.cmd, req.param);
  return true;
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


	client = nh.serviceClient<master_node::motors>("i_am_fake_robot",true);
	ros::Subscriber sub = nh.subscribe("chatter", 1000, chatterCallback);		ros::ServiceServer server = nh.advertiseService("btn1", btn1);
	ros::ServiceServer server2 = nh.advertiseService("i_am_fake_robot", fake);

/*
 //Work but I do not subscribe pointcloud
        ros::Subscriber sub_cloud = nh.subscribe<sensor_msgs::PointCloud2>("/camera/depth/color/points", 1, pointCloudCallback);
        pub_cloud = nh.advertise<sensor_msgs::PointCloud2>("output_cloud", 1);
        test_pub = nh.advertise<sensor_msgs::PointCloud2>("test_cloud", 1);
*/


        // Spin
        ros::spin();
        
        
        /* ros::Rate loop_rate(1);
    while (ros::ok()) {
      loop_rate.sleep();
    }*/
    }
};
}
PLUGINLIB_EXPORT_CLASS(master_node::Main, nodelet::Nodelet)
