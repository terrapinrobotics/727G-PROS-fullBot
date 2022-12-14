#include "main.h"
#include "ARMS/chassis.h"
#include "arms/config.h"

#define FLYWHEEL_SPEED 127

//LV_IMG_DECLARE(stretched_dog);

/*void displayImage() {
  lv_obj_t * img1 = lv_img_create(lv_scr_act(), NULL);
  lv_img_set_src(img1, &stretched_dog);
  lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, 0);
}*/

void initialize() {
	// cedric kat and hailley are to blame for this
	//displayImage();

	// reset our inertial!
	inertial.reset();

	autons::init();

	//arms::init();


	// flywheel
	flywheel.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
	intake.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
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
	//arms::odom::reset({0, 0}, 0);
	autons::capybara_run();
}

void opcontrol() {
	// disable ARMS PID during opcontrol! (allows use of PROS motors)
	// this is a custom 727G function I added to ARMS for our specific use case
	//arms::chassis::chassisTaskEnabled = false;

	// create drive motors!
	pros::Motor FL_mtr = pros::Motor(4, pros::motor_gearset_e_t::E_MOTOR_GEARSET_18, true);
	pros::Motor FR_mtr = pros::Motor(20, pros::motor_gearset_e_t::E_MOTOR_GEARSET_18, false);
	pros::Motor BL_mtr = pros::Motor(1, pros::motor_gearset_e_t::E_MOTOR_GEARSET_18, true);
	pros::Motor BR_mtr = pros::Motor(2, pros::motor_gearset_e_t::E_MOTOR_GEARSET_18, false);

	//set brake modes
	FL_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	FR_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	BL_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	BR_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

	// to avoid using a task for piston delays, we'll have a variable
	// that keeps track of how many ms have ellapsed since the last
	// time we pressed the button

	uint32_t last_press_indexer = 0;
	bool indexer_state = false;

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
			expansion_right.set_value(1);
		}

		// flywheel
		if (master.get_digital(DIGITAL_L2)) {
			flywheel = FLYWHEEL_SPEED;
		} else {
			flywheel.brake();
		}

		// intake
		if (master.get_digital(DIGITAL_R2)) {
			intake.move_voltage(12000);
		} else if (master.get_digital(DIGITAL_R1)) {
			intake.move_voltage(-12000);
		} else {
			intake.brake();
		}

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
