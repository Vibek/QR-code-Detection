//=================================================================================================
// Copyright (c) 2014, Vibekananda Dutta, WUT
// All rights reserved.

//=================================================================================================
#ifndef HECTOR_QRCODE_DETECTION_H
#define HECTOR_QRCODE_DETECTION_H

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <tf/transform_listener.h>
#include <std_msgs/String.h>
#include <stdio.h>
#include <iostream>
#include <sensor_msgs/image_encodings.h>

//#define M_PI 3.14159265
namespace zbar {
  class ImageScanner;
}

namespace hector_qrcode_detection {

class qrcode_detection_impl {
public:
  qrcode_detection_impl(ros::NodeHandle nh, ros::NodeHandle priv_nh);
  ~qrcode_detection_impl();

protected:
  void imageCallback(const sensor_msgs::ImageConstPtr& image, const sensor_msgs::CameraInfoConstPtr& camera_info);

private:
  ros::NodeHandle nh_;
  image_transport::ImageTransport image_transport_;
  image_transport::CameraSubscriber camera_subscriber_;
  image_transport::CameraPublisher rotated_image_publisher_;
  image_transport::CameraPublisher qrcode_image_publisher_;
  
  //image_transport::Subscriber image_sub_;

  ros::Publisher percept_publisher_;

  zbar::ImageScanner *scanner_;

  tf::TransformListener *listener_;
  std::string rotation_source_frame_id_;
  std::string rotation_target_frame_id_;
  int rotation_image_size_;
};

} // namespace hector_qrcode_detection

#endif // HECTOR_QRCODE_DETECTION_H