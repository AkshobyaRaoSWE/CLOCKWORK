#include "main.h"
#include "robot.hpp"
#include "lemlib/chassis/trackingWheel.hpp"

pros::Controller master(pros::E_CONTROLLER_MASTER);

// ---- Drivetrain ---- TODO: set ports, gearset, track width, wheel, rpm
pros::MotorGroup left_motors({-1, -2, 3}, pros::v5::MotorGears::blue);
pros::MotorGroup right_motors({4, 5, -6}, pros::v5::MotorGears::blue);

lemlib::Drivetrain drivetrain(
	&left_motors,
	&right_motors,
	11.5,                       // track width (in)
	lemlib::Omniwheel::NEW_325, // wheel diameter
	450,                        // drive rpm
	2                           // horizontal drift
);

// ---- Odometry ---- TODO: set sensor ports + wheel sizes
pros::Imu imu(10);

pros::Rotation vertical_sensor(20);
lemlib::TrackingWheel vertical_tracking_wheel(
	&vertical_sensor, lemlib::Omniwheel::NEW_2, k_vert_tw_offset_in);

pros::Rotation horizontal_sensor(-19);
lemlib::TrackingWheel horizontal_tracking_wheel(
	&horizontal_sensor, lemlib::Omniwheel::NEW_2, k_horiz_tw_offset_in);

lemlib::OdomSensors sensors(
	&vertical_tracking_wheel,
	nullptr,
	&horizontal_tracking_wheel,
	nullptr,
	&imu
);

// ---- Motion controllers ---- TODO: tune gains
lemlib::ControllerSettings lateral_controller(
	10,  // kP
	0,   // kI
	3,   // kD
	3,   // anti windup
	1,   // small error range, in inches
	100, // small error range timeout, in ms
	3,   // large error range, in inches
	500, // large error range timeout, in ms
	20   // maximum acceleration (slew)
);

lemlib::ControllerSettings angular_controller(
	2,   // kP
	0,   // kI
	10,  // kD
	3,   // anti windup
	1,   // small error range, in degrees
	100, // small error range timeout, in ms
	3,   // large error range, in degrees
	500, // large error range timeout, in ms
	0    // maximum acceleration (slew)
);

// ---- Driver input curves ----
lemlib::ExpoDriveCurve throttle_curve(3, 10, 1.019);
lemlib::ExpoDriveCurve steer_curve(3, 10, 1.019);

// ---- Chassis ----
lemlib::Chassis chassis(
	drivetrain,
	lateral_controller,
	angular_controller,
	sensors,
	&throttle_curve,
	&steer_curve
);

ts::selector* selector = ts::selector::get();
