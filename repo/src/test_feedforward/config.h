/**
 * config.h  —  v4: Feedforward + PID
 * ------------------------------------
 * HOW TO MEASURE PID_KFF:
 *   1. Upload test_feedforward sketch.
 *   2. Phase 1 (open-loop): read steady-state pulse counts.
 *   3. Kff = DRIVE_SPEED / average_count
 *   4. Repeat at PWM 120, 150, 180 and average all three results.
 *
 * Example: 9 pulses at PWM 180 -> K = 9/180 = 0.05 -> Kff = 20.0
 *
 * TUNING AFTER ADDING FEEDFORWARD:
 *   Ki should be reduced to ~50% of the Ziegler-Nichols value
 *   because the integrator no longer carries the full SS burden.
 *   PID_INTEGRAL_LIMIT can also be tightened significantly.
 *
 * Article: "Feedforward Control: The Missing Half of Every PID Tutorial"
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

// ── Feedforward Gain ──────────────────────────────────────────
// Kff = 1/K = step PWM / steady-state speed
// REPLACE WITH YOUR MEASURED VALUE.
#define PID_KFF               20.0f

// ── PID Gains (reduced Ki from ZN due to feedforward) ─────────
#define PID_KP                64.0f
#define PID_KI               106.0f   // ~50% of ZN value (213.3)
#define PID_KD                 4.8f
#define PID_INTEGRAL_LIMIT    60.0f   // Tighter — less windup needed

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
