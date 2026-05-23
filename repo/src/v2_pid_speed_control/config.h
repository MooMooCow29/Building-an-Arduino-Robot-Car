/**
 * config.h  —  v2: PID Closed-Loop Speed Control
 * -----------------------------------------------
 * All tunable parameters in one place.
 *
 * HOW TO SET TARGET_SPEED_PULSES:
 *   1. Upload test_pid sketch with Kp=Ki=Kd=0, base PWM = DRIVE_SPEED.
 *   2. Open Serial Monitor. Read reported pulse counts.
 *   3. Set TARGET_SPEED_PULSES to the average of left and right readings.
 *   Typical value for TT motors at PWM 180: 6 to 10 pulses per 100ms.
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

// ── Encoder Pins (hardware interrupt pins only) ───────────────
// Pin assignments defined in encoder.h (2 = INT0, 3 = INT1)

// ── Encoder Physical Constants ────────────────────────────────
#define ENCODER_SLOTS         20      // Slots per disc revolution
#define WHEEL_DIAMETER_MM     65      // mm
// MM_PER_PULSE = PI * 65 / 20 = ~10.2 mm

// ── PID Timing ────────────────────────────────────────────────
#define PID_INTERVAL_MS       100     // Control loop period (ms)
// DT_S = PID_INTERVAL_MS / 1000.0f = 0.1 seconds

// ── Target Speed ──────────────────────────────────────────────
#define TARGET_SPEED_PULSES   8       // Pulses per 100ms window

// ── PID Gains (empirically tuned — start here) ────────────────
#define PID_KP                8.0f
#define PID_KI                2.0f
#define PID_KD                0.0f
#define PID_INTEGRAL_LIMIT    80.0f

// ── Motor Speeds ──────────────────────────────────────────────
#define DRIVE_SPEED           180
#define REVERSE_SPEED         160
#define TURN_SPEED            150

// ── Obstacle Avoidance ────────────────────────────────────────
#define STOP_DISTANCE         30      // cm

// ── Timing ────────────────────────────────────────────────────
#define STOP_PAUSE_MS         200
#define REVERSE_TIME_MS       400
#define TURN_TIME_MS          500

// ── Sensor ────────────────────────────────────────────────────
#define ULTRASONIC_TIMEOUT_US 30000UL
#define NO_OBSTACLE_CM        999
