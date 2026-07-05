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
│   ├── autons.hpp       # autonomous routine declarations
│   └── motions.hpp      # reusable motion primitives
├── src/
│   ├── main.cpp         # PROS entry points: initialize / autonomous / opcontrol wiring
│   ├── robot.cpp        # all device + chassis definitions (ports, PID, odom)
│   ├── opcontrol.cpp    # driver control
│   ├── motions.cpp      # motion primitive implementations
│   └── autons/
│       ├── registry.cpp # registers every routine with the selector (order = screen order)
│       ├── match.cpp    # competition routines
│       └── calibration.cpp # odom/PID calibration + test routines
├── clock.sh             # build / upload helper (uses the bundled PROS toolchain)
└── project.pros         # PROS project + template manifest
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
| 4+3 Right / Left | `four_three_*`              | Match autons (stubs / in progress)   |
| Horizontal Offset| `horizontal_offset`         | Calibrate horizontal tracking-wheel offset |
| Vertical Offset  | `vertical_offset`           | Calibrate vertical tracking-wheel offset   |
| PID Test         | `pid_test`                  | Sanity-check turn/drive PID          |
| 2-Phase Drive    | `two_phase_drive_demo`      | Demo of `drive_full_then_slow`       |

### Calibration workflow

`horizontal_offset` / `vertical_offset` spin the robot 10 full rotations and
print the residual drift plus a corrected offset to the brain screen. Copy the
printed value into `k_horiz_tw_offset_in` / `k_vert_tw_offset_in` in
[`include/robot.hpp`](include/robot.hpp). `pid_test` drives a known
turn-and-straight pattern so you can confirm the pose ends where expected.

---

## Motion primitives

[`include/motions.hpp`](include/motions.hpp) / [`src/motions.cpp`](src/motions.cpp):

### `drive_full_then_slow`

Drives straight with a two-phase speed profile — **full speed** for a first
distance, then **decelerates to a lower speed** for a second distance. Useful
for approaching a target fast but arriving under control. Heading is held with a
simple P controller so the robot tracks straight, and the driver curve is
bypassed.

```cpp
motions::drive_full_then_slow(
    float full_dist,     // inches driven at full_speed
    float slow_dist,     // inches driven at slow_speed after the full phase
    int   slow_speed,    // reduced power, -127..127 (the "slow" speed)
    int   timeout_ms,    // safety cap; motion always ends by this time
    int   full_speed = 127,   // power for the first phase, -127..127
    float heading_kp = 2.0f); // P gain, power per degree of heading error
```

Distance is measured from the pose at the call site, so the robot must have a
valid heading/pose (call `chassis.setPose(...)` first). Pass negative speeds to
run the profile in reverse.

```cpp
// Full speed for 24 in, then coast at 40 power for the next 12 in.
motions::drive_full_then_slow(24, 12, 40, 3000);
```

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
