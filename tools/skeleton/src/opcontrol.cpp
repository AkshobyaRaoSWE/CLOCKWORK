#include "main.h"
#include "robot.hpp"
#include "pros/rtos.hpp"

void opcontrol() {
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);

	while (true) {
		int leftY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		int rightX = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
		chassis.arcade(leftY, rightX);

		// TODO: subsystem controls, e.g.
		// if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) intake.move(127);

		pros::delay(10);
	}
}
