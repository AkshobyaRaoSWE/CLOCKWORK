#include "main.h"
#include "robot.hpp"
#include "autons.hpp"
#include "pros/rtos.hpp"

namespace autons {

void seven_ball_right() {
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
	chassis.setPose(0, 0, 0);
	intake_motor_1.move(127);
	intake_motor_2.move(127);
	wing.extend();
	chassis.moveToPoint(9.5, 43, 900, {.maxSpeed = 90, .minSpeed = 20});
	pros::delay(800);
	tounge.extend();
	chassis.turnToHeading(24, 500);
	chassis.turnToPoint(27, 14, 500);
	chassis.moveToPoint(27, 14, 1000, {.maxSpeed = 90});
	chassis.turnToPoint(30, 1, 500);
	chassis.moveToPoint(30, 1, 800, {.maxSpeed = 90});
	chassis.waitUntilDone();
	pros::delay(200);
	chassis.moveToPoint(32, 35, 2500, {.forwards = false, .maxSpeed = 45});
	chassis.waitUntilDone();
	wing.retract();
	intake_motor_1.move(-127);
	intake_motor_2.move(-127);
	pros::delay(300);
	intake_motor_1.move(127);
	intake_motor_2.move(127);
	pros::delay(1000);
	chassis.moveToPose(23, 25, 214, 900, {.lead = 0.5, .maxSpeed = 90});
	chassis.turnToHeading(172, 500);
	chassis.moveToPoint(23, 50, 800, {.forwards = false, .maxSpeed = 90});
}

void seven_ball_left() {
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
	chassis.setPose(0, 0, 0);
	intake_motor_1.move(127);
	intake_motor_2.move(127);
	wing.extend();
	chassis.moveToPoint(-11, 33, 900, {.maxSpeed = 90});
	pros::delay(800);
	tounge.extend();
	chassis.turnToHeading(-24, 500, {}, true);
	chassis.turnToHeading(-120, 500, {}, false);
	chassis.moveToPoint(-32, 15, 800, {.maxSpeed = 90}, false);
	chassis.turnToHeading(-177, 500, {}, true);
	chassis.moveToPoint(-38, 5, 800, {.maxSpeed = 90}, false);
	chassis.waitUntilDone();
	pros::delay(200);
	chassis.moveToPoint(-35, 33, 1300, {.forwards = false, .maxSpeed = 55}, false);
	chassis.waitUntilDone();
	wing.retract();
	tounge.retract();
	intake_motor_1.move(-127);
	intake_motor_2.move(-127);
	pros::delay(300);
	intake_motor_1.move(127);
	intake_motor_2.move(127);
	pros::delay(1000);
	chassis.moveToPose(-43, 20, -185, 900, {.lead = 0.5}, false);
	chassis.turnToHeading(-172, 500, {}, false);
	chassis.moveToPoint(-43, 55, 800, {.forwards = false}, false);
}

void solo_awp() {
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
	chassis.setPose(0, 0, 0);
	wing.extend();
	intake_motor_1.move(127);
	intake_motor_2.move(127);
	tounge.extend();
	chassis.moveToPoint(0, 45, 2000, {.maxSpeed = 80});
	chassis.waitUntilDone();
	chassis.turnToHeading(93, 500);
	chassis.moveToPoint(14, 47, 800, {.forwards = true, .maxSpeed = 75});
	chassis.waitUntilDone();
	pros::delay(450);
	chassis.moveToPoint(-35, 47.5, 1500, {.forwards = false, .maxSpeed = 55});
	chassis.waitUntilDone();
	wing.retract();
	chassis.waitUntilDone();
	pros::delay(1000);
	tounge.retract();
	chassis.turnToHeading(201, 500);
	chassis.waitUntilDone();
	wing.extend();
	chassis.moveToPoint(-22, 25, 1000, {.maxSpeed = 100});
	chassis.turnToHeading(174, 500);
	chassis.moveToPoint(-21, -25, 1000, {.maxSpeed = 80});
	chassis.turnToHeading(133, 500);
	chassis.moveToPoint(-37, -13, 800, {.forwards = false});
	chassis.waitUntilDone();
	redirect.extend();
	chassis.waitUntilDone();
	pros::delay(700);
	chassis.moveToPoint(2, -53, 1100, {.forwards = true, .maxSpeed = 80});
	chassis.turnToHeading(90, 500);
	redirect.retract();
	chassis.moveToPoint(14, -52, 900, {.forwards = true});
	chassis.moveToPoint(-17, -53.5, 1300, {.forwards = false, .maxSpeed = 55});
	wing.retract();
}

void four_three_right() {
}

void four_three_left() {
}

} // namespace autons
