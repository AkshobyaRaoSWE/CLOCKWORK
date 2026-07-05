#include "main.h"
#include "robot.hpp"
#include "autons.hpp"
#include "pros/rtos.hpp"

namespace autons {

// Starter routine. Copy this to build your real autons.
void example() {
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
	chassis.setPose(0, 0, 0);
	chassis.moveToPoint(0, 24, 2000);
	chassis.turnToHeading(90, 1000);
	chassis.waitUntilDone();
}

} // namespace autons
