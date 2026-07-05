#pragma once

// Reusable motion primitives built on top of the lemlib chassis.
// Definitions live in src/motions.cpp.

namespace motions {

// Drive straight with a two-phase speed profile: full speed for the first
// `full_dist` inches, then decelerate to `slow_speed` for the next
// `slow_dist` inches. Heading is held with a simple P controller so the
// robot tracks straight. Blocks until the total distance is covered or the
// timeout elapses, then stops the drive.
//
// full_dist   inches driven at `full_speed`
// slow_dist   inches driven at `slow_speed` after the full-speed phase
// slow_speed  reduced drive power, -127..127 (the "Z" speed)
// timeout_ms  safety cap; motion always ends by this time
// full_speed  power for the first phase, -127..127 (defaults to max)
// heading_kp  P gain (power per degree of heading error)
//
// For a reverse move, pass negative full_speed and slow_speed.
void drive_full_then_slow(float full_dist, float slow_dist, int slow_speed,
                          int timeout_ms, int full_speed = 127,
                          float heading_kp = 2.0f);

} // namespace motions
