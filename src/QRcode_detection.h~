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
#include <zbar.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <tinyxml.h>
#include <fstream>  
#include <string.h>  
#include <sstream>
//#include <dynamic_reconfigure/server.h>
//#include <hector_motion_detection/MotionDetectionConfig.h>
#include <std_msgs/Int32.h>
#include <iostream>
#include <tf/transform_listener.h>
#include <QRcode_detection/Pose.h>
#include "QRcode_detection/Goal.h"
//#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
//#include <opencv/cv.h>
#define PI 3.14159265
// distance from camera to center of robot
#define CAMERA_OFFSET 0.16 

namespace zbar {
  class ImageScanner;
}
/*
typedef struct {
int x, y;
} Pt;*/

class QRcodeDetection{
public:
    QRcodeDetection();
    ~QRcodeDetection();
private:
    void imageCallback(const sensor_msgs::ImageConstPtr& img); //, const sensor_msgs::CameraInfoConstPtr& info);
    bool checkIfNewQR(QRcode_detection::Pose qrPose);
    //bool isLeft(std::string dev);
    //BarcodeHandler(ros::Publisher & chatter_pub,std::string dev);
    
    //void parseRegionXML(std::string file_region, std::vector<cv::Point2f> &region);
    //void dynRecParamCallback(MotionDetectionConfig &config, uint32_t level);

    ros::Publisher publisher;
    ros::Publisher percept_publisher_;
    ros::Publisher qrCodeCountPublisher;
    ros::Publisher chatter_pub;
    image_transport::CameraSubscriber camera_sub_;
    image_transport::CameraPublisher rotated_image_pub_;
    image_transport::CameraPublisher qrcode_image_pub_;

    image_transport::Subscriber image_sub_;

    //dynamic_reconfigure::Server<MotionDetectionConfig> dyn_rec_server_;
     zbar::ImageScanner *scanner_;
     // Pt point[4];
   // cv_bridge::CvImageConstPtr img_prev_ptr_;
    //cv_bridge::CvImageConstPtr img_current_ptr_;
    //cv_bridge::CvImageConstPtr img_current_col_ptr_;
    int qrCount;
    int lengthPixelL, lengthPixelR;
    float distanceL, distanceR, squareDistanceL, squareDistanceR, distanceAvg;
    //float qrcodeX, qrcodeY;
    int rotation_image_size_;
    //double min_percept_size, max_percept_size;
    //double min_density;
   // std::string roatation_source_frame_id_;
    //std::string roatation_target_frame_id_;
   QRcode_detection::Pose seenQRPose;
   QRcode_detection::Pose msg;
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
