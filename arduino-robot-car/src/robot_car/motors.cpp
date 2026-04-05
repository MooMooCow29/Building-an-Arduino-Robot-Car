/**
 * motors.cpp
 * ----------
 * Implementation of the L298N motor driver API.
 *
 * Left motor  → L298N Channel A (ENA, IN1, IN2)
 * Right motor → L298N Channel B (ENB, IN3, IN4)
 *
 * Because the motors are mirror-mounted, forward rotation on the left
 * motor requires IN1=HIGH/IN2=LOW, while forward rotation on the right
 * motor requires IN3=LOW/IN4=HIGH. This inversion is encapsulated here
 * so the rest of the firmware never needs to think about it.
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
  // Mirror-mounted: physically inverted compared to left motor
  digitalWrite(PIN_IN3, LOW);
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
  // Active brake: set both INx HIGH to short motor terminals
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
