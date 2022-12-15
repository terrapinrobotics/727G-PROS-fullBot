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

    arms::chassis::move(-220, arms::RELATIVE);
}