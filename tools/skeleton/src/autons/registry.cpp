#include "autons.hpp"
#include "titanselect/titanselect.hpp"

// Registering an auton adds it to the on-screen selector. Order here is the
// order shown on the brain screen.

static ts::auton example("Example", autons::example);
static ts::auton horizontalOffset("Horizontal Offset", autons::horizontal_offset);
static ts::auton verticalOffset("Vertical Offset", autons::vertical_offset);
static ts::auton pidTest("PID Test", autons::pid_test);
