#include "main.h"
#include "robot.hpp"
#include "lemlib/chassis/trackingWheel.hpp"

pros::Controller master(pros::E_CONTROLLER_MASTER);

// ---- Drivetrain ----
pros::MotorGroup left_motors({-13, -14, 15}, pros::v5::MotorGears::blue);
pros::MotorGroup right_motors({-18, 17, 16}, pros::v5::MotorGears::blue);

lemlib::Drivetrain drivetrain(
	&left_motors,
	&right_motors,
	9.5,                        // track width (in)
	lemlib::Omniwheel::NEW_325, // wheel diameter
	420,                        // drive rpm
	6                           // horizontal drift
);

// ---- Intake ----
pros::Motor intake_motor_1(-12);
pros::Motor intake_motor_2(-19);

// ---- Pneumatics ----
pros::adi::Pneumatics wing('A', false);
pros::adi::Pneumatics tounge('C', false);
pros::adi::Pneumatics redirect('B', false);

// ---- Odometry ----
pros::Imu imu(1);

pros::Rotation vertical_sensor(11);
lemlib::TrackingWheel vertical_tracking_wheel(
	&vertical_sensor, lemlib::Omniwheel::NEW_2, k_vert_tw_offset_in);

pros::Rotation horizontal_sensor(-2);
lemlib::TrackingWheel horizontal_tracking_wheel(
	&horizontal_sensor, lemlib::Omniwheel::NEW_275_HALF, k_horiz_tw_offset_in);

lemlib::OdomSensors sensors(
	&vertical_tracking_wheel,
	nullptr,
	&horizontal_tracking_wheel,
	nullptr,
	&imu
);

// ---- Motion controllers ----
lemlib::ControllerSettings lateral_controller(
	11,  // proportional gain (kP)
	0,   // integral gain (kI)
	8,   // derivative gain (kD)
	3,   // anti windup
	1,   // small error range, in inches
	100, // small error range timeout, in ms
	3,   // large error range, in inches
	500, // large error range timeout, in ms
	0    // maximum acceleration (slew)
);

lemlib::ControllerSettings angular_controller(
	1.2, // proportional gain (kP)
	0,   // integral gain (kI)
	6,   // derivative gain (kD)
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
