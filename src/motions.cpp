#include "motions.hpp"
#include "robot.hpp"
#include "lemlib/util.hpp" // lemlib::angleError
#include "pros/rtos.hpp"
#include <cstdint>

namespace motions {

void drive_full_then_slow(float full_dist, float slow_dist, int slow_speed,
                          int timeout_ms, int full_speed, float heading_kp) {
	const lemlib::Pose start = chassis.getPose();
	const float target_heading = start.theta; // degrees
	const float total_dist = full_dist + slow_dist;
	const std::uint32_t t0 = pros::millis();

	while (pros::millis() - t0 < static_cast<std::uint32_t>(timeout_ms)) {
		const lemlib::Pose pose = chassis.getPose();
		const float traveled = start.distance(pose);
		if (traveled >= total_dist) {
			break;
		}

		const int throttle = (traveled < full_dist) ? full_speed : slow_speed;
		// Hold the starting heading. angleError returns degrees in (-180, 180].
		const float err = lemlib::angleError(target_heading, pose.theta, false);
		const int turn = static_cast<int>(heading_kp * err);

		chassis.arcade(throttle, turn, true); // true -> skip driver curve
		pros::delay(10);
	}

	chassis.arcade(0, 0, true);
}

} // namespace motions
