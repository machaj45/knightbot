cmake_minimum_required(VERSION 2.8.3)
project(master_node)

find_package(catkin REQUIRED COMPONENTS 
roscpp 
rospy 
std_msgs
std_srvs
message_generation
)

add_service_files(DIRECTORY srv FILES arduino_srv.srv)

generate_messages(DEPENDENCIES std_msgs)

catkin_package(
)

include_directories(
include ${catkin_INCLUDE_DIRS}
)


add_executable(main src/main.cpp)
target_link_libraries(main ${catkin_LIBRARIES})
