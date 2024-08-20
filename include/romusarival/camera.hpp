#ifndef ROMUSARIVAL_CAMERA_HPP
#define ROMUSARIVAL_CAMERA_HPP

#include "romusarival/main.hpp"
#include "romusarival/sequence.hpp"
#include <iostream>
#include <cmath>

void PID_cam();
void scan_cam_atas(float pix_cam_y, float tar[2], float set_speed[2], float sp_trac); // 0: y 1: w
void scan_cam_atas_mirror(float pix_cam_y, float tar[2], float set_speed[2], float sp_trac); // 0: y 1: w

#endif