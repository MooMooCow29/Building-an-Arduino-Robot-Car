/**
 * config.h
 * --------
 * Central configuration file. Adjust these values to tune
 * robot behaviour without editing the main sketch or library files.
 */

#pragma once

// ── Serial ──────────────────────────────────────────────────
#define SERIAL_BAUD       9600

// ── Pin Assignments ──────────────────────────────────────────
// L298N Motor Driver — Channel A (Left Motor)
#define PIN_ENA           9     // PWM speed control — must be a PWM pin
#define PIN_IN1           8     // Direction bit 1
#define PIN_IN2           7     // Direction bit 2

// L298N Motor Driver — Channel B (Right Motor)
#define PIN_ENB           6     // PWM speed control — must be a PWM pin
#define PIN_IN3           5     // Direction bit 1
#define PIN_IN4           4     // Direction bit 2

// HC-SR04 Ultrasonic Sensor
#define PIN_TRIG          13
#define PIN_ECHO          12

// ── Speed Settings (0–255 PWM) ───────────────────────────────
#define DRIVE_SPEED       180   // Normal forward cruise speed
#define REVERSE_SPEED     160   // Speed while reversing
#define TURN_SPEED        150   // Speed during a turn

// ── Obstacle Avoidance Thresholds ────────────────────────────
#define STOP_DISTANCE     30    // cm — stop if obstacle closer than this

// ── Timing (milliseconds) ────────────────────────────────────
#define STOP_PAUSE_MS     200   // Brief pause before reversing
#define REVERSE_TIME_MS   400   // Duration of reverse manoeuvre
#define TURN_TIME_MS      500   // Duration of turn manoeuvre

// ── Sensor ───────────────────────────────────────────────────
#define ULTRASONIC_TIMEOUT_US  30000UL  // 30 000 µs ≈ 5 m max range
#define NO_OBSTACLE_CM         999      // Sentinel: no echo received
