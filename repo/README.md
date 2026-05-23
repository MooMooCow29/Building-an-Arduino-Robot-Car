# Building an Arduino Robot Car

A fully documented, progressively developed Arduino robot car series covering open-loop control, PID closed-loop speed control, Ziegler-Nichols formal tuning, and feedforward control.

**Published in [Level Up Coding](https://levelup.gitconnected.com) on Medium**
**Author: [Karan Singh](https://medium.com/@ks683557) — Electrical Engineering, University of East Anglia**

---

## Article Series

| Version | Article | Key Concept |
|---|---|---|
| [v1](#v1--obstacle-avoidance-open-loop) | [Building an Arduino Robot Car from First Principles](https://medium.com/@ks683557) | Open-loop motor control |
| [v2](#v2--pid-closed-loop-speed-control) | [PID Control from First Principles](https://medium.com/@ks683557) | Closed-loop PID with encoders |
| [v3](#v3--ziegler-nichols-tuning) | [Ziegler-Nichols PID Tuning from First Principles](https://levelup.gitconnected.com/ziegler-nichols-pid-tuning-from-first-principles-the-method-that-brought-control-theory-out-of-the-0062c96dbaf3) | Formal systematic gain tuning |
| [v4](#v4--feedforward--pid) | [Feedforward Control: The Missing Half of Every PID Tutorial](https://medium.com/@ks683557) | Proactive baseline control |

---

## Hardware

| Component | Qty | Notes |
|---|---|---|
| Arduino Uno R3 | 1 | ATmega328P, 5V logic |
| L298N Motor Driver Module | 1 | Dual H-bridge, up to 2A/channel |
| HC-SR04 Ultrasonic Sensor | 1 | 2–400 cm range |
| TT DC Gear Motor (1:48) | 2 | 3–6V, yellow housing |
| Optical Wheel Encoder (20-slot) | 2 | For v2, v3, v4 only |
| 65mm Rubber Wheels | 2 | Yellow hub |
| Swivel Caster Wheel | 1 | Front support |
| MDF Robot Chassis | 1 | Two-layer laser-cut |
| 9V Battery + Clip | 1 | Powers L298N and Arduino |

---

## Pin Map

| Arduino Pin | Connected To | Used in |
|---|---|---|
| **9** (PWM) | L298N ENA — Left motor speed | All versions |
| **8** | L298N IN1 — Left motor dir A | All versions |
| **7** | L298N IN2 — Left motor dir B | All versions |
| **6** (PWM) | L298N ENB — Right motor speed | All versions |
| **5** | L298N IN3 — Right motor dir A | All versions |
| **4** | L298N IN4 — Right motor dir B | All versions |
| **13** | HC-SR04 Trig | All versions |
| **12** | HC-SR04 Echo | All versions |
| **2** (INT0) | Left wheel encoder signal | v2, v3, v4 |
| **3** (INT1) | Right wheel encoder signal | v2, v3, v4 |

> ⚠️ **Critical:** Arduino GND, L298N GND, and battery negative **must** all be connected together. Missing this causes erratic behaviour.

---

## Repository Structure

```
Building-an-Arduino-Robot-Car/
├── src/
│   ├── v1_obstacle_avoidance/     # Article 1 — open-loop, no encoders
│   │   ├── robot_car.ino
│   │   ├── config.h
│   │   ├── motors.h/.cpp
│   │   └── ultrasonic.h/.cpp
│   │
│   ├── v2_pid_speed_control/      # Article 2 — PID, empirical tuning
│   │   ├── robot_car_pid.ino
│   │   ├── config.h
│   │   ├── motors.h/.cpp
│   │   ├── ultrasonic.h/.cpp
│   │   ├── encoder.h/.cpp
│   │   └── pid.h/.cpp
│   │
│   ├── v3_ziegler_nichols/        # Article 3 — ZN tuned PID
│   │   ├── robot_car_zn.ino       # Same logic as v2; config.h uses ZN gains
│   │   ├── config.h               # ZN-derived Kp, Ki, Kd
│   │   ├── motors.h/.cpp
│   │   ├── ultrasonic.h/.cpp
│   │   ├── encoder.h/.cpp
│   │   └── pid.h/.cpp
│   │
│   ├── v4_feedforward_pid/        # Article 4 — feedforward + PID
│   │   ├── robot_car_ff.ino
│   │   ├── config.h               # Adds PID_KFF
│   │   ├── motors.h/.cpp
│   │   ├── ultrasonic.h/.cpp
│   │   ├── encoder.h/.cpp
│   │   └── pid.h/.cpp             # Adds kff field to PIDController
│   │
│   ├── test_motors/               # Verify motor wiring direction
│   ├── test_ultrasonic/           # Verify sensor readings
│   ├── test_pid/                  # PID tuning; two-phase open/closed
│   ├── test_step_response/        # ZN step response data collection
│   ├── test_ultimate_gain/        # ZN ultimate gain characterisation
│   └── test_feedforward/          # Kff measurement + FF+PID tuning
│
├── docs/
│   └── wiring_diagram.md
├── schematics/
│   └── README.md
├── .gitignore
├── LICENSE
└── README.md
```

---

## Getting Started

### Prerequisites
- [Arduino IDE](https://www.arduino.cc/en/software) 1.8.x or 2.x
- No external libraries required

### Recommended Upload Order

**Step 1 — Test motors:**
Open `src/test_motors/test_motors.ino`. Upload. Watch each motor direction. Swap wires at L298N if any motor runs backwards.

**Step 2 — Test sensor:**
Open `src/test_ultrasonic/test_ultrasonic.ino`. Upload. Wave hand in front of sensor and verify readings in Serial Monitor.

**Step 3 — Upload main sketch:**
Start with `v1_obstacle_avoidance/robot_car.ino` and work up through versions.

---

## Version Notes

### v1 — Obstacle Avoidance (Open-Loop)
Upload `src/v1_obstacle_avoidance/robot_car.ino`. No encoders needed. Robot drives forward, reverses and turns when an obstacle is detected within `STOP_DISTANCE` (30 cm). Tune timing and threshold in `config.h`.

### v2 — PID Closed-Loop Speed Control
Requires wheel encoders on pins 2 and 3. Upload `test_pid.ino` first to find your `TARGET_SPEED_PULSES` value, then set it in `config.h` before uploading the main sketch.

### v3 — Ziegler-Nichols Tuning
**Step 1:** Upload `test_step_response.ino`. Send a character to Serial Monitor. Copy the CSV output into a spreadsheet and extract K, L, tau graphically.
**Step 2:** Compute ZN gains: `Kp = 1.2*tau/(K*L)`, `Ki = Kp/(2L)`, `Kd = Kp*(0.5L)`.
**Step 3:** Enter gains in `v3_ziegler_nichols/config.h`. Upload `robot_car_zn.ino`.
**If overshoot > 25%:** Reduce all three gains by 20–30%.

### v4 — Feedforward + PID
**Step 1:** Upload `test_feedforward.ino`. During Phase 1 (open-loop), read the steady-state pulse counts. Compute `Kff = DRIVE_SPEED / average_count`. Repeat at PWM 120, 150, 180 and average.
**Step 2:** Enter `Kff` as `PID_KFF` in `v4_feedforward_pid/config.h`. Also reduce `PID_KI` to ~50% of the Ziegler-Nichols value.
**Step 3:** Upload `robot_car_ff.ino`. During Phase 2 of `test_feedforward`, verify settling time and overshoot have improved vs v3.

---

## Troubleshooting

| Symptom | Cause | Fix |
|---|---|---|
| Robot spins in place | One motor wired backwards | Swap that motor's wire pair at L298N |
| Robot drifts left/right (v1) | Motor speed mismatch | Upgrade to v2 PID control |
| Sensor always reads 999 | Echo pin disconnected | Check wiring; Trig/Echo not swapped |
| Arduino resets at motor start | PSU inrush current | Add 100µF cap across battery terminals |
| L298N very hot | Motor stall or over-voltage | Check for mechanical obstruction |
| Erratic behaviour | Missing common ground | Connect Arduino GND to L298N GND |
| PID oscillates rapidly | Kp too high | Reduce Kp by 20% increments |
| Residual steady-state error | Ki too low | Increase Ki gradually |
| FF+PID overshoots worse | Kff too high | Re-measure K at multiple PWM values |

---

## Future Development

- [x] Open-loop obstacle avoidance (v1)
- [x] PID closed-loop speed control (v2)
- [x] Ziegler-Nichols formal tuning (v3)
- [x] Feedforward + PID (v4)
- [ ] Heading control outer loop (v5)
- [ ] Odometry and position tracking
- [ ] Bluetooth remote control
- [ ] Servo-mounted sensor for left/right scanning

---

## License

MIT License — see [LICENSE](LICENSE) for details.
