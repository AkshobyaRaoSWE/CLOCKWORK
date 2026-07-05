#include "main.h"
#include "robot.hpp"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/odom.hpp" // lemlib::update()

// Robot config -> src/robot.cpp
// Auton routines -> src/autons/*.cpp
// Driver control -> src/opcontrol.cpp

void on_center_button() {}

void initialize() {
	pros::lcd::initialize();
	chassis.calibrate();

	// odom update loop
	pros::Task([] {
		while (true) {
			lemlib::update();
			pros::delay(10);
		}
	});

	selector->display();

	// pose readout
	pros::Task screen_task([&]() {
		while (true) {
			pros::lcd::print(0, "X: %f", chassis.getPose().x);
			pros::lcd::print(1, "Y: %f", chassis.getPose().y);
			pros::lcd::print(2, "Theta: %f", chassis.getPose().theta);
			pros::delay(50);
		}
	});
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	selector->run_selected_auton();
}
