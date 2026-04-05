/**
 * config.h  —  PID Edition
 * ------------------------
 * All tunable parameters in one place.
 * Edit this file to adjust robot behaviour without
 * touching the sketch or library implementation files.
 *
 * This config extends the original obstacle-avoidance
 * config with encoder and PID settings.
 *
 * Author : [Your Name]
 * Date   : 2026
 * License: MIT
 */

#pragma once

// ── Serial ──────────────────────────────────────────────────
#define SERIAL_BAUD         9600

// ── Motor Driver Pins (L298N) ────────────────────────────────
#define PIN_ENA             9     // Left motor PWM  (Timer 1)
#define PIN_IN1             8     // Left motor direction A
#define PIN_IN2             7     // Left motor direction B
#define PIN_ENB             6     // Right motor PWM (Timer 0)
#define PIN_IN3             5     // Right motor direction A
#define PIN_IN4             4     // Right motor direction B

// ── Ultrasonic Sensor Pins (HC-SR04) ─────────────────────────
#define PIN_TRIG            13
#define PIN_ECHO            12

// ── Encoder Physical Constants ────────────────────────────────
// Pin assignments are defined in encoder.h (ENCODER_LEFT_PIN = 2, RIGHT = 3)
#define ENCODER_SLOTS       20      // Slots per encoder disc revolution
#define WHEEL_DIAMETER_MM   65      // Wheel diameter in millimetres
// Computed:
// MM_PER_PULSE = (PI * WHEEL_DIAMETER_MM) / ENCODER_SLOTS
//              = (3.1416 * 65) / 20 = ~10.2 mm per pulse

// ── PID Timing ───────────────────────────────────────────────
#define PID_INTERVAL_MS     100     // Control loop period (milliseconds)
// dt_s = PID_INTERVAL_MS / 1000.0f = 0.1 seconds

// ── Target Speed ─────────────────────────────────────────────
// Pulses counted per PID_INTERVAL_MS at the desired cruise speed.
// HOW TO MEASURE:
//   1. Upload this sketch with PID disabled (kp=ki=kd=0, base PWM = DRIVE_SPEED).
//   2. Open Serial Monitor and read the reported pulse counts.
//   3. Set TARGET_SPEED_PULSES to the average of left and right readings.
// Typical value for TT motors at PWM 180: 6 to 10 pulses per 100 ms.
#define TARGET_SPEED_PULSES 8

// ── PID Gains ────────────────────────────────────────────────
// Start with KI = 0, KD = 0. Tune KP first.
// See the tuning procedure in the article / README for guidance.
#define PID_KP              8.0f
#define PID_KI              2.0f
#define PID_KD              0.0f    // Set to 0 for velocity control
#define PID_INTEGRAL_LIMIT  80.0f   // Anti-windup clamp

// ── Base Drive Speed (open-loop starting point) ───────────────
// The PID will adjust this value up or down.
// Set to a value that produces roughly TARGET_SPEED_PULSES
// when measured open-loop.
#define DRIVE_SPEED         180     // 0-255 PWM

// ── Other Motion Parameters ───────────────────────────────────
#define REVERSE_SPEED       160
#define TURN_SPEED          150

// ── Obstacle Avoidance ────────────────────────────────────────
#define STOP_DISTANCE       30      // cm — stop if obstacle closer

// ── Timing (milliseconds) ─────────────────────────────────────
#define STOP_PAUSE_MS       200
#define REVERSE_TIME_MS     400
#define TURN_TIME_MS        500

// ── Sensor ────────────────────────────────────────────────────
#define ULTRASONIC_TIMEOUT_US   30000UL
#define NO_OBSTACLE_CM          999
