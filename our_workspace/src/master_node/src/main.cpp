#include <ros/ros.h>
#include <nodelet/nodelet.h>
#include <pluginlib/class_list_macros.h>

#include "std_msgs/String.h"
#include "std_msgs/Int16MultiArray.h"
#include "std_msgs/Int16.h"

#include <sensor_msgs/PointCloud2.h>


//restricting
#include <pcl/filters/passthrough.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/common/transforms.h>
#include <pcl_conversions/pcl_conversions.h>

#include <pcl_ros/transforms.h>
#include <tf2_eigen/tf2_eigen.h>
#include <tf2_ros/transform_listener.h>
#include <tf/transform_broadcaster.h>

#include <pcl/common/io.h>

#include <Eigen/Geometry> //quaternions

#include <pcl/kdtree/kdtree.h>
#include <pcl/segmentation/extract_clusters.h>

#include <pcl/common/pca.h>
#include <pcl/PointIndices.h>
#include <pcl/filters/extract_indices.h>

#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>


#include <pcl/filters/voxel_grid.h> //Downsampling

ros::Publisher pub_motor;
tf2_ros::Buffer tf_buffer;
ros::Publisher pub_tr_cloud;


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

		output.data[0]=5;
		output.data[1]=90;
		pub_motor.publish(output);
	}
}

void canCountCallback(const std_msgs::Int16::ConstPtr& can_count){
ROS_INFO("There is: [%d] cans.", can_count->data);
}

void odomLCallback(const std_msgs::Int16::ConstPtr& odom_L){
ROS_INFO("Odometry left: [%d] mm.", odom_L->data);
}

void odomRCallback(const std_msgs::Int16::ConstPtr& odom_R){
ROS_INFO("Odometry right: [%d] mm.", odom_R->data);
}



///
///
///Make transformations first
///
///


/**
 * return [x,y,z,w] (w+xi+yj+zk)
 * */
Eigen::Quaternion<float> setQuaternion(float w, float x, float y, float z){
	Eigen::Quaternion<float> q(w,x,y,z);
	return q;
}


Eigen::Quaternion<float>  toQuaternion(double roll, double pitch, double yaw){
    Eigen::Quaternion<float>  q;
    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);
    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);

    q.w() = cy * cr * cp + sy * sr * sp;
    q.x() = cy * sr * cp - sy * cr * sp;
    q.y() = cy * cr * sp + sy * sr * cp;
    q.z() = sy * cr * cp - cy * sr * sp;
    return q;
}


/**
 * 
 * Theta radians
 * */
void transformation(pcl::PointCloud<pcl::PointXYZ>::Ptr cloudPtr, Eigen::Quaternion<float> q, float xtrans, float ytrans, float ztrans){	
	pcl::PointCloud<pcl::PointXYZ>::Ptr tmp(new pcl::PointCloud<pcl::PointXYZ>);
	Eigen::Affine3f transform = Eigen::Affine3f::Identity();

	transform.translation() << xtrans, ytrans, ztrans; //Translation
	transform.rotate (setQuaternion(q.w(),q.x(),q.y(),q.z())); //rotation

  // Executing the transformation
	pcl::PointCloud<pcl::PointXYZ>::Ptr transformed_cloud (new pcl::PointCloud<pcl::PointXYZ> ());
	pcl::transformPointCloud (*cloudPtr, *tmp, transform);	
	*cloudPtr=*tmp; 	
}

/**
 * Rotate view into XYZ meaningfully
 * */
void rotate_view(const pcl::PointCloud<pcl::PointXYZ>::Ptr point_cloud_realsense) {

  Eigen::Quaternion<float> q_n = toQuaternion(0,-0.5933,0);
  transformation(point_cloud_realsense, q_n, 0.055, 0.032,0.32);
}



/**
 * Publish transformed (rotated) cloud
 * */
void transform_cloud(const pcl::PointCloud<pcl::PointXYZ>::Ptr point_cloud_uav, const ros::Time& stamp, const std::string target_frame_,
                     const std::string uav_frame_, const bool transform) {
  pcl::PointCloud<pcl::PointXYZ>::Ptr point_cloud_target(new pcl::PointCloud<pcl::PointXYZ>());
  geometry_msgs::TransformStamped     uav2target_transform;
  try {
    // Obtain transform from uav frame to target frame
    uav2target_transform = tf_buffer.lookupTransform(target_frame_, uav_frame_, stamp, ros::Duration(0.1));

    // Transform point cloud to target frame
    Eigen::Affine3d uav2target_eigen_transform = tf2::transformToEigen(uav2target_transform);
    if (transform){
    uav2target_eigen_transform.translation() << 0,0,0; //set zero translation
    }
    pcl::transformPointCloud(*point_cloud_uav, *point_cloud_target, uav2target_eigen_transform);
    point_cloud_target->header.frame_id = target_frame_;
    *point_cloud_uav                    = *point_cloud_target;
  }
  catch (tf2::TransformException& ex) {
    ROS_ERROR("Error during transform to target frame: %s", ex.what());
    return;
  }
}



///
///
///Filtering part
///
///

/**
 *  restricting ground
 * */
void restrict_ground(pcl::PointCloud<pcl::PointXYZ>::Ptr cloudPtr){
pcl::PointCloud<pcl::PointXYZ>::Ptr tmp(new pcl::PointCloud<pcl::PointXYZ>);
  pcl::PassThrough<pcl::PointXYZ> pass;
  pass.setInputCloud (cloudPtr);
  //pass.setFilterFieldName ("y"); //chosing axe
  //pass.setFilterLimits (-0.5, 1000.0); //sets limits for filtering

  pass.setFilterFieldName ("z"); //chosing axe
  pass.setFilterLimits (0, 1000.0); //sets limits for filtering
  pass.filter (*tmp);
  *cloudPtr=*tmp;
}

/**
 * Care about downsampling
 * */
void downsampling(pcl::PointCloud<pcl::PointXYZ>::Ptr cloudPtr, float leaf_size){
	pcl::PointCloud<pcl::PointXYZ>::Ptr tmp(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::VoxelGrid<pcl::PointXYZ> sor;
	sor.setInputCloud (cloudPtr); //setting input cloud
	sor.setLeafSize (leaf_size, leaf_size, leaf_size); //seting downsampling parameters
	sor.filter (*tmp); //saving 
	*cloudPtr=*tmp; 	
}


void pointCloudCallback(const sensor_msgs::PointCloud2ConstPtr& cloud_msg) {
  ROS_INFO_THROTTLE(1.0, "Received pointcloud");
  pcl::PointCloud<pcl::PointXYZ>::Ptr input(new pcl::PointCloud<pcl::PointXYZ>());
  pcl::PCLPointCloud2 tmp;

  pcl_conversions::toPCL(*cloud_msg, tmp);
  pcl::fromPCLPointCloud2(tmp, *input);
  
  transform_cloud(input, cloud_msg->header.stamp, "camera_link", cloud_msg->header.frame_id, true); 
  rotate_view(input); 
  
  sensor_msgs::PointCloud2 output;
  pcl::toROSMsg(*input, output);
  pub_tr_cloud.publish(output);
  
  //restrict_ground(input);
 // downsampling(input, 0.1);
  
 // ROS_INFO_THROTTLE(1.0, "Received pointcloud %s", input->header.frame_id);
  //pcl::fromPCLPointCloud2(tmp, *input);
    
}

///
///
/// Transform to clusters
///
///


/**
 * Create clusters
 * cloudPtr - input cloud
 * cluster_tolerance - radius
 * min_size - minimum number of points for creating cluster
 * max_size - maximum number of points for creating cluster
 * */
std::vector<pcl::PointIndices> cluster_extraction(pcl::PointCloud<pcl::PointXYZ>::Ptr cloudPtr, float cluster_tolerance, int min_size, int max_size){
	 pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ>);
	tree->setInputCloud (cloudPtr);
	
	std::vector<pcl::PointIndices> cluster_indices; //vector including all clusters
	
	pcl::EuclideanClusterExtraction<pcl::PointXYZ> ec;
	ec.setClusterTolerance (cluster_tolerance); // x [m]
	ec.setMinClusterSize (min_size);
	ec.setMaxClusterSize (max_size);
	ec.setSearchMethod (tree);
	ec.setInputCloud (cloudPtr);
	ec.extract (cluster_indices);

		return cluster_indices;	
}


/**
 * Make clusters (vector of pointclouds)
 * */
std::vector<pcl::PointCloud<pcl::PointXYZ> > get_pointClouds( std::vector<pcl::PointIndices> clusters, pcl::PointCloud<pcl::PointXYZ>::Ptr cloudPtr){
	
	std::vector<pcl::PointCloud<pcl::PointXYZ> > output;
	for (std::vector<pcl::PointIndices>::const_iterator it = clusters.begin (); it != clusters.end (); ++it){
	
		pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_cluster (new pcl::PointCloud<pcl::PointXYZ>); //create temporary pointcloud represents one cluster
		for (std::vector<int>::const_iterator pit = it->indices.begin (); pit != it->indices.end (); ++pit){ //write points
            cloud_cluster->points.push_back (cloudPtr->points[*pit]);
		}
		
		cloud_cluster->width = cloudPtr->points.size ();
		cloud_cluster->height = 1;
		cloud_cluster->is_dense = false;   
			
		output.push_back(*cloud_cluster); //write output		
	}
	return output;
}


namespace master_node
{
class Main : public nodelet::Nodelet {
public:
    virtual void onInit() {
        ROS_INFO("-------Main nodelet ready!--------");
        // Initialize ROS
        ros::NodeHandle& nh = getPrivateNodeHandle();
	ros::Subscriber sub1 = nh.subscribe("chatter", 1000, chatterCallback);
	ros::Subscriber sub2 = nh.subscribe("can_count", 1000, canCountCallback);
	ros::Subscriber sub3 = nh.subscribe("odomL", 1000, odomLCallback);
	ros::Subscriber sub4 = nh.subscribe("odomR", 1000, odomRCallback);
	ros::Subscriber sub5 = nh.subscribe<sensor_msgs::PointCloud2>("/camera/depth/image_rect_raw", 1, pointCloudCallback);
	
	pub_motor = nh.advertise<std_msgs::Int16MultiArray>("motor_control",1);
	pub_tr_cloud= nh.advertise<sensor_msgs::PointCloud2>("tr_cloud", 1);

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
