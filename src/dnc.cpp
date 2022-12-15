// Here we go...
#include "devices.h"
#include "autons.h"

// Here are our turning constants!
#define FL_TURN_CONST 0.0052083
#define FR_TURN_CONST -0.0052083
#define BL_TURN_CONST 0.0052083
#define BR_TURN_CONST -0.0052083

// acceleration and speed parameters (defaults)
#define ACCELERATION 5
#define SPEED 5

void turnTo(float desired_heading, unsigned int speed, unsigned int ACC_PARAM) {

    pros::Motor FL_mtr = pros::Motor(4, pros::motor_gearset_e_t::E_MOTOR_GEARSET_18, true);
	pros::Motor FR_mtr = pros::Motor(20, pros::motor_gearset_e_t::E_MOTOR_GEARSET_18, false);
	pros::Motor BL_mtr = pros::Motor(1, pros::motor_gearset_e_t::E_MOTOR_GEARSET_18, true);
	pros::Motor BR_mtr = pros::Motor(2, pros::motor_gearset_e_t::E_MOTOR_GEARSET_18, false);

	//set brake modes
	FL_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	FR_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	BL_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	BR_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  // This function will determine the correct direction to turn, and the speed
  // to turn at unsigned integers are better because they take less
  // computational power on the brain

  // FORMAT OF CALL : turnTo(desired_heading, speed)
  // INPUTS         : desired_heading -> the desired global heading to which the
  //                  robot should turn in degrees on the range [0,360]
  //                  speed -> the turning speed as a positive % value
  // OUTPUTS        : void
  // EFFECTS        : robot will turn to specified heading at the specified
  //                speed

  // determine the necesary change in heading (dH)
  // the d repreents delta, or change, while H represents heading.
  // dH -> change in heading
  float dH = desired_heading - (360 - inertial.get_heading());

  // Define parameter turnSpeed which represents the real time maximum as
  // calculated by the acceleration format
  int turnSpeed = 0;

  // maximum speed a wheel can be
  float maxSpeed = 0;

  while (fabs(dH) >= 1.5) {
    // determine if the magnitude of dH is greater than 180
    dH = desired_heading - (360 - inertial.get_heading());
    while (fabs(dH) > 180) {
      // if so, determine i++f positive or negative
      if (dH > 0) {
        // if positive, subtract 360
        dH -= 360;
      }

      else {
        // else, add 360
        dH += 360;
      }
    }

    // 0.010 is 10ms delay
    float decel_limit = (pow(turnSpeed*200/100, 2) * 1/2 / ACC_PARAM / 60 / 200);

    // dH now represents the sign and magnitude of the fastest turn. Pos
    // values indicate a counterclockwise turn, while negative values
    // indicate a clockwise turn

    // using trig, geometry, and algebra, we can write an equation to
    // represent the linear relationship between desired turning angle
    // in degrees and wheel rotations.
    float FL_speed = FL_TURN_CONST * dH;
    float FR_speed = FR_TURN_CONST * dH;
    float BL_speed = BL_TURN_CONST * dH;
    float BR_speed = BR_TURN_CONST * dH;

    float speedArray[4] = {FL_speed, FR_speed, BL_speed, BR_speed};
    for (int i = 0; i < 4; i++) {
      if (fabs(speedArray[i]) > maxSpeed) {
        maxSpeed = fabs(speedArray[i]);
      }
    }

    // Minimum distance from target that will allow the robot to deccelerate
    // at the desired ACC_PARAM
    if (maxSpeed < fabs(decel_limit)) {
      turnSpeed -= ACC_PARAM;
    }

    else {
      // if turnSpeed >= 100, turnSpeed = speed
      // else turnSpeed += acceleration param
      turnSpeed = turnSpeed >= speed ? speed : turnSpeed + ACC_PARAM;
    }

    FL_mtr.move(FL_speed * turnSpeed * 3.64);
    FR_mtr.move(FR_speed * turnSpeed * 3.64);
    BL_mtr.move(BL_speed * turnSpeed * 3.64);
    BR_mtr.move(BR_speed * turnSpeed * 3.64);

    // wait to avoid brain overheat
    pros::delay(10);
  }

  // brake all
  FL_mtr.brake();
  FR_mtr.brake();
  BL_mtr.brake();
  BR_mtr.brake();
}

void dnc::turn (float desired_heading, bool isFieldTurn = false) {
  if (isFieldTurn) turnTo(desired_heading, 50, 5);
  else turnTo(inertial.get_heading() + desired_heading, 50, 5);
}