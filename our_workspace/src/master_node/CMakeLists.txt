cmake_minimum_required(VERSION 2.8.3)
project(master_node)

find_package(catkin REQUIRED COMPONENTS
    roscpp
    #rospy  
    pcl_conversions
	pcl_ros
	nodelet
	std_msgs
	sensor_msgs
	#std_srvs
	#message_generation
)

#add_service_files(DIRECTORY srv FILES motors.srv buttons.srv arduino_srv.srv)

#generate_messages(DEPENDENCIES std_msgs)

#generate_messages(
#     DEPENDENCIES
	#     std_msgs
#)

catkin_package(
    LIBRARIES nodelet_plugins #
    CATKIN_DEPENDS nodelet#
)

include_directories(
  include #headers in include
  ${catkin_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
  ${EIGEN_INCLUDE_DIRS}
)


add_library(nd src/main.cpp)
target_link_libraries(nd ${catkin_LIBRARIES})
