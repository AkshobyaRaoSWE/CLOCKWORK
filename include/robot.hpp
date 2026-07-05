#pragma once

// Shared robot configuration: devices, subsystems, and the chassis.
// Definitions live in src/robot.cpp. Include this anywhere you need to
// drive the robot (autons, opcontrol, calibration).

#include "lemlib/api.hpp"
#include "titanselect/titanselect.hpp"
#include "pros/adi.hpp"
#include "pros/misc.hpp"
#include "pros/motors.hpp"

// Tracking wheel offsets (inches). Shared by odom config + calibration.
inline constexpr float k_vert_tw_offset_in = -2.4352f;
inline constexpr float k_horiz_tw_offset_in = -3.045f;

// Input devices
extern pros::Controller master;

// Intake
extern pros::Motor intake_motor_1;
extern pros::Motor intake_motor_2;

// Pneumatics
extern pros::adi::Pneumatics wing;
extern pros::adi::Pneumatics tounge;
extern pros::adi::Pneumatics redirect;

// Drive + odom
extern lemlib::Chassis chassis;

// Auton selector
extern ts::selector* selector;
