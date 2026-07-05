#include "autons.hpp"
#include "titanselect/titanselect.hpp"

// Registering an auton adds it to the on-screen selector. Order here is the
// order shown on the brain screen.

static ts::auton leftSeven("Left 7 Ball", autons::seven_ball_left);
static ts::auton rightSeven("Right 7 Ball", autons::seven_ball_right);
static ts::auton soloAwp("Solo Awp", autons::solo_awp);
// four_three_right / four_three_left are empty stubs — not registered until
// they actually do something (a registered empty auton = a dead selector button).
static ts::auton horizontalOffset("Horizontal Offset", autons::horizontal_offset);
static ts::auton verticalOffset("Vertical Offset", autons::vertical_offset);
static ts::auton pidTest("PID Test", autons::pid_test);
static ts::auton twoPhaseDemo("2-Phase Drive", autons::two_phase_drive_demo);
