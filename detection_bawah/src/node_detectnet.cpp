#include <jetson-inference/detectNet.h>
#include <jetson-utils/cudaResize.h>
#include <jetson-utils/videoOutput.h>
#include <romusa_messages/ObjectDetection.h>

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>

#include <unordered_map>

#include "image_converter.h"
#include "ros_compat.h"
#include <romusarival/filter.hpp>

#include <jetson-inference/objectTrackerIOU.h>
// #include <romusarival/main.hpp>
// ros::Subscriber* sub = NULL;
// globals
videoOutput *output = NULL;

detectNet *net = NULL;
uint32_t overlay_flags = detectNet::OVERLAY_NONE;

imageConverter *input_cvt = NULL;
imageConverter *overlay_cvt = NULL;
imageConverter *overlay_out = NULL;
// uchar4* d_resized;
uchar3 *d_resized = NULL;

std::vector<int> current_detection;
median_filter<float> median_xCenter(5);
median_filter<float> median_yCenter(5);

int dstWidth = 320;
int dstHeight = 240;

Publisher<sensor_msgs::Image> overlay_pub = NULL;

ros::Publisher object_detection_pub;

ros::Time last_msg_time;
romusa_messages::ObjectDetection object_detection_msg = romusa_messages::ObjectDetection();

void timerCallback(const ros::TimerEvent &)
{
  ros::Duration time_since_last_msg = ros::Time::now() - last_msg_time;

  if (time_since_last_msg.toSec() > 1.0)
  { // Timeout of 1 second
    ROS_WARN("No message received for over 1 second.");
  }
  object_detection_msg.center_x[0] = 0;
  object_detection_msg.center_y[0] = 0;
  object_detection_pub.publish(object_detection_msg);
}

// publish overlay image
bool publish_overlay(detectNet::Detection *detections, int numDetections)
{
  // std::cout << "publisher called" << std::endl;
  // get the image dimensions
  const uint32_t width = input_cvt->GetWidth();
  const uint32_t height = input_cvt->GetHeight();

  // assure correct image size
  if (!overlay_cvt->Resize(input_cvt->GetWidth(), input_cvt->GetHeight(), imageConverter::ROSOutputFormat))
    return false;

  // generate the overlay
  if (!net->Overlay(input_cvt->ImageGPU(), overlay_cvt->ImageGPU(), width, height, imageConverter::InternalFormat, detections, numDetections,
                    overlay_flags))
  {
    return false;
  }

  // populate the message
  sensor_msgs::Image msg;

  if (!overlay_cvt->Convert(msg, imageConverter::ROSOutputFormat))
    return false;

  // populate timestamp in header field
  msg.header.stamp = ROS_TIME_NOW();
  ROS_DEBUG("publishing %ux%u overlay image", width, height);

  // VIDEO OUTPUT DISPLAY TO USER camera 1
  output->Render(overlay_cvt->ImageGPU(), overlay_cvt->GetWidth(), overlay_cvt->GetHeight());
  cudaResize(overlay_cvt->ImageGPU(), overlay_cvt->GetWidth(), overlay_cvt->GetHeight(), overlay_out->ImageGPU(), 320, 240,
             imageConverter::ROSOutputFormat);
  cudaResize(overlay_cvt->ImageGPU(), overlay_cvt->GetWidth(), overlay_cvt->GetHeight(), d_resized, dstWidth, dstHeight,
             imageConverter::ROSOutputFormat);

  // overlay_cvt->SetFrom(d_resized, dstWidth, dstHeight, imageConverter::ROSOutputFormat);

  // output->Render(d_resized, dstWidth, dstHeight);
}

// input image subscriber callback
void img_callback(const sensor_msgs::Image::ConstPtr &input)
{
  // std::cout << "subscribe called" << std::endl;
  // convert the image to reside on GPU
  if (!input_cvt || !input_cvt->Convert(input))
  {
    ROS_INFO("failed to convert %ux%u %s image", input->width, input->height, input->encoding.c_str());
    return;
  }

  // classify the image
  // detectNet::Detection *detections = NULL;
  static detectNet::Detection *detections;
  static int counter = 0;
  static int numDetections = 0;

  if (counter++ > 2)
  {
    counter = 0;
    numDetections = net->Detect(input_cvt->ImageGPU(), input_cvt->GetWidth(), input_cvt->GetHeight(), &detections, detectNet::OVERLAY_NONE);
  }
  // verify success
  if (numDetections < 0)
  {
    ROS_ERROR("failed to run object detection on %ux%u image", input->width, input->height);
    return;
  }

  else if (numDetections == 0)
  {
    for (int i = 0; i < 8; i++)
    {
      object_detection_msg.class_id = 7; // 7 equal NULL class, detections, numDetections
      object_detection_msg.center_x[i] = 0;
      object_detection_msg.center_y[i] = 0;
      object_detection_msg.width[i] = 0;
      object_detection_msg.height[i] = 0;
    }
  }

  else
  {
    // clear vector for memory management
    current_detection.clear();
    // create a detection for each bounding box
    for (int n = 0; n < numDetections; n++)
    {
      detectNet::Detection *det = detections + n;
      current_detection.push_back(det->ClassID);

      // ROS_INFO("object %i class #%u (%s)  confidence=%f", n, det->ClassID, net->GetClassDesc(det->ClassID), det->Confidence);
      // ROS_INFO("object %i bounding box (%f, %f)  (%f, %f)  w=%f  h=%f", n, det->Left, det->Top, det->Right, det->Bottom, det->Width(), det->Height());
      float center_x[8];
      float center_y[8];
      // create a detection sub-message
      if (det->ClassID)
      {
        det->Center(&center_y[det->ClassID - 1], &center_x[det->ClassID - 1]); // x y reversed cause of camera is potrait
        object_detection_msg.class_id = det->ClassID - 1;
        object_detection_msg.unique_id = numDetections;
        object_detection_msg.confidence[det->ClassID - 1] = det->Confidence;
        object_detection_msg.width[det->ClassID - 1] = det->Width();
        object_detection_msg.height[det->ClassID - 1] = det->Height();
        object_detection_msg.center_x[det->ClassID - 1] = center_x[det->ClassID - 1];
        object_detection_msg.center_y[det->ClassID - 1] = center_y[det->ClassID - 1];

        // median_xCenter.update(center_x[det->ClassID - 1] ) ;
        // median_yCenter.update(center_y[det->ClassID - 1] );
        // object_detection_msg.center_x[det->ClassID - 1] = median_xCenter.get_out() ;
        // object_detection_msg.center_y[det->ClassID - 1] = median_yCenter.get_out() ;
        ROS_INFO("class#%u(%s) || pixel X: %.3f || pixel Y: %.3f", det->ClassID, net->GetClassDesc(det->ClassID), center_x[det->ClassID - 1], center_y[det->ClassID - 1]);
      }
      // publish the detection message
    }

    for (int i = 0; i < object_detection_msg.confidence.size(); i++)
    {
      // reset val in arr messages;
      if (std::find(current_detection.begin(), current_detection.end(), i + 1) == current_detection.end())
      {
        object_detection_msg.confidence[i] = 0;
        object_detection_msg.center_x[i] = 0;
        object_detection_msg.center_y[i] = 0;
        object_detection_msg.width[i] = 0;
        object_detection_msg.height[i] = 0;
      }
    }

    // generate the overlay (if there are subscribers)
    if (ROS_NUM_SUBSCRIBERS(overlay_pub) > 0)
    {
    }
  }
  object_detection_pub.publish(object_detection_msg);

  publish_overlay(detections, numDetections);

  last_msg_time = ros::Time::now();
}

int main(int argc, char **argv)
{
  output = videoOutput::Create("webrtc://@:8544/output");
  // output = videoOutput::Create(resource_str.c_str(), video_options);

  cudaMalloc(&d_resized, dstWidth * dstHeight * sizeof(uchar4));

  /*
   * create node instance
   */
  ROS_CREATE_NODE("detectnet");

  object_detection_pub = nh.advertise<romusa_messages::ObjectDetection>("object_detection", 1000);

  /*
   * retrieve parameters
   */
  std::string model_name = "ssd-mobilenet-v2";
  std::string model_path;
  std::string prototxt_path;
  std::string class_labels_path;

  std::string input_blob = DETECTNET_DEFAULT_INPUT;
  std::string output_cvg = DETECTNET_DEFAULT_COVERAGE;
  std::string output_bbox = DETECTNET_DEFAULT_BBOX;
  std::string overlay_str = "box,labels,conf";

  float mean_pixel = 0.0f;
  float threshold = DETECTNET_DEFAULT_THRESHOLD;

  ROS_DECLARE_PARAMETER("model_name", model_name);
  ROS_DECLARE_PARAMETER("model_path", model_path);
  ROS_DECLARE_PARAMETER("prototxt_path", prototxt_path);
  ROS_DECLARE_PARAMETER("class_labels_path", class_labels_path);
  ROS_DECLARE_PARAMETER("input_blob", input_blob);
  ROS_DECLARE_PARAMETER("output_cvg", output_cvg);
  ROS_DECLARE_PARAMETER("output_bbox", output_bbox);
  ROS_DECLARE_PARAMETER("overlay_flags", overlay_str);
  ROS_DECLARE_PARAMETER("mean_pixel_value", mean_pixel);
  ROS_DECLARE_PARAMETER("threshold", threshold);

  /*
   * retrieve parameters
   */
  ROS_GET_PARAMETER("model_name", model_name);
  ROS_GET_PARAMETER("model_path", model_path);
  ROS_GET_PARAMETER("prototxt_path", prototxt_path);
  ROS_GET_PARAMETER("class_labels_path", class_labels_path);
  ROS_GET_PARAMETER("input_blob", input_blob);
  ROS_GET_PARAMETER("output_cvg", output_cvg);
  ROS_GET_PARAMETER("output_bbox", output_bbox);
  ROS_GET_PARAMETER("overlay_flags", overlay_str);
  ROS_GET_PARAMETER("mean_pixel_value", mean_pixel);
  ROS_GET_PARAMETER("threshold", threshold);

  overlay_flags = detectNet::OverlayFlagsFromStr(overlay_str.c_str());
  //   model_name = "/home/bagas/ProgramRobot/ROS/PROG_PHYNIX_ROS1_Main/devel/lib/computer_vision2/ssd-mobilenet.onnx";
  // model_path = "./src/object_detect/src/ssd-mobilenet.onnx";
  model_path = "./src/detection_bawah/src/labelbaru.onnx";
  // model_path = "./src/detection_bawah/src/nasionalbawah1.onnx";

  class_labels_path = "./src/detection_bawah/src/labels.txt";
  mean_pixel = 0.f;
  threshold = 0.75f;
  input_blob = "input_0";
  //   output_blob = NULL;
  output_cvg = "scores";
  output_bbox = "boxes";
  //   maxBatchSize = DEFAULT_MAX_BATCH_SIZE;

  /*
   * load object detection network
   */
  if (model_path.size() > 0)
  {
    // create network using custom model paths
    net = detectNet::Create(prototxt_path.c_str(), model_path.c_str(), mean_pixel, class_labels_path.c_str(), threshold, input_blob.c_str(),
                            output_cvg.c_str(), output_bbox.c_str());

    // net->SetTracker(objectTrackerIOU::Create(3, 15, 0.5f));
  }
  else
  {
    // create network using the built-in model
    net = detectNet::Create(model_name.c_str());
  }

  if (!net)
  {
    ROS_ERROR("failed to load detectNet model");
    return 0;
  }

  /*
   * create the class labels parameter vector
   */
  //   std::hash<std::string> model_hasher;  // hash the model path to avoid collisions on the param server
  //   std::string model_hash_str = std::string(net->GetModelPath()) + std::string(net->GetClassPath());
  //   const size_t model_hash = model_hasher(model_hash_str);

  //   ROS_INFO("model hash => %zu", model_hash);
  //   ROS_INFO("hash string => %s", model_hash_str.c_str());

  // obtain the list of class descriptions
  //   std::vector<std::string> class_descriptions;
  //   const uint32_t num_classes = net->GetNumClasses();

  //   for (uint32_t n = 0; n < num_classes; n++) class_descriptions.push_back(net->GetClassDesc(n));

  // create the key on the param server
  //   std::string class_key = std::string("class_labels_") + std::to_string(model_hash);

  //   ROS_DECLARE_PARAMETER(class_key, class_descriptions);
  //   ROS_SET_PARAMETER(class_key, class_descriptions);

  // populate the vision info msg
  //   std::string node_namespace = ROS_GET_NAMESPACE();
  //   ROS_INFO("node namespace => %s", node_namespace.c_str());

  /*
   * create image converter objects
   */
  input_cvt = new imageConverter();
  overlay_cvt = new imageConverter();
  overlay_out = new imageConverter();

  if (!input_cvt || !overlay_cvt)
  {
    ROS_ERROR("failed to create imageConverter objects");
    return 0;
  }

  /*
   * advertise publisher topics
   */
  ROS_CREATE_PUBLISHER(sensor_msgs::Image, "overlay", 2, overlay_pub);

  /*
   * subscribe to image topicimg
   */
  //   auto img_sub = ROS_CREATE_SUBSCRIBER(sensor_msgs::Image, "image_in", 5, img_callback);
  ros::Subscriber sub = nh.subscribe("image_in", 1000000, img_callback);

  ros::Timer timer = nh.createTimer(ros::Duration(1), timerCallback); // Check every 0.1 seconds

  ROS_SPIN();

  /*
   * free resources
   */
  delete net;
  delete input_cvt;
  delete overlay_cvt;

  return 0;
}
