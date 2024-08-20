/*
 * Copyright (c) 2020, NVIDIA CORPORATION. All rights reserved.
 * DIMODIF OLEH HARES
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

// #include <jetson-utils/videoOutput.h>
#include <jetson-utils/videoSource.h>

#include <iostream>

#include "commandLine.h"

#include "detectNet.h"
#include <jetson-inference/objectTrackerIOU.h>

#include "filesystem.h"
#include "image_converter.h"
#include "iostream"
#include "logging.h"
#include "modelDownloader.h"
#include "objectTracker.h"
#include "ros_compat.h"
#include "tensorConvert.h"
#include "videoOutput.h"
#include "videoSource.h"

// globals
videoSource* stream = NULL;
videoOutput* stream2 = videoOutput::Create(NULL);
imageConverter* image_cvt = NULL;
// Publisher<sensor_msgs::Image> image_pub = NULL;
ros::Publisher image_pub;

// aquire and publish camera frame
bool aquireFrame() {
  imageConverter::PixelType* nextFrame = NULL;

  // get the latest frame
  if (!stream->Capture(&nextFrame, 1000)) {
    ROS_ERROR("failed to capture next frame");
    return false;
  }

  // assure correct image size
  if (!image_cvt->Resize(stream->GetWidth(), stream->GetHeight(), imageConverter::ROSOutputFormat)) {
    ROS_ERROR("failed to resize camera image converter");
    return false;
  }

  // populate the message
  sensor_msgs::Image msg;

  if (!image_cvt->Convert(msg, imageConverter::ROSOutputFormat, nextFrame)) {
    ROS_ERROR("failed to convert video stream frame to sensor_msgs::Image");
    return false;
  }

  // populate timestamp in header field
  msg.header.stamp = ROS_TIME_NOW();

  // publish the message
  image_pub.publish(msg);

  ROS_DEBUG("published %ux%u video frame", stream->GetWidth(), stream->GetHeight());

  // // render the image
  // stream2->Render(image_cvt->ImageGPU(), image_cvt->GetWidth(), image_cvt->GetHeight());

  // //   // update status bar
  // char str[256];
  // sprintf(str, "%s (%ux%u) | %.1f FPS", "test", image_cvt->GetWidth(), image_cvt->GetHeight(), stream->GetFrameRate());
  // stream2->SetStatus(str);

  return true;
}

#include <jetson-inference/detectNet.h>

#include <unordered_map>

// globals
detectNet* net = NULL;

uint32_t overlay_flags  = detectNet::OVERLAY_NONE;

imageConverter* input_cvt = NULL;

imageConverter* overlay_cvt = NULL;

Publisher<sensor_msgs::Image> overlay_pub = NULL;

// triggered when a new subscriber connected

// publish overlay image
bool publish_overlay(detectNet::Detection* detections, int numDetections) {
  // get the image dimensions
  const uint32_t width = input_cvt->GetWidth();
  const uint32_t height = input_cvt->GetHeight();

  // assure correct image size
  if (!overlay_cvt->Resize(width, height, imageConverter::ROSOutputFormat)) 
    return false;

  // generate the overlaydf turbine
  if (!net->Overlay(input_cvt->ImageGPU(), overlay_cvt->ImageGPU(), width, height, imageConverter::InternalFormat, detections, numDetections, overlay_flags))
    return false;
  // populate the message
  sensor_msgs::Image msg;

  if (!overlay_cvt->Convert(msg, imageConverter::ROSOutputFormat)) 
    return false;

  // populate timestamp in header field
  msg.header.stamp = ROS_TIME_NOW();

  // publish the message
  overlay_pub->publish(msg);
  ROS_DEBUG("publishing %ux%u overlay image", width, height);
}



// // input image subscriber callback
// void img_callback(const sensor_msgs::ImageConstPtr input) {
//   //tracking 
//   net->SetTracker(objectTrackerIOU::Create(3, 15, 0.5f));
  
//   // convert the image to reside on GPU
//   if (!input_cvt || !input_cvt->Convert(input)) {
//     ROS_INFO("failed to convert %ux%u %s image", input->width, input->height, input->encoding.c_str());
//     return;
//   }

//   // classify the image
//   detectNet::Detection* detections = NULL;

//   const int numDetections = net->Detect(input_cvt->ImageGPU(), input_cvt->GetWidth(), input_cvt->GetHeight(), &detections, detectNet::OVERLAY_NONE);

//   // verify success
//   if (numDetections < 0) {
//     ROS_ERROR("failed to run object detection on %ux%u image", input->width, input->height);
//     return;
//   }

//   // if objects were detected, send out message
//   if (numDetections > 0) {
//     ROS_INFO("detected %i objects in %ux%u image", numDetections, input->width, input->height);

//     // create a detection for each bounding box

//     for (int n = 0; n < numDetections; n++) {
//       detectNet::Detection* det = detections + n;

//       ROS_INFO("object %i class #%u (%s)  confidence=%f", n, det->ClassID, net->GetClassDesc(det->ClassID), det->Confidence);
//       ROS_INFO("object %i bounding box (%f, %f)  (%f, %f)  w=%f  h=%f", n, det->Left, det->Top, det->Right, det->Bottom, det->Width(), det->Height());

//       // create a detection sub-message

//       // publish the detection message
//     }

//     // generate the overlay (if there are subscribers)
//     if (ROS_NUM_SUBSCRIBERS(overlay_pub) > 0)
//       publish_overlay(detections, numDetections);
//   }
// }

// node main loop
int main(int argc, char** argv) {
  /*
	 * create node instance
	 */
  ROS_CREATE_NODE("video_source_2");
  std::cout << "huh" << std::endl;

  /*
	 * declare parameters
	 */
  videoOptions video_options;

  std::string resource_str;
  std::string codec_str;
  std::string flip_str;

  int video_width = video_options.width;
  int video_height= video_options.height;
  int video_framerate = video_options.frameRate;
  int latency = video_options.latency;

  resource_str = "/dev/video1";

  ROS_DECLARE_PARAMETER("resource", resource_str);
  ROS_DECLARE_PARAMETER("codec", codec_str);
  // ROS_DECLARE_PARAMETER("width", video_width);
  // ROS_DECLARE_PARAMETER("height", video_height);
  // ROS_DECLARE_PARAMETER("framerate", video_options.frameRate);
  ROS_DECLARE_PARAMETER("loop", video_options.loop);
  ROS_DECLARE_PARAMETER("flip", flip_str);
  ROS_DECLARE_PARAMETER("latency", latency);

  video_width = 640;
  video_height = 640;
  video_framerate = 10;

  /*
	 * retrieve parameters
	 */
  //camera 1
  ROS_GET_PARAMETER("resource", resource_str);
  ROS_GET_PARAMETER("codec", codec_str);
  // ROS_GET_PARAMETER("width", video_width);
  // ROS_GET_PARAMETER("height", video_height);
  // ROS_GET_PARAMETER("framerate", video_options.frameRate);
  ROS_GET_PARAMETER("loop", video_options.loop);
  ROS_GET_PARAMETER("flip", flip_str);
  ROS_GET_PARAMETER("latency", latency);



  if (resource_str.size() == 0) {
    ROS_ERROR("resource param wasn't set - please set the node's resource parameter to the input device/filename/URL");
    return 0;
  }

  if (codec_str.size() != 0)
    video_options.codec = videoOptions::CodecFromStr(codec_str.c_str());

  if (flip_str.size() != 0)
    video_options.flipMethod = videoOptions::FlipMethodFromStr(flip_str.c_str());


  // video_widthX = 640;
  // video_heightX = 640;
  // video_optionsX.frameRate = 10;
  video_options.width = video_width;
  video_options.height = video_height;
  video_options.latency = latency;


  ROS_INFO("opening video source: %s", resource_str.c_str());

  /*
	 * open video source
	 */
  stream = videoSource::Create(resource_str.c_str(), video_options);

  if (!stream ) {
    ROS_ERROR("failed to open video source");
    return 0;
  }

  /*
	 * create image converter
	 */
  image_cvt = new imageConverter();

  if (!image_cvt) {
    ROS_ERROR("failed to create imageConverter");
    return 0;
  }

  /*
	 * advertise pubMakefile:126: recipe for target 'cmTC_fff5e/fast' failed
make: * [cmTC_fff5e/fast] Error 2isher topics
	 */
  // ROS_CREATE_PUBLISHER(sensor_msgs::Image, "image_in", 2, image_pub);
  image_pub = nh.advertise<sensor_msgs::Image>("image_in_2", 1000000);
  /*
	 * start the camera streaming
	 */
  if (!stream->Open()) {
    ROS_ERROR("failed to start streaming video source");
    return 0;
  }

  /*
	 * start publishing video frames
	 */
  while (ROS_OK()) {
    // stream = videoSource::Create(resource_str.c_str(), video_options);

    if (!aquireFrame()) {
      if (!stream->IsStreaming() ) {
        ROS_INFO("stream is closed or reached EOS, exiting node...");
        break;
      }
    }

    // detecting(argc, argv);
    if (ROS_OK())
      ROS_SPIN_ONCE();
  }

  /*
	 * free resources
	 */
  delete stream;
  delete image_cvt;

  return 0;
}