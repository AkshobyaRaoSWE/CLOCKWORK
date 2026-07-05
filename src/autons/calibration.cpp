#include "main.h"
#include "robot.hpp"
#include "autons.hpp"
#include "clockwork/clockwork.hpp"
#include "pros/rtos.hpp"
#include <cmath>

namespace autons {

void horizontal_offset() {
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
	chassis.setPose(0, 0, 0);

	for (int i = 0; i < 10; i++) {
		chassis.turnToHeading(360 * (i + 1), 2000,
			{.direction = AngularDirection::CW_CLOCKWISE},
			true);
		pros::delay(100);
	}

	pros::delay(500); // settle

	lemlib::Pose pose = chassis.getPose();
	pros::lcd::print(0, "=== HORIZ OFFSET TEST ===");
	pros::lcd::print(1, "X drift:  %.4f in", pose.x);
	pros::lcd::print(2, "Y drift:  %.4f in", pose.y);
	pros::lcd::print(3, "Heading:  %.2f deg", pose.theta);
	pros::lcd::print(4, "---");
	pros::lcd::print(5, "correction = -%.4f",
		pose.x / (2 * M_PI * 10));
	pros::lcd::print(6, "new_horiz_offset = %.4f",
		k_horiz_tw_offset_in - pose.x / (2 * M_PI * 10));
}

void vertical_offset() {
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
	chassis.setPose(0, 0, 0);

	for (int i = 0; i < 10; i++) {
		chassis.turnToHeading(360 * (i + 1), 2000,
			{.direction = AngularDirection::CW_CLOCKWISE},
			true); // blocking
		pros::delay(100);
	}

	pros::delay(500);

	lemlib::Pose pose = chassis.getPose();
	pros::lcd::print(0, "=== VERT OFFSET TEST ===");
	pros::lcd::print(1, "X drift:  %.4f in", pose.x);
	pros::lcd::print(2, "Y drift:  %.4f in", pose.y);
	pros::lcd::print(3, "Heading:  %.2f deg", pose.theta);
	pros::lcd::print(4, "---");
	pros::lcd::print(5, "correction = -%.4f",
		pose.y / (2 * M_PI * 10));
	pros::lcd::print(6, "new_vert_offset = %.4f",
		k_vert_tw_offset_in - pose.y / (2 * M_PI * 10));
}

void pid_test() {
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
	chassis.setPose(0, 0, 0);

	// Turn 90 and check X/Y don't move
	chassis.turnToHeading(90, 2000, {.direction = AngularDirection::CW_CLOCKWISE}, true);
	pros::delay(500);
	// X should be ~0, Y should be ~0

	// Turn back
	chassis.turnToHeading(0, 2000, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE}, true);
	pros::delay(500);
	// Still ~0, ~0

	// Now drive straight and check it actually goes straight
	chassis.moveToPoint(0, 24, 3000, {}, true);
	pros::delay(500);
	// X should be ~0, Y should be ~24
}

void two_phase_drive_demo() {
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
	chassis.setPose(0, 0, 0);
	// Full speed for 24 in, then coast at 40 power for the next 12 in.
	clockwork::Motion motion(&chassis);
	motion.driveFullThenSlow(24, 12, 40, 3000);
}

} // namespace autons
