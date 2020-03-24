#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include "nav_msgs/Odometry.h"

float pickUpPos[3] = {3, 4, 1};
float dropOffPos[3] = {-1, 0, 1};
bool inPickUpZone = false;
bool inDropOffZone = false;

void call_back(const nav_msgs::Odometry::ConstPtr &msg)
{
  float cur_x;
  float cur_y;
  cur_x = msg->pose.pose.position.x;
  cur_y = msg->pose.pose.position.y;

  if (std::abs(-pickUpPos[0] - cur_x) < 1 && std::abs(-pickUpPos[1] - cur_y) < 1)
  {
    inPickUpZone = true;
  }
  if (inPickUpZone==true && std::abs(-dropOffPos[0] - cur_x) < 1 && std::abs(-dropOffPos[1] - cur_y) < 1)
  {
    inDropOffZone = true;
  }
  ROS_INFO("cur x: %f, cur y: %f", cur_x, cur_y);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  //Design subscriber of odometry by using nodehandle n
  
  ros::Subscriber pose_sub = n.subscribe("odom", 1, call_back);
  ros::Rate loop_rate(0.1); //
  
  // Set our initial shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CUBE;

  while (ros::ok())
  {
    visualization_msgs::Marker marker;
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "map";
    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    marker.ns = "basic_shapes";
    marker.id = 0;

    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
    marker.type = shape;

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    marker.action = visualization_msgs::Marker::ADD;

    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
    marker.pose.position.x = pickUpPos[0];
    marker.pose.position.y = pickUpPos[1];
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = pickUpPos[2];

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 1.0;
    marker.scale.y = 1.0;
    marker.scale.z = 1.0;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();

    // Publish the marker
    while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      ROS_INFO("Debug");
      sleep(1);
    }
    marker_pub.publish(marker);
    ROS_INFO("Marker at pick-up location is created");
    while(!inPickUpZone)
    {
      ros::spinOnce();
    }
    if (inPickUpZone == true)
    {
      marker.action = visualization_msgs::Marker::DELETE;
      marker_pub.publish(marker);
      ROS_INFO("Now, we arrived in pick-up zone");
      ros::Duration(5.0).sleep();
      ROS_INFO("Marker at the drop off zone is created!");
      marker.pose.position.x = dropOffPos[0];
      marker.pose.position.y = dropOffPos[1];
      marker.pose.position.z = 0;
      marker.pose.orientation.x = 0.0;
      marker.pose.orientation.y = 0.0;
      marker.pose.orientation.z = 0.0;
      marker.pose.orientation.w = dropOffPos[2];
      marker.action = visualization_msgs::Marker::ADD;
      shape = visualization_msgs::Marker::SPHERE;
      marker_pub.publish(marker);
    }
    while(!inDropOffZone)
    {
      ros::spinOnce();
    }
    if (inDropOffZone == true)
    {
      marker.action = visualization_msgs::Marker::DELETE;
      marker_pub.publish(marker);
      ROS_INFO("Here we are! drop-off zone");
    }

    ros::Duration(5.0).sleep();


    loop_rate.sleep();
    return 0;
  }
}
