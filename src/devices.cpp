#include "devices.h"

pros::Controller master = pros::Controller(pros::E_CONTROLLER_MASTER);
pros::Motor FL_mtr = pros::Motor(4, true);
pros::Motor FR_mtr = pros::Motor(20, false);
pros::Motor BL_mtr = pros::Motor(1, true);
pros::Motor BR_mtr = pros::Motor(2, false);