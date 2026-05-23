/**
 * config.h  —  v3: Ziegler-Nichols Tuned PID
 * --------------------------------------------
 * Same architecture as v2. Only the PID gains change.
 *
 * HOW THESE GAINS WERE DERIVED (Step Response Method):
 *   Step input:          PWM 180
 *   Steady-state speed:  9 pulses / 100ms
 *   Dead time      L  =  0.150 s
 *   Time constant  tau =  0.400 s
 *   Process gain   K  =  9 / 180 = 0.05
 *
 *   Kp = 1.2 * tau / (K * L) = 1.2 * 0.40 / (0.05 * 0.15) = 64.0
 *   Ti = 2L = 0.30 s   ->   Ki = Kp / Ti = 213.3
 *   Td = 0.5L = 0.075s  ->  Kd = Kp * Td = 4.8
 *
 * REPLACE THESE WITH YOUR OWN MEASURED VALUES.
 * Run test_step_response or test_ultimate_gain sketches first.
 *
 * If overshoot exceeds 25%, reduce all three gains by 20-30%.
 *
 * Article: "Ziegler-Nichols PID Tuning from First Principles"
 * Published: Level Up Coding — medium.com/@ks683557
 *
 * Author : [Your Name]
 * Date   : 2026
 * License: MIT
 */

#pragma once

// ── Serial ───────────────────────────────────────────────────
#define SERIAL_BAUD           9600

// ── Motor Driver Pins (L298N) ─────────────────────────────────
#define PIN_ENA               9
#define PIN_IN1               8
#define PIN_IN2               7
#define PIN_ENB               6
#define PIN_IN3               5
#define PIN_IN4               4

// ── Ultrasonic Sensor (HC-SR04) ───────────────────────────────
#define PIN_TRIG              13
#define PIN_ECHO              12

// ── PID Timing ────────────────────────────────────────────────
#define PID_INTERVAL_MS       100
#define TARGET_SPEED_PULSES   8

// ── Ziegler-Nichols Derived Gains ─────────────────────────────
// Replace with YOUR measured values from the step response test.
#define PID_KP                64.0f
#define PID_KI               213.3f
#define PID_KD                 4.8f
#define PID_INTEGRAL_LIMIT   255.0f

// ── Motor Speeds ──────────────────────────────────────────────
#define DRIVE_SPEED           180
#define REVERSE_SPEED         160
#define TURN_SPEED            150

// ── Obstacle Avoidance ────────────────────────────────────────
#define STOP_DISTANCE         30

// ── Timing ────────────────────────────────────────────────────
#define STOP_PAUSE_MS         200
#define REVERSE_TIME_MS       400
#define TURN_TIME_MS          500

// ── Sensor ────────────────────────────────────────────────────
#define ULTRASONIC_TIMEOUT_US 30000UL
#define NO_OBSTACLE_CM        999
