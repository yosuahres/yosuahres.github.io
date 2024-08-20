#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

ros::Publisher pub_opencv;

// ros::Subscriber sub_vision_atas;

// void vision_atas_callback(const romusa_messages::camatas::ConstPtr &camatas_msg);

void img_callback(const sensor_msgs::Image::ConstPtr& msg){
    cv_bridge::CvImagePtr cv_ptr;
    try{
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }catch(cv_bridge::Exception& e){
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;;
    }

    // Define the top left and bottom right points of the rectangle
    cv::Point top_left(100, 100);
    cv::Point bottom_right(200, 200);

    // Draw the rectangle on the image
    cv::rectangle(cv_ptr->image, top_left, bottom_right, CV_RGB(255, 0, 0), 3);

    cv::imshow("camera", cv_ptr->image);
    cv::waitKey(3);
}

int main(int argc, char **argv)
{
    // sub_vision_atas = nh.subscribe<romusa_messages::camatas>("camatas", 1000, vision_atas_callback);

    ros::init(argc, argv, "video_cv");
    ros::NodeHandle nh; 

    image_transport::ImageTransport it(nh);
    image_transport::Subscriber sub = it.subscribe("image_in_2", 1, img_callback);
    
    
    ros::spin();

    return 0;
}



