#include "main.h"
#include "robot.hpp"
#include "autons.hpp"
#include "pros/rtos.hpp"
#include <cmath>

namespace autons {

// Spin 10 full turns, read residual drift, print the corrected offset.
// Set the printed value into k_horiz_tw_offset_in / k_vert_tw_offset_in.

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
			true);
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

	chassis.turnToHeading(90, 2000, {.direction = AngularDirection::CW_CLOCKWISE}, true);
	pros::delay(500);
	chassis.turnToHeading(0, 2000, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE}, true);
	pros::delay(500);
	chassis.moveToPoint(0, 24, 3000, {}, true);
	pros::delay(500);
	// X ~0, Y ~24 if tuned
}

} // namespace autons
