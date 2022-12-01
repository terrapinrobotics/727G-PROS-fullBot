#include "main.h"
#include "autons.h"

void delay_until_target(pros::Motor motor, int target) {
    while (!((motor.get_position() < target + 5) && (motor.get_position() > target - 5))) {
        pros::delay(20);
    }
}

void auton_functions::spin_intake(int units, int velocity) {
    intake.move_relative(velocity, velocity);
    delay_until_target(intake, units);
}