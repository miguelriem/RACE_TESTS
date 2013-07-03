#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>


std::string _name="Refree";


int main(int argc, char **argv)
{
  ros::init(argc, argv, _name);

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  ros::Publisher pub = n.advertise<std_msgs::String>("player_out", 1);

  std_msgs::String msg_out;
  ROS_INFO("%s will publish a msg\n", _name.c_str());
  msg_out.data = "hello world";
  pub.publish(msg_out);

  return 0;
}

