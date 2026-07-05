#pragma once

// Autonomous routines. Each routine assumes it starts a run and sets its own
// starting pose. Registered with the selector in src/autons/registry.cpp.

namespace autons {

// ---- Match routines ----
void seven_ball_left();
void seven_ball_right();
void solo_awp();
void four_three_left();
void four_three_right();

// ---- Calibration / tuning ----
void horizontal_offset();
void vertical_offset();
void pid_test();
void two_phase_drive_demo();

} // namespace autons
