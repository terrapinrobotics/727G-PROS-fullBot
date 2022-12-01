#include "main.h"
#include "arms/config.h"

#define FLYWHEEL_SPEED 100

LV_IMG_DECLARE(stretched_dog);

void displayImage() {
  lv_obj_t * img1 = lv_img_create(lv_scr_act(), NULL);
  lv_img_set_src(img1, &stretched_dog);
  lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, 0);
}

void initialize() {
	// cedric kat and hailley are to blame for this
	displayImage();

	// reset our inertial!
	inertial.reset();

	arms::init();

	// flywheel
	flywheel.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);

	// intake
	intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}


void autonomous() {
	autons::capybara_run();
}

void opcontrol() {

	// disable ARMS PID during opcontrol! (allows use of PROS motors)
	arms::pid::mode = DISABLE;

	// create drive motors!
	pros::Motor FL_mtr = pros::Motor(4, pros::motor_gearset_e_t::E_MOTOR_GEARSET_18, true);
	pros::Motor FR_mtr = pros::Motor(20, pros::motor_gearset_e_t::E_MOTOR_GEARSET_18, false);
	pros::Motor BL_mtr = pros::Motor(1, pros::motor_gearset_e_t::E_MOTOR_GEARSET_18, true);
	pros::Motor BR_mtr = pros::Motor(2, pros::motor_gearset_e_t::E_MOTOR_GEARSET_18, false);

	while (true) {
		// drive!
		int axis1 = master.get_analog(ANALOG_RIGHT_X);
		int axis3 = master.get_analog(ANALOG_LEFT_Y);
		int axis4 = master.get_analog(ANALOG_LEFT_X);

		FL_mtr = (axis3 + axis1 + axis4);
		BL_mtr = (axis3 + axis1 - axis4);
		FR_mtr = (axis3 - axis1 - axis4);
		BR_mtr = (axis3 - axis1 + axis4);

		pros::delay(20);
	}
}
