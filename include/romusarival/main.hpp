#ifndef ROMUSARIVAL_MAIN_HPP
#define ROMUSARIVAL_MAIN_HPP

#include "ros/ros.h"

#include <romusa_messages/ObjectDetection.h>
#include <romusa_messages/odom.h>
#include <romusa_messages/cmd_vel.h>
#include <romusa_messages/cmd_slider.h>
#include <romusa_messages/vac.h>
#include <romusa_messages/enc_slider.h>
#include <romusa_messages/posGlob.h>
#include <romusa_messages/camatas.h>

#include "romusarival/config.hpp"

#include <stdint.h>

extern romusa_messages::cmd_vel vel_msg;
extern romusa_messages::cmd_slider slider_msg;
extern romusa_messages::vac vac_msg;
extern romusa_messages::posGlob posGlobalRos_msg;

extern ConfigParser PIDConf;
extern ConfigParser PointBlue;
extern ConfigParser PointRed;

struct JoyButton
{
  float axes[8];
  short buttons[11];
};

extern JoyButton jb;

#define MATI 0
#define SEDOT 0
#define SEBUL 1
#define NYALA 1

#define BIRU 0
#define MERAH 1

extern float RoboPose_Global[3];
extern float RoboSpeed[3];
extern short int posArm, posLifter;
extern bool start_robot;
extern bool retry_robot;
extern bool map_status;

extern float target[3], speed[3];
extern float RoboPose_Offset[3];
extern float pose_temp[3];
extern int FirstRun;
extern int timeRun;

extern float tar_pix_y;
extern float tar_cam[2];
extern float sp_cam[2];
extern float sp_speed_track;

extern uint8_t nomor_sampah;
extern int numDet;
extern float centerX;
extern float centerY;
extern float cm_center_x;
extern uint8_t last_detect;
extern uint8_t nomor_sampah_bawah;

extern short int offset_slider;
extern float target_new;
extern float err_cam;
extern float speed_trac;

extern float x_lidar_biru;
extern float y_lidar_biru;
extern float x_lidar_merah;
extern float y_lidar_merah;
extern float diff_lidar;

extern float localposX;
extern float localposY;
extern float prev_localposX;
extern float prev_localposY;
extern float localSpeedX;
extern float localSpeedY;
extern float Odom_Pose[3];
extern float Odom_Pose_Offset[3];
extern float Odom_Pose_temp[3];

extern uint8_t firstState;

extern bool prox_kanan, prox_kiri;
extern uint8_t Prediksi_Final;
extern uint8_t last_predict;


extern uint32_t time_delay;
extern uint32_t counter;

extern uint8_t status_pos;

#endif