/**
 * encoder.h
 * ---------
 * Wheel encoder pulse counting via hardware interrupts.
 *
 * Uses Arduino Uno INT0 (pin 2) for the left wheel encoder
 * and INT1 (pin 3) for the right wheel encoder.
 *
 * Speed is reported as a raw pulse count per measurement window.
 * Calling encoder_get_speed() returns the accumulated count
 * since the last call and atomically resets the counter,
 * ready for the next measurement window.
 *
 * Hardware note:
 *   Connect encoder signal wire to the pin listed below.
 *   Connect encoder VCC to Arduino 5V.
 *   Connect encoder GND to Arduino GND.
 *   Use INPUT_PULLUP if the encoder module has open-collector output.
 *
 * Author : [Your Name]
 * Date   : 2026
 * License: MIT
 */

#pragma once
#include <Arduino.h>

// ── Pin Assignments ───────────────────────────────────────────
#define ENCODER_LEFT_PIN   2    // Hardware interrupt INT0
#define ENCODER_RIGHT_PIN  3    // Hardware interrupt INT1

// ── Motor side selector ───────────────────────────────────────
typedef enum { MOTOR_LEFT, MOTOR_RIGHT } MotorSide;

/**
 * Initialise encoder pins as INPUT_PULLUP and attach
 * rising-edge hardware interrupts.
 * Call once from setup().
 */
void encoder_init();

/**
 * Return the pulse count accumulated since the last call
 * for the specified motor side, then atomically reset
 * the counter to zero.
 *
 * Call at a fixed, consistent interval (e.g. every 100 ms)
 * to convert the raw count into a speed measurement:
 *
 *   speed_mm_s = encoder_get_speed(side) * MM_PER_PULSE
 *                / (INTERVAL_MS / 1000.0f)
 *
 * Returns: pulse count (long, always >= 0 for forward motion)
 */
long encoder_get_speed(MotorSide side);

/**
 * Reset both encoder counters to zero without reading them.
 * Useful when switching between forward and reverse to
 * discard any pulses counted during the transition.
 */
void encoder_reset_all();
