/**
 * ultrasonic.cpp
 * --------------
 * Implementation for the HC-SR04 ultrasonic distance sensor.
 *
 * Trigger sequence (required by the HC-SR04 datasheet):
 *   1. Pull Trig LOW for ≥ 2 µs to ensure a clean pulse edge.
 *   2. Pull Trig HIGH for exactly 10 µs.
 *   3. Pull Trig LOW.
 *   4. Wait for Echo pin to go HIGH (sensor is transmitting burst).
 *   5. Measure the duration Echo stays HIGH (round-trip travel time).
 *   6. Convert duration to distance.
 */

#include "ultrasonic.h"
#include "config.h"

void ultrasonic_init() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  // Ensure Trig starts low
  digitalWrite(PIN_TRIG, LOW);
}

long ultrasonic_read_us() {
  // 1. Guarantee Trig is LOW before triggering
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);

  // 2. Fire the 10 µs trigger pulse
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // 3. Measure echo pulse width (returns 0 on timeout)
  return pulseIn(PIN_ECHO, HIGH, ULTRASONIC_TIMEOUT_US);
}

long ultrasonic_read_cm() {
  long duration = ultrasonic_read_us();
  if (duration == 0) return NO_OBSTACLE_CM;
  return duration / 58L;
}
