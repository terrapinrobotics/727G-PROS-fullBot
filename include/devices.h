#include "api.h"

#ifndef _DEVICES_H_
#define _DEVICES_H_

extern pros::Controller master;

/*
extern pros::Motor FL_mtr;
extern pros::Motor FR_mtr;
extern pros::Motor BL_mtr;
extern pros::Motor BR_mtr;
*/

extern pros::Motor_Group flywheel;

extern pros::Motor intake;

extern pros::ADIDigitalOut indexer;
extern pros::ADIDigitalOut expansion_left;
extern pros::ADIDigitalOut expansion_right;

extern pros::Imu inertial;

#endif