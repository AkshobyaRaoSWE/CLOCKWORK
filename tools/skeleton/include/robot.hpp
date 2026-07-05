#pragma once

// Shared robot configuration: devices, subsystems, and the chassis.
// Definitions live in src/robot.cpp. Include this anywhere you need to
// drive the robot (autons, opcontrol, calibration).

#include "lemlib/api.hpp"
#include "titanselect/titanselect.hpp"
#include "pros/misc.hpp"
#include "pros/motors.hpp"

// Tracking wheel offsets (inches). Shared by odom config + calibration.
// TODO: tune with the Horizontal/Vertical Offset autons.
inline constexpr float k_vert_tw_offset_in = 0.0f;
inline constexpr float k_horiz_tw_offset_in = 0.0f;

// Input devices
extern pros::Controller master;

// Drive + odom
extern lemlib::Chassis chassis;

// Auton selector
extern ts::selector* selector;

// TODO: declare subsystems here, e.g.
// extern pros::Motor intake;
// extern pros::adi::Pneumatics wing;
