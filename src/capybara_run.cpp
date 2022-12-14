#include "ARMS/flags.h"
#include "autons.h"
#include "main.h"


void autons::capybara_run() {
    // guide on ARMS:
	// flags: ASYNC (non-blocking)
	// THRU (no PID controller)
	// RELATIVE (relative to current position)
	// REVERSE (reverse robot heading when moving)
	// combine with |
	// example: arms::chassis::move({5, 8}, ASYNC | THRU); (coord (5, 8), async, no PID)

    arms::chassis::move({0, 0, 90}, 127, 100); // up exit error to fix?
    //arms::chassis::turn(90);
    for (int i=0; i<4; i++) {
        arms::chassis::move(840, arms::RELATIVE);
        arms::chassis::turn(-90, arms::RELATIVE);
    }


    // start of real auton

    /*
    arms::chassis::move(-10);
    auton_functions::spin_intake(100, 100);
    */
}