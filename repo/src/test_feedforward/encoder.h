/**
 * encoder.h
 * ---------
 * Wheel encoder pulse counting via hardware interrupts.
 *
 * Uses Arduino Uno INT0 (pin 2) for left wheel
 * and INT1 (pin 3) for right wheel.
 *
 * Why volatile?
 *   Variables modified inside an ISR must be declared volatile.
 *   Without it the compiler may cache the value in a register
 *   and the main loop will never observe ISR updates.
 *
 * Why noInterrupts() during read?
 *   A long (32-bit) cannot be read atomically on an 8-bit AVR.
 *   An ISR firing mid-read produces a corrupted value.
 *   Disabling interrupts for the ~4 CPU cycles of the read
 *   prevents this race condition.
 *
 * Author : [Your Name]
 * Date   : 2026
 * License: MIT
 */

#pragma once
#include <Arduino.h>

#define ENCODER_LEFT_PIN   2    // Hardware interrupt INT0
#define ENCODER_RIGHT_PIN  3    // Hardware interrupt INT1

typedef enum { MOTOR_LEFT, MOTOR_RIGHT } MotorSide;

/**
 * Initialise encoder pins as INPUT_PULLUP and attach
 * rising-edge hardware interrupts. Call once from setup().
 */
void encoder_init();

/**
 * Return the pulse count accumulated since the last call
 * for the specified motor, then atomically reset to zero.
 *
 * Call at a fixed, consistent interval (e.g. every 100 ms).
 *
 *   speed_mm_s = encoder_get_speed(side) * 10.2f
 *                / (INTERVAL_MS / 1000.0f)
 *
 * Returns: pulse count (long, >= 0 for forward motion)
 */
long encoder_get_speed(MotorSide side);

/**
 * Reset both counters without reading.
 * Call when switching direction to discard transition pulses.
 */
void encoder_reset_all();
