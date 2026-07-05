# CLOCKWORK

Competition code for the **CLOCKWORK** VEX V5 robot (team 2360C), 2025–2026 season.
Built on [PROS](https://pros.cs.purdue.edu/) with [LemLib](https://lemlib.readthedocs.io/)
for odometry and motion, and [titanselect](https://github.com/) for on-brain
auton selection.

This repo is a single, cleanly-split PROS project that replaces the old
one-folder-per-autonomous layout: all routines live in one codebase and are
picked at match time from the brain's touchscreen.

---

## What it does

- **Field-centric odometry** — two dead-wheel tracking wheels + a V5 inertial
  sensor feed LemLib's odometry so the robot always knows its `(x, y, θ)` pose.
- **Closed-loop motion** — PID-controlled `moveToPoint` / `moveToPose` /
  `turnToHeading` for repeatable autonomous paths.
- **On-brain auton selector** — every routine is registered once and shown as a
  button on the brain screen; the selected one runs in `autonomous()`.
- **Driver control** — arcade drive with an exponential input curve, plus
  intake / wing / tongue / redirect handling on a most-recent-press-wins scheme.
- **Calibration tools** — built-in routines to measure tracking-wheel offsets
  and sanity-check the PID before a competition.

---

## Hardware

| Subsystem   | Devices                                                            |
|-------------|-------------------------------------------------------------------|
| Drivetrain  | 6× V5 motors (blue/600 rpm cart), 3 per side, on 3.25" omnis; 9.5" track width, geared to 420 rpm output |
| Intake      | 2× V5 motors (ports 12, 19)                                       |
| Pneumatics  | `wing` (ADI A), `tongue` (ADI C), `redirect` (ADI B)             |
| Odometry    | V5 IMU (port 1); vertical dead wheel (2", rotation port 11); horizontal dead wheel (2.75", rotation port 2) |

Ports and physical constants live in [`src/robot.cpp`](src/robot.cpp);
shared tuning constants (tracking-wheel offsets) are in
[`include/robot.hpp`](include/robot.hpp).

### Tuning (current values)

- **Lateral PID:** kP 11, kD 8
- **Angular PID:** kP 1.2, kD 6
- **Drive curve:** `ExpoDriveCurve(3, 10, 1.019)` on throttle and steer
- **Tracking-wheel offsets:** vertical −2.4352 in, horizontal −3.045 in

---

## Project layout

```
CLOCKWORK/
├── include/
│   ├── robot.hpp        # shared device/subsystem/chassis declarations + constants
│   └── autons.hpp       # autonomous routine declarations
├── src/
│   ├── main.cpp         # PROS entry points: initialize / autonomous / opcontrol wiring
│   ├── robot.cpp        # all device + chassis definitions (ports, PID, odom)
│   ├── opcontrol.cpp    # driver control
│   └── autons/
│       ├── registry.cpp # registers every routine with the selector (order = screen order)
│       ├── match.cpp    # competition routines
│       └── calibration.cpp # odom/PID calibration + test routines
├── clock.sh             # build / upload helper (uses the bundled PROS toolchain)
└── project.pros         # PROS project + template manifest
```

Motion primitives and the intake wrapper come from the **[clockwork](https://github.com/AkshobyaRaoSWE/clockwork-lib)**
PROS library (installed as a template). Install/upgrade it with:

```bash
pros c add-depot clockwork https://raw.githubusercontent.com/AkshobyaRaoSWE/clockwork-lib/main/depot.json
pros c apply clockwork
```

**To add an autonomous:** write the function in `src/autons/match.cpp`, declare
it in `include/autons.hpp`, and register it in `src/autons/registry.cpp`:

```cpp
static ts::auton myRoutine("My Routine", autons::my_routine);
```

It then appears as a button on the brain screen automatically.

---

## Autonomous routines

Registered in [`src/autons/registry.cpp`](src/autons/registry.cpp):

| Selector name    | Function                    | Purpose                              |
|------------------|-----------------------------|--------------------------------------|
| Left 7 Ball      | `seven_ball_left`           | Match auton, left side               |
| Right 7 Ball     | `seven_ball_right`          | Match auton, right side              |
| Solo Awp         | `solo_awp`                  | Solo autonomous win point            |
| Horizontal Offset| `horizontal_offset`         | Calibrate horizontal tracking-wheel offset |
| Vertical Offset  | `vertical_offset`           | Calibrate vertical tracking-wheel offset   |
| PID Test         | `pid_test`                  | Sanity-check turn/drive PID          |
| 2-Phase Drive    | `two_phase_drive_demo`      | Demo of `Motion::driveFullThenSlow`  |

> `four_three_right` / `four_three_left` are empty stubs and are **not**
> registered (a registered empty routine would show as a dead selector button).
> Fill them in, then add them to `registry.cpp`.

### Calibration workflow

`horizontal_offset` / `vertical_offset` spin the robot 10 full rotations and
print the residual drift plus a corrected offset to the brain screen. Copy the
printed value into `k_horiz_tw_offset_in` / `k_vert_tw_offset_in` in
[`include/robot.hpp`](include/robot.hpp). `pid_test` drives a known
turn-and-straight pattern so you can confirm the pose ends where expected.

---

## Motion + intake helpers

These come from the [clockwork](https://github.com/AkshobyaRaoSWE/clockwork-lib)
library. In autons, `chassis` and the `intake` roller are the shared globals
from `robot.cpp`.

```cpp
#include "clockwork/clockwork.hpp"

clockwork::Motion motion(&chassis);
motion.driveFullThenSlow(24, 12, 40, 3000); // full speed 24 in, then 40-power 12 in
motion.driveDistance(24, 100, 3000);        // straight 24 in, heading-held
motion.driveTimed(400, 60);                 // 60 power for 400 ms

intake.in();               // spin intake in    (was intake_motor_1/2.move(127))
intake.out();              // spin intake out
intake.stop();             // stop
intake.pulse(-127, 300);   // eject 300 ms, then stop
```

See the [library README](https://github.com/AkshobyaRaoSWE/clockwork-lib) for
the full API.

---

## Driver controls

Arcade drive: **left stick Y** = throttle, **right stick X** = turn.
Intake/pneumatics use a most-recent-press-wins scheme.

| Button | Effect                                                        |
|--------|---------------------------------------------------------------|
| L1     | Intake in; wing extends (stays out until an outtake action)   |
| R2     | Intake in; wing retracts                                      |
| X      | Intake in; redirect extends                                   |
| L2     | Intake out (reverse); wing retracts                           |
| R1     | Force wing extend                                             |
| Y      | Toggle tongue                                                 |

---

## Build & upload

Requires the PROS toolchain. `clock.sh` uses the CLI + ARM toolchain that ship
with the Cursor / VS Code **PROS extension**, so nothing extra needs to be on
your `PATH`:

```bash
./clock.sh          # build (pros make)
./clock.sh upload   # build + upload to a connected V5 brain
./clock.sh clean    # remove build artifacts
```

Or, with a normal PROS install:

```bash
pros make           # build
pros mu             # build + upload
```

You can also build/upload from the PROS extension's UI in your editor.

### Templates

The project vendors its PROS templates (LemLib, liblvgl, titanselect and the
kernel libs) under `firmware/` and `include/` so it builds standalone. These are
managed by PROS via `project.pros`.
