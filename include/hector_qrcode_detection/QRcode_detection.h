//=================================================================================================
// Copyright (c) 2014, Vibekananda Dutta, WUT
// All rights reserved.

//=================================================================================================
#ifndef HECTOR_QRCODE_DETECTION_H
#define HECTOR_QRCODE_DETECTION_H

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <opencv/cv.h>
#include <cv_bridge/cv_bridge.h>
#include <stdio.h>
#include <hector_worldmodel_msgs/ImagePercept.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sensor_msgs/image_encodings.h>
#include <time.h>
#include <geometry_msgs/Point.h>
//#include "duration.h"
#include <dirent.h>
#include <sstream>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <tinyxml.h>
//#include <fstream>  
//#include <string.h>  
//#include <sstream>
//#include <dynamic_reconfigure/server.h>
#include <hector_motion_detection/MotionDetectionConfig.h>
#include <std_msgs/Int32.h>
#include <iostream>
#include <tf/transform_listener.h>
#include <zbar.h>
//#include <hector_worldmodel_msgs/ImagePercept.h>
//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
//#include <opencv/cv.h>
//#define M_PI 3.14159265
namespace zbar {
  class ImageScanner;
}

class QRcodeDetection{
public:
   QRcodeDetection();
    ~QRcodeDetection();
private:
    void imageCallback(const sensor_msgs::ImageConstPtr& img); //, const sensor_msgs::CameraInfoConstPtr& info);
    //void mappingCallback(const thermaleye_msgs::Mapping& mapping);
    //void directoryExistsOrCreate(const char* pzPath);
    //bool saveImg(cv::Mat image, const std::string DIRECTORY, const std::string EXTENSION, const char * DIR_FORMAT, const char * FILE_FORMAT);
    
    //void parseRegionXML(std::string file_region, std::vector<cv::Point2f> &region);
    //void dynRecParamCallback(MotionDetectionConfig &config, uint32_t level);

    ros::Publisher image_percept_pub_;
    image_transport::CameraSubscriber camera_sub_;
    image_transport::CameraPublisher rotated_image_pub_;
    image_transport::CameraPublisher qrcode_image_pub_;

    image_transport::Subscriber image_sub_;

    //dynamic_reconfigure::Server<MotionDetectionConfig> dyn_rec_server_;
     zbar::ImageScanner *scanner_;

   // cv_bridge::CvImageConstPtr img_prev_ptr_;
    //cv_bridge::CvImageConstPtr img_current_ptr_;
    //cv_bridge::CvImageConstPtr img_current_col_ptr_;

    int rotated_image_size;
    //double min_percept_size, max_percept_size;
    //double min_density;
    std::string roatation_source_frame_id_;
    std::string roatation_target_frame_id_;


};

/*
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

} // namespace hector_qrcode_detection*/

#endif // HECTOR_QRCODE_DETECTION_H
