/**
 * config.h  —  v1: Obstacle Avoidance (Open-Loop)
 * ------------------------------------------------
 * All tunable parameters in one place.
 *
 * Author : [Your Name]
 * Date   : 2026
 * License: MIT
 */

#pragma once

// ── Serial ───────────────────────────────────────────────────
#define SERIAL_BAUD           9600

// ── Motor Driver Pins (L298N) ─────────────────────────────────
#define PIN_ENA               9     // Left  motor PWM  (Timer 1)
#define PIN_IN1               8     // Left  motor direction A
#define PIN_IN2               7     // Left  motor direction B
#define PIN_ENB               6     // Right motor PWM  (Timer 0)
#define PIN_IN3               5     // Right motor direction A
#define PIN_IN4               4     // Right motor direction B

// ── Ultrasonic Sensor Pins (HC-SR04) ─────────────────────────
#define PIN_TRIG              13
#define PIN_ECHO              12

// ── Motor Speeds (0-255 PWM) ──────────────────────────────────
#define DRIVE_SPEED           180
#define REVERSE_SPEED         160
#define TURN_SPEED            150

// ── Obstacle Avoidance ────────────────────────────────────────
#define STOP_DISTANCE         30    // cm — stop if obstacle closer

// ── Timing (milliseconds) ─────────────────────────────────────
#define STOP_PAUSE_MS         200
#define REVERSE_TIME_MS       400
#define TURN_TIME_MS          500

// ── Sensor ────────────────────────────────────────────────────
#define ULTRASONIC_TIMEOUT_US 30000UL
#define NO_OBSTACLE_CM        999
