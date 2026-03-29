# Arduino 2WD Obstacle-Avoidance Robot Car

A fully documented Arduino robot car that drives autonomously and avoids obstacles using an HC-SR04 ultrasonic sensor and an L298N dual H-bridge motor driver.

![Robot Car](docs/images/robot_overview.jpg)

---

## Table of Contents

- [Overview](#overview)
- [Hardware](#hardware)
- [Wiring](#wiring)
- [Repository Structure](#repository-structure)
- [Getting Started](#getting-started)
- [Configuration](#configuration)
- [How It Works](#how-it-works)
- [Testing](#testing)
- [Troubleshooting](#troubleshooting)
- [Future Improvements](#future-improvements)
- [License](#license)

---

## Overview

This robot uses a **differential drive** (tank-steer) chassis. Two independently powered rear wheels handle both propulsion and steering. A front swivel caster provides the third point of contact. The HC-SR04 sensor scans for obstacles ahead; when one is detected within the configured threshold, the robot stops, reverses, turns, and continues.

The firmware is structured as a **non-blocking state machine** using `millis()` — the main loop never calls `delay()` during normal operation, so the sensor is polled on every iteration regardless of which manoeuvre is in progress.

---

## Hardware

| Component | Qty | Notes |
|---|---|---|
| Arduino Uno R3 | 1 | ATmega328P, 5 V logic |
| L298N Motor Driver Module | 1 | Dual H-bridge, up to 2 A/channel |
| HC-SR04 Ultrasonic Sensor | 1 | 2–400 cm range |
| TT DC Gear Motor (1:48) | 2 | 3–6 V, yellow housing |
| 65 mm Rubber Wheels | 2 | Yellow hub |
| Swivel Caster Wheel | 1 | Front support |
| MDF Robot Chassis | 1 | Two-layer laser-cut |
| 9 V Battery + Clip | 1 | Powers L298N and Arduino |
| Half-size Breadboard | 1 | For sensor connections |
| Jumper Wires | — | Male-to-male and male-to-female |
| M3 Standoffs & Screws | — | For mounting Arduino |

---

## Wiring

### Pin Map

| Arduino Pin | Connected To | Notes |
|---|---|---|
| **9** (PWM) | L298N ENA | Left motor speed |
| **8** | L298N IN1 | Left motor direction A |
| **7** | L298N IN2 | Left motor direction B |
| **6** (PWM) | L298N ENB | Right motor speed |
| **5** | L298N IN3 | Right motor direction A |
| **4** | L298N IN4 | Right motor direction B |
| **13** | HC-SR04 Trig | Trigger pulse output |
| **12** | HC-SR04 Echo | Echo pulse input |
| **5V** | HC-SR04 VCC, L298N VSS | Logic supply |
| **GND** | HC-SR04 GND, L298N GND | Common ground ⚠️ |

> ⚠️ **Critical:** Arduino GND, L298N GND, and battery negative **must** all be connected together. Missing this causes erratic behaviour.

### Power

```
9V Battery (+) ──── L298N VS terminal
9V Battery (–) ──── L298N GND ──── Arduino GND
L298N 5V output ──── Arduino 5V pin (when VS ≤ 12V, jumper present)
```

### L298N Direction Truth Table

| IN1 | IN2 | Motor A Behaviour |
|-----|-----|-------------------|
| HIGH | LOW | Forward |
| LOW | HIGH | Reverse |
| HIGH | HIGH | Brake |
| LOW | LOW | Coast |

*(Same logic applies to IN3/IN4 for Motor B)*

---

## Repository Structure

```
arduino-robot-car/
├── src/
│   ├── robot_car/              # Main sketch (upload this)
│   │   ├── robot_car.ino       # Main state-machine loop
│   │   ├── config.h            # All tunable constants in one place
│   │   ├── motors.h/.cpp       # L298N motor driver API
│   │   └── ultrasonic.h/.cpp   # HC-SR04 sensor API
│   ├── test_motors/            # Motor wiring verification sketch
│   │   └── test_motors.ino
│   └── test_ultrasonic/        # Sensor wiring verification sketch
│       └── test_ultrasonic.ino
├── docs/
│   ├── wiring_diagram.md       # Detailed wiring notes
│   └── images/                 # Build photos
├── schematics/
│   └── README.md               # Schematic notes
├── .gitignore
├── LICENSE
└── README.md
```

---

## Getting Started

### Prerequisites

- [Arduino IDE](https://www.arduino.cc/en/software) 1.8.x or 2.x
- No external libraries required — uses only the Arduino standard library

### Upload Steps

1. **Clone this repository:**
   ```bash
   git clone https://github.com/YOUR_USERNAME/arduino-robot-car.git
   ```

2. **Open the main sketch** in Arduino IDE:
   `src/robot_car/robot_car.ino`

3. **Select your board and port:**
   - Tools → Board → Arduino Uno
   - Tools → Port → (your COM port)

4. **Upload:** Click the Upload button or press `Ctrl+U`

5. **Open Serial Monitor** (Tools → Serial Monitor, 9600 baud) to watch live distance readings and state transitions.

> **Tip:** Run the motor test sketch first (`src/test_motors/test_motors.ino`) to verify wiring before uploading the main sketch.

---

## Configuration

All tunable parameters are in `src/robot_car/config.h`. No other files need to be edited for basic customisation.

```cpp
// How close (cm) before stopping
#define STOP_DISTANCE     30

// Motor speeds (0–255)
#define DRIVE_SPEED       180
#define REVERSE_SPEED     160
#define TURN_SPEED        150

// Manoeuvre durations (ms)
#define REVERSE_TIME_MS   400
#define TURN_TIME_MS      500
```

### Tuning Tips

| Parameter | Too Low | Too High |
|---|---|---|
| `STOP_DISTANCE` | Collides with obstacles | Stops far from obstacles; hesitant movement |
| `DRIVE_SPEED` | Slow; may stall on carpet | Fast; less reaction time |
| `TURN_TIME_MS` | Doesn't clear the obstacle | Over-turns; wastes time |

---

## How It Works

The firmware implements a **four-state reactive state machine**:

```
         obstacle detected
FORWARD ──────────────────► STOP
                              │ (200 ms pause)
                              ▼
                           REVERSING
                              │ (400 ms)
                              ▼
                           TURNING
                              │ (500 ms)
                              ▼
                           FORWARD ◄── (loop)
```

The state machine is driven by `millis()` timestamps rather than `delay()`, so the ultrasonic sensor is read on every iteration of `loop()` — the robot is never "blind" during a manoeuvre.

### Key Design Decisions

**Why non-blocking?**  
`delay()` freezes the CPU. If a new obstacle appears during a turn, a blocking implementation cannot react. The `millis()` approach reads the sensor every ~10 ms even during manoeuvres.

**Why mirror-mounted motor polarity?**  
The two motors face opposite directions on the chassis. "Forward" for the left motor (CW shaft rotation) corresponds to the opposite winding direction as "forward" for the right motor (CCW shaft rotation). This inversion is handled transparently inside `motors.cpp` — the rest of the firmware just calls `motors_forward()`.

---

## Testing

Two dedicated test sketches are included:

### Motor Test (`src/test_motors/`)
Tests each motor independently (forward/backward) and then combined manoeuvres. Upload and watch the Serial Monitor. If a motor spins in the wrong direction, swap its wires at the L298N screw terminals.

### Ultrasonic Test (`src/test_ultrasonic/`)
Prints live distance readings every 100 ms. Wave your hand in front of the sensor to verify readings change correctly.

---

## Troubleshooting

| Symptom | Cause | Fix |
|---|---|---|
| Robot spins in place | One motor wired backwards | Swap that motor's wire pair at L298N |
| Robot drifts left/right | Motor speed mismatch | Reduce `DRIVE_SPEED` on the faster side |
| Sensor always reads 999 | Echo pin disconnected | Check wiring; confirm Trig/Echo not swapped |
| Robot doesn't stop | `STOP_DISTANCE` too small | Increase to 40–50 cm and re-test |
| Arduino resets at motor start | PSU can't handle inrush | Add 100 µF capacitor across battery terminals |
| L298N gets very hot | Motor stall or over-voltage | Check for mechanical obstruction; check voltage |
| Erratic behaviour | Missing common ground | Connect Arduino GND to L298N GND |

---

## Future Improvements

- [ ] Bluetooth remote control via HC-05/HC-06 module
- [ ] IR line-following sensors for track navigation
- [ ] Wheel encoders for odometry and PID speed control
- [ ] Servo-mounted sensor for left/right scanning
- [ ] OLED display showing live distance and state
- [ ] Replace fixed turn time with gyroscope-measured heading

---

## License

This project is licensed under the MIT License — see [LICENSE](LICENSE) for details.
