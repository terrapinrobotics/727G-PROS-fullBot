#include "main.h"
#include "autons.h"

void delay_until_target(pros::Motor_Group& motor, int target) {
    while (!((motor.get_positions()[0] < target + 5) && (motor.get_positions()[0] > target - 5))) {
        pros::delay(20);
    }
}

void auton_functions::spin_intake(int units, int velocity) {
    intake.move_relative(units, velocity);
    delay_until_target(intake, units);
}