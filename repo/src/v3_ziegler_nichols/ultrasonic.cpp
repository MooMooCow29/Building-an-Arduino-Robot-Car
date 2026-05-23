/**
 * ultrasonic.cpp
 * --------------
 * HC-SR04 ultrasonic sensor implementation.
 *
 * Trigger sequence (per datasheet):
 *   1. Pull Trig LOW >= 2 us (clean edge)
 *   2. Pull Trig HIGH for exactly 10 us
 *   3. Pull Trig LOW
 *   4. Measure Echo HIGH pulse duration with pulseIn()
 *   5. Convert: distance_cm = duration_us / 58
 *
 * Author : [Your Name]
 * Date   : 2026
 * License: MIT
 */

#include "ultrasonic.h"
#include "config.h"

void ultrasonic_init() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);
}

long ultrasonic_read_us() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  return pulseIn(PIN_ECHO, HIGH, ULTRASONIC_TIMEOUT_US);
}

long ultrasonic_read_cm() {
  long duration = ultrasonic_read_us();
  if (duration == 0) return NO_OBSTACLE_CM;
  return duration / 58L;
}
