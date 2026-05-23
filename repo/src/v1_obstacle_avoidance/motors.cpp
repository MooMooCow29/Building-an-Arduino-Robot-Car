/**
 * motors.cpp
 * ----------
 * L298N dual H-bridge motor driver implementation.
 *
 * Left motor  -> L298N Channel A (ENA, IN1, IN2)
 * Right motor -> L298N Channel B (ENB, IN3, IN4)
 *
 * Mirror-mounting inversion:
 *   "Forward" for the left motor = IN1 HIGH / IN2 LOW
 *   "Forward" for the right motor = IN3 LOW / IN4 HIGH  (inverted)
 *   This is encapsulated here. Higher-level code never sets pins directly.
 *
 * Author : [Your Name]
 * Date   : 2026
 * License: MIT
 */

#include "motors.h"
#include "config.h"

// ── Internal helpers ─────────────────────────────────────────

static void left_forward(uint8_t speed) {
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  analogWrite(PIN_ENA, speed);
}
static void left_backward(uint8_t speed) {
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, HIGH);
  analogWrite(PIN_ENA, speed);
}
static void left_stop() {
  analogWrite(PIN_ENA, 0);
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
}

static void right_forward(uint8_t speed) {
  digitalWrite(PIN_IN3, LOW);   // Inverted: mirror-mounted
  digitalWrite(PIN_IN4, HIGH);
  analogWrite(PIN_ENB, speed);
}
static void right_backward(uint8_t speed) {
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENB, speed);
}
static void right_stop() {
  analogWrite(PIN_ENB, 0);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);
}

// ── Public API ───────────────────────────────────────────────

void motors_init() {
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);
  motors_stop();
}

void motors_forward(uint8_t speed) {
  left_forward(speed);
  right_forward(speed);
}
void motors_backward(uint8_t speed) {
  left_backward(speed);
  right_backward(speed);
}
void motors_turn_right(uint8_t speed) {
  left_forward(speed);
  right_stop();
}
void motors_turn_left(uint8_t speed) {
  left_stop();
  right_forward(speed);
}
void motors_spin_right(uint8_t speed) {
  left_forward(speed);
  right_backward(speed);
}
void motors_spin_left(uint8_t speed) {
  left_backward(speed);
  right_forward(speed);
}
void motors_brake() {
  analogWrite(PIN_ENA, 255);
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, HIGH);
  analogWrite(PIN_ENB, 255);
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, HIGH);
}
void motors_stop() {
  left_stop();
  right_stop();
}
void motors_set(uint8_t speedLeft, bool dirLeft,
                uint8_t speedRight, bool dirRight) {
  if (dirLeft)  left_forward(speedLeft);
  else          left_backward(speedLeft);
  if (dirRight) right_forward(speedRight);
  else          right_backward(speedRight);
}
