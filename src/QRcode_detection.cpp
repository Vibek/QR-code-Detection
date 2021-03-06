//=================================================================================================
// Copyright (c) 2014, Vibekananda Dutta, WUT
//Faculty of Power and Aeronautical Engineering (MEiL)/ZTMiR Laboratory
// All rights reserved.

//=================================================================================================

#include "QRcode_detection.h"
#include <zbar.h>
using namespace zbar;


QRcodeDetection::QRcodeDetection()
{
      publisher = chatter_pub;
        //device_name = dev;
    //ros::NodeHandle nodeHandle;
    ros::NodeHandle n;
    ros::NodeHandle p_n("~"); //private nh
   
    scanner_ = new zbar::ImageScanner;
    scanner_->set_config(ZBAR_QRCODE, ZBAR_CFG_ENABLE, 1);

    rotation_image_size_ = 2;
    qrCount =0;
    image_transport::ImageTransport it(n);
    image_transport::ImageTransport rotated_image_it(p_n);
    image_transport::ImageTransport qrcode_image_it(p_n);

    //camera_sub_ = it.subscribeCamera("/rgb/image_color", 100, &MotionDetection::imageCallback, this);

    image_sub_ = it.subscribe("/camera/rgb/image_raw", 1, &QRcodeDetection::imageCallback, this);
    qrCodeCountPublisher = n.advertise<QRcode_detection::Goal>("ch_qrcodecount", 1);
    percept_publisher_ = n.advertise<hector_worldmodel_msgs::ImagePercept>("image_percept", 100);
    rotated_image_pub_ = rotated_image_it.advertiseCamera("image_rotated", 10);
    qrcode_image_pub_ = qrcode_image_it.advertiseCamera("image_qrcode", 100);
    chatter_pub = n.advertise<QRcode_detection::Pose>("qrcode_pose", 100);
    cv::namedWindow("view");
    cv::namedWindow("Qrcode");
    cv::namedWindow("Rotation Image");
 
    seenQRPose.x = -1.0; seenQRPose.y = -1.0;
}
 

QRcodeDetection::~QRcodeDetection() {
 //cv::destroyAllWindows();
   cv::destroyWindow("view");
   cv::destroyWindow("Qrcode");
   cv::destroyWindow("Rotataion Image");
}

void QRcodeDetection::imageCallback(const sensor_msgs::ImageConstPtr& img)//, const sensor_msgs::CameraInfoConstPtr& camera_info)
{
  //cv_bridge::CvImageConstPtr cv_image(cv_bridge::toCvCopy(img, sensor_msgs::image_encodings::MONO8));

  cv_bridge::CvImageConstPtr cv_image;
  cv_image = cv_bridge::toCvShare(img, "mono8");
  cv::Mat rotation_matrix = cv::Mat::eye(2,3,CV_32FC1);
  double rotation_angle = 0.0;
    
  ROS_INFO("Received new image with %u x %u pixels.", img->width, img->height);
  cv::imshow("Rotation Image", rotation_matrix);
   cv::waitKey(3);
   
    // calculate rotation angle
    tf::StampedTransform transform;
    tfScalar roll, pitch, yaw;
    transform.getBasis().getRPY(roll, pitch, yaw);
    rotation_angle = -roll;
    ROS_INFO("Rotation_angle: %lf",  rotation_angle);

    // Transform the image.
      cv::Mat in_image = cv_image->image;

      // Compute the output image size.
      int max_dim = in_image.cols > in_image.rows ? in_image.cols : in_image.rows;
      int min_dim = in_image.cols < in_image.rows ? in_image.cols : in_image.rows;
      int noblack_dim = min_dim / sqrt(2);
      int diag_dim = sqrt(in_image.cols*in_image.cols + in_image.rows*in_image.rows);
      int out_size;
      int candidates[] = { noblack_dim, min_dim, max_dim, diag_dim, diag_dim }; // diag_dim repeated to simplify limit case.
      int step = rotation_image_size_;
      out_size = candidates[step] + (candidates[step + 1] - candidates[step]) * (rotation_image_size_ - step);
      ROS_INFO("out_size: %d", out_size);

      // Compute the rotation matrix.
      rotation_matrix = cv::getRotationMatrix2D(cv::Point2f(in_image.cols / 2.0, in_image.rows / 2.0), 180 * rotation_angle / PI, 1);
      rotation_matrix.at<double>(0, 2) += (out_size - in_image.cols) / 2.0;
      rotation_matrix.at<double>(1, 2) += (out_size - in_image.rows) / 2.0;

      // Do the rotation
      cv_bridge::CvImage *temp = new cv_bridge::CvImage(*cv_image);
      cv::warpAffine(in_image, temp->image, rotation_matrix, cv::Size(out_size, out_size));
      //cv_image.reset(temp);

      sensor_msgs::CameraInfo::Ptr info;
      info.reset(new sensor_msgs::CameraInfo());
      info->header = img->header;

      if (rotated_image_pub_.getNumSubscribers() > 0) {

            // try{

       cv_bridge::CvImage cvImg;
      //img_motion.copyTo(cvImg.image);
      cvImg.header = img->header;
      cvImg.image = temp->image;
      cvImg.encoding = sensor_msgs::image_encodings::MONO8;
      rotated_image_pub_.publish(cvImg.toImageMsg(), info);
        
      }
    
   /*catch (cv::Exception& e)
    {
      ROS_ERROR("Image processing error: %s %s %s %i", e.err.c_str(), e.func.c_str(), e.file.c_str(), e.line);
      return;
    }*/
// } 
     cv::imshow("view", temp->image);
       cv::waitKey(3);
  // wrap image data
  Image zbar(cv_image->image.cols, cv_image->image.rows, "Y800", cv_image->image.data, cv_image->image.cols * cv_image->image.rows);

  // scan the image for barcodes
  scanner_->scan(zbar);

  // extract results
  hector_worldmodel_msgs::ImagePercept percept;
  percept.header = img->header;
  //percept.camera_info = *camera_info;
  percept.info.class_id = "qrcode";
  percept.info.class_support = 1.0;

 // int counter =0;


  for(Image::SymbolIterator symbol = zbar.symbol_begin(); symbol != zbar.symbol_end(); ++symbol) {

  time_t seconds;
  struct tm * timeinfo;
  time (&seconds);
// Get the current time
  timeinfo = localtime (&seconds);

   // focal length(calculated before) and test distance
       float f = 275.0, D = 25.0;   

   std::cout<< "[ " <<  timeinfo->tm_hour << ":" << timeinfo->tm_min << std::setw(2) << std::setfill('0') << timeinfo->tm_sec <<"]"<<std::endl;

   std::cout<< "Decoded " << symbol->get_type_name() << " symbol \"" << symbol->get_data() << '"' << std::endl;
   std::ofstream outputFile;
   outputFile.open("Decoded_QRcode.txt",std::ios_base::trunc);
   outputFile << "Decoded " << symbol->get_type_name() << " symbol \"" << symbol->get_data() << '"' << std::endl;
   outputFile.close();

    percept.info.object_id = ros::this_node::getName() + "/" + symbol->get_data();
    percept.info.object_id = symbol->get_data();
    percept.info.object_support = 1.0;
    percept.info.name = symbol->get_data();

    if (symbol->get_location_size() != 4) {
      ROS_WARN("Could not get symbol locations(location_size != 4)");
      continue;
    }

    // point order is left/top, left/bottom, right/bottom, right/top
    int min_x = 99999999, min_y = 99999999, max_x = 0, max_y = 0;
    for(int i = 0; i < 4; ++i) {
      if (symbol->get_location_x(i) > max_x) max_x = symbol->get_location_x(i);//p[0]
      if (symbol->get_location_x(i) < min_x) min_x = symbol->get_location_x(i);//p[1]
      if (symbol->get_location_y(i) > max_y) max_y = symbol->get_location_y(i);//p[2]
      if (symbol->get_location_y(i) < min_y) min_y = symbol->get_location_y(i);//p[3]
    }
    
     // Length of pixels top left and bottom left
       lengthPixelL = abs(min_x - min_y);

     // Length of pixels top right and bottom right
        lengthPixelR = abs(max_x - max_y);
  
      ROS_INFO_STREAM(lengthPixelL << " " << lengthPixelR);

     // Calculate the distance from the qrrcode to camera
        distanceL = (f * D) / lengthPixelL;
        distanceR = (f * D) / lengthPixelR;
        squareDistanceL = pow(distanceL, 2);
        squareDistanceR = pow(distanceR, 2);
        ROS_INFO("Distance from qr code :Left_side: %f Right_side: %f", squareDistanceL, squareDistanceR);
    // rotate the percept back
    cv::Vec3f left_top_corner(min_x, min_y, 1.0f);
    cv::Vec3f right_bottom_corner(max_x, max_y, 1.0f);

     // Calculate Average and convert to meters
      distanceAvg = ((distanceL + distanceR) / 2) / 39.3701;
      ROS_INFO("Average distance in meter: %fm", distanceAvg);
    // TODO: calculate the inverse transformation of rotation_matrix
    /*if (rotation_angle != 0.0) {
      ROS_ERROR("Non-zero rotations are currently not supported!");
      continue;
    }*/

    percept.x      = (left_top_corner(0) + right_bottom_corner(0)) / 2;
    percept.y      = (left_top_corner(1) + right_bottom_corner(1)) / 2;
    percept.width  = right_bottom_corner(0) - left_top_corner(0);
    percept.height = right_bottom_corner(1) - left_top_corner(1);
    percept_publisher_.publish(percept);

  ROS_INFO("location: min_x: %d  min_y: %d  max_x: %d  max_y: %d", min_x, min_y, max_x, max_y);

  ROS_INFO("percept:  x: %f  y: %f  width: %f  height: %f", percept.x, percept.y, percept.width, percept.height);

     //double onaxisv, offaxisv, thetav;
     msg.x = squareDistanceL;
     msg.y = squareDistanceR;

   //ROS_INFO("location: min_x: %d  min_y: %d  max_x: %d  max_y: %d", min_x, min_y, max_x, max_y);
    cv::Rect rect(cv::Point2i(std::max(min_x, 0), std::max(min_y, 0)), cv::Point2i(std::min(max_x, cv_image->image.cols), std::min(max_y, cv_image->image.rows)));
    cv_bridge::CvImagePtr qrcode_cv(new cv_bridge::CvImage(*cv_image));
    qrcode_cv->image = cv_image->image(rect);
         

    if (qrcode_image_pub_.getNumSubscribers() > 0) {
     // try {
        
      cv_bridge::CvImage cvImg;
      //img_motion.copyTo(cvImg.image);
      cvImg.header = img->header;
      cvImg.image = qrcode_cv->image ;
      cvImg.encoding = sensor_msgs::image_encodings::MONO8;
      qrcode_image_pub_.publish(cvImg.toImageMsg(), info);
    
            //} catch(cv::Exception& e) {
        //ROS_ERROR("cv::Exception: %s", e.what());
      //}
    }
      cv::imshow("Qrcode", qrcode_cv->image);
       cv::waitKey(3);
      //publisher.publish(msg);  
      QRcodeDetection::checkIfNewQR(msg);
      ROS_INFO_STREAM(msg);
     ros::Time timeNow = ros::Time::now();
     ROS_INFO_STREAM(timeNow);
       
  }
 
  // clean up
  zbar.set_data(NULL, 0);
}
bool QRcodeDetection::checkIfNewQR(QRcode_detection::Pose qrPose){
/*for (std::vector<corobot_common::Pose>::iterator it = qrCodeList.begin() ; it != qrCodeList.end(); ++it){
corobot_common::Pose itPose = *it;
if(abs(qrPose.x - itPose.x) <= 1.5 && abs(qrPose.y - itPose.y) <= 1.5){ //if the point approx matches the points in the list
return false;
}
}*/
/*
if(seenQRPose.x != -1.0)
if(abs(qrPose.x - seenQRPose.x) <= 1.5 && abs(qrPose.y - seenQRPose.y) <= 1.5 && abs(qrPose.theta - seenQRPose.theta) <= 0.7) //if the point approx matches the points in the list
return false;
*/
             seenQRPose.x = qrPose.x; 
             seenQRPose.y = qrPose.y; 
             seenQRPose.theta = qrPose.theta;
             std::stringstream ss; 
                QRcode_detection::Goal topicMsg;
                if(qrPose.x > seenQRPose.x)
                     ss << "L" << ++qrCount;
                 else
                       ss << "R" << ++qrCount;
                           topicMsg.name = ss.str(); 
                             qrCodeCountPublisher.publish(topicMsg);
                          return true;
}


//} // namespace hector_qrcode_detection

int main(int argc, char **argv)
{
  
  //cv::startWindowThread();

  ros::init(argc, argv, "QRcode_detection");
  QRcodeDetection qrd;
  ros::spin();
  
  return 0;
}
