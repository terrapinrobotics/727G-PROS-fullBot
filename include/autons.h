#ifndef _AUTONS_H_
#define _AUTONS_H_

#include "okapi/api.hpp"

namespace autons {
    // auton functions
    void init(void);
    void capybara_run(void);
}

void spin_intake(int, int);
void spin_flywheel(int);

extern std::shared_ptr<okapi::ChassisController> chassis;

#endif