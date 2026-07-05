#pragma once

// Autonomous routines. Each routine sets its own starting pose.
// Registered with the selector in src/autons/registry.cpp.

namespace autons {

// ---- Match routines ----
void example();

// ---- Calibration / tuning ----
void horizontal_offset();
void vertical_offset();
void pid_test();

} // namespace autons
