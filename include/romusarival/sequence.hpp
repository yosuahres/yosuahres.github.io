#ifndef ROMUSARIVAL_SEQUENCE_HPP
#define ROMUSARIVAL_SEQUENCE_HPP

#include <iostream>
#include "romusarival/main.hpp"

typedef enum
{
    AFK,
    //target posisi
    START_MAJU,
    MAJU_LAGI,
    ROTATE,
    GET,
    SCAN,
    FIND_KIRI_INT,
    DETECT,
    FIND_KANAN,
    FIND_KIRI,
    MANUAL,
    DAUN,
    PLASTIK,
    FERRO,
    KERTAS,
    NON_FERRO,
    SCAN_BAWAH,
    AMBIL,
    //mekanisme
    ARM_KOTAK,
    LIFTER_NAIK_MEJA, 
    LIFTER_TURUN_MEJA, 
    LIFTER_TURUN_KOTAK, 
    LIFTER_NAIK_KOTAK,
    FIND_KANAN_INT,
    //ga kepake 
    STOP_BAWAH,
    STOP_ATAS,
    //
    STOP_KOTAK,
    RESTART_MUNDUR,
    RESTART,
    //lidar
    FERRO_LIDAR,
    KERTAS_LIDAR,
    NON_FERRO_LIDAR,
    PLASTIK_LIDAR,
    DAUN_LIDAR, 

} sequenceMove;

extern sequenceMove romusa_state;
extern sequenceMove tujuanKotak;


/*
1. Daun
2. Plastik
3. Ferro
4. Kertas
5. Non Ferro
*/

void romusa_sequence_biru();
void romusa_sequence_merah();

void kotak(uint8_t x, uint8_t predict);
void kotak_tes(uint8_t x);
void sensor(uint8_t get);

void sensor_camera(uint8_t cam, uint8_t sens);
std::string convToString(sequenceMove state);


#endif