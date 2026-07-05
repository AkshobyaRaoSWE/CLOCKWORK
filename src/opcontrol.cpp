#include "main.h"
#include "robot.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include <cstdint>

void opcontrol() {
	enum class IntakeDriver { None, L1, L2Fwd, R2, L2Out };
	std::uint32_t l1_press_ms = 0;
	std::uint32_t x_press_ms = 0;
	std::uint32_t r2_press_ms = 0;
	std::uint32_t l2_out_press_ms = 0;

	bool wing_sticky_extended = false;

	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);

	while (true) {
		int leftY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		int rightX = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

		chassis.arcade(leftY, rightX);

		const std::uint32_t now = pros::millis();
		bool l1 = master.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
		bool l2 = master.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
		bool r1 = master.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
		bool r2 = master.get_digital(pros::E_CONTROLLER_DIGITAL_X);
		bool x = master.get_digital(pros::E_CONTROLLER_DIGITAL_R2);

		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
			l1_press_ms = now;
		}
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
			x_press_ms = now;
		}
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
			r2_press_ms = now;
		}
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
			l2_out_press_ms = now;
		}

		// Highest press time wins; ties: R2 > L2 out > X (old L2) > L1
		IntakeDriver driver = IntakeDriver::None;
		std::uint32_t best_t = 0;
		int tie_pri = 0;
		auto consider = [&](bool held, IntakeDriver d, std::uint32_t t, int pri) {
			if (!held) {
				return;
			}
			if (t > best_t || (t == best_t && pri > tie_pri)) {
				best_t = t;
				driver = d;
				tie_pri = pri;
			}
		};
		consider(l1, IntakeDriver::L1, l1_press_ms, 1);
		consider(x, IntakeDriver::L2Fwd, x_press_ms, 2);
		consider(r2, IntakeDriver::R2, r2_press_ms, 4);
		consider(l2, IntakeDriver::L2Out, l2_out_press_ms, 3);

		if (driver == IntakeDriver::L2Out) {
			intake_motor_1.move(-127);
			intake_motor_2.move(-127);
		} else if (driver == IntakeDriver::L1 || driver == IntakeDriver::L2Fwd || driver == IntakeDriver::R2) {
			intake_motor_1.move(127);
			intake_motor_2.move(127);
		} else {
			intake_motor_1.move(0);
			intake_motor_2.move(0);
		}

		if (driver == IntakeDriver::L1) {
			wing_sticky_extended = true;
		} else if (driver == IntakeDriver::L2Fwd || driver == IntakeDriver::L2Out) {
			wing_sticky_extended = false;
		}

		if (driver == IntakeDriver::L1 || driver == IntakeDriver::L2Fwd) {
			redirect.retract();
		} else if (r2) {
			redirect.extend();
		}

		if (driver == IntakeDriver::L2Fwd || driver == IntakeDriver::L2Out) {
			wing.retract();
		} else if (driver == IntakeDriver::L1) {
			wing.extend();
		} else if (driver == IntakeDriver::R2) {
			// leave wing unchanged (R2 does not drive wing)
		} else if (wing_sticky_extended) {
			wing.extend();
		} else {
			wing.retract();
		}
		if (r1) {
			wing.extend();
		}

		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
			tounge.toggle();
		}

		pros::delay(10);
	}
}
