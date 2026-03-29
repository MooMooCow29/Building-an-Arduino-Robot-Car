/**
 * ultrasonic.h
 * ------------
 * Public API for the HC-SR04 ultrasonic distance sensor.
 *
 * The HC-SR04 emits a 40 kHz ultrasonic burst when triggered and
 * reports the round-trip echo time as a pulse width on the Echo pin.
 *
 * Distance formula:
 *   distance_cm = echo_duration_us / 58
 *
 * Derivation:
 *   Speed of sound in air ≈ 0.0343 cm/µs at 20 °C
 *   Round-trip distance   = 0.0343 × t
 *   One-way distance      = (0.0343 × t) / 2 = t / 58.3 ≈ t / 58
 *
 * Valid range: 2 cm – 400 cm.
 * Returns NO_OBSTACLE_CM (999) when no echo is received within timeout.
 */

#pragma once
#include <Arduino.h>

/**
 * Initialise the sensor pins (Trig as OUTPUT, Echo as INPUT).
 * Call once from setup().
 */
void ultrasonic_init();

/**
 * Trigger one measurement and return the distance in centimetres.
 * Blocks for the duration of the echo pulse (max ~26 ms at 4.5 m).
 * Returns NO_OBSTACLE_CM if no echo is received within the timeout.
 */
long ultrasonic_read_cm();

/**
 * Return the raw echo pulse duration in microseconds.
 * Useful if you want to apply a custom conversion formula
 * (e.g., temperature-corrected speed of sound).
 */
long ultrasonic_read_us();
