#ifndef ROMUSARIVAL_MOVE_HPP
#define ROMUSARIVAL_MOVE_HPP

#include "romusarival/main.hpp"
#include <iostream>
#include <cmath>

void set_point(std::string case_move);
void move_x_biru(float SP_POS[3], float SP_SPEED[3]);
void move_y_biru(float sp_pos[3], float sp_speed[3]);
void move_odom(float sp_pos[3], float sp_speed[3]);
void move_lidar(float sp_pos[3], float sp_speed[3]);

void move_x_merah(float SP_POS[3], float SP_SPEED[3]);
void move_y_merah(float sp_pos[3], float sp_speed[3]);
void move_lidar_mirror(float sp_pos[3], float sp_speed[3]);

void input_move(float x, float y, float sudut, float speed_x, float speed_y, float speed_sudut);

#endif