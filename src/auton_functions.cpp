#include "main.h"
#include "autons.h"

using namespace okapi::literals;

std::shared_ptr<okapi::ChassisController> chassis;

const double lin_kP = 0.05;
const double lin_kI = 0;
const double lin_kD = 0;

const double ang_kP = 0;
const double ang_kI = 0;
const double ang_kD = 0;

void autons::init() {
    chassis = okapi::ChassisControllerBuilder()
        .withMotors({-4, -1}, {20, 2})
        .withGains(
            {lin_kP, lin_kI, lin_kD}, 
            {ang_kP, ang_kI, ang_kD}
        )
        .withDimensions(okapi::AbstractMotor::gearset::green, {{4_in, 11.25_in}, okapi::imev5GreenTPR})
        .build();
}

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
