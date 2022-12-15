#include "main.h"
#include "autons.h"

void delay_until_target(pros::Motor_Group& motor, int target) {
    while (!((motor.get_positions()[0] < target + 5) && (motor.get_positions()[0] > target - 5))) {
        pros::delay(20);
    }
}

void delay_until_target(pros::Motor motor, int target) {
    while (!((motor.get_position() < target + 5) && (motor.get_position() > target - 5))) {
        pros::delay(20);
    }
}

void spin_intake(int units, int velocity) {
    intake.move_relative(units, velocity);
    delay_until_target(intake, units);
}

void spin_flywheel(int msec) {
    flywheel.move_voltage(12000);
    pros::delay(msec);
    flywheel.brake();
}
