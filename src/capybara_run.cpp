#include "autons.h"
#include "main.h"

using namespace okapi::literals;

void autons::capybara_run() {
    // guide on ARMS:
	// flags: ASYNC (non-blocking)
	// THRU (no PID controller)
	// RELATIVE (relative to current position)
	// REVERSE (reverse robot heading when moving)
	// combine with |
	// example: arms::chassis::move({5, 8}, ASYNC | THRU); (coord (5, 8), async, no PID)

    //chassis->moveDistance(10_in);
	//chassis->turnAngle(90_deg);
	dnc::turn(90, true);
    //chassis->moveDistance(-10_in);
}