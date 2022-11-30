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

	// set motor brake modes
	/*
	FL_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	FR_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	BL_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	BR_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	*/

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
	arms::chassis::turn(90);
}

void opcontrol() {
	// to avoid using a task for piston delays, we'll have a variable
	// that keeps track of how many ms have ellapsed since the last
	// time we pressed the button

	uint32_t last_press_indexer = 0;
	bool indexer_state = false;

	// expansions [a, b]
	uint32_t last_press_expansions[2] = {0, 0};
	bool expansion_state[2] = {false, false};

	while (true) {

		// fire discs when L1!
        if (master.get_digital(DIGITAL_L1)) {
			indexer.set_value(1);
			indexer_state = true;
			last_press_indexer = pros::millis();
		}

		if (indexer_state && pros::millis() - last_press_indexer > 100) {
			// if >100 ms have passed
			indexer.set_value(0);
			indexer_state = false;
		}

		// expansion pneumatics
        if (master.get_digital(DIGITAL_A)) {
			expansion_left.set_value(1);
			expansion_state[0] = true;
			last_press_expansions[0] = pros::millis();
		}

		if (master.get_digital(DIGITAL_X)) {
			expansion_right.set_value(1);
			expansion_state[1] = true;
			last_press_expansions[1] = pros::millis();
		}

		if (expansion_state[0] && pros::millis() - last_press_expansions[0] > 100) {
			// if >100 ms have passed
			expansion_left.set_value(0);
			expansion_state[0] = false;
		}

		if (expansion_state[1] && pros::millis() - last_press_expansions[1] > 100) {
			// if >100 ms have passed
			expansion_right.set_value(0);
			expansion_state[1] = false;
		}

		// flywheel
		if (master.get_digital(DIGITAL_L2)) {
			flywheel = FLYWHEEL_SPEED;
		} else {
			flywheel.brake();
		}

		// intake
		if (master.get_digital(DIGITAL_R2)) {
			intake = 100;
		} else if (master.get_digital(DIGITAL_R1)) {
			intake = -100;
		} else {
			intake.brake();
		}

		// drive!
		int axis1 = master.get_analog(ANALOG_RIGHT_X);
		int axis3 = master.get_analog(ANALOG_LEFT_Y);
		int axis4 = master.get_analog(ANALOG_LEFT_X);

		/*
		FL_mtr = (axis3 + axis1 + axis4);
		BL_mtr = (axis3 + axis1 - axis4);
		FR_mtr = (axis3 - axis1 - axis4);
		BR_mtr = (axis3 - axis1 + axis4);
		*/

		pros::delay(20);
	}
}
