/**
 * motors.h
 * --------
 * Public API for the L298N dual H-bridge motor driver.
 *
 * All speed values are PWM duty cycle: 0 (stopped) to 255 (full speed).
 *
 * Physical note: the two TT motors are mirror-mounted on the chassis,
 * so "forward" on the left motor uses opposite IN polarity to the right.
 * This is handled internally; callers simply use the semantic functions.
 */

#pragma once
#include <Arduino.h>

/**
 * Initialise all motor driver pins as outputs and ensure
 * both motors start in the stopped/coast state.
 * Call once from setup().
 */
void motors_init();

/**
 * Drive both motors forward at the given speed (0–255).
 */
void motors_forward(uint8_t speed);

/**
 * Drive both motors backward at the given speed (0–255).
 */
void motors_backward(uint8_t speed);

/**
 * Pivot right: left motor forward, right motor stopped.
 * For a tighter spin, set right motor backward — see motors_spin_right().
 */
void motors_turn_right(uint8_t speed);

/**
 * Pivot left: right motor forward, left motor stopped.
 */
void motors_turn_left(uint8_t speed);

/**
 * Spin in place to the right: left motor forward, right motor backward.
 * Faster rotation than motors_turn_right() but less smooth.
 */
void motors_spin_right(uint8_t speed);

/**
 * Spin in place to the left: right motor forward, left motor backward.
 */
void motors_spin_left(uint8_t speed);

/**
 * Active brake: short the motor terminals via the H-bridge.
 * Stops faster than coast but causes more motor wear.
 */
void motors_brake();

/**
 * Coast: disable both channels. Motors free-wheel to a stop.
 * Equivalent to motors_stop().
 */
void motors_stop();

/**
 * Set individual wheel speeds for fine-grained control.
 * speedLeft / speedRight : 0–255
 * dirLeft / dirRight     : true = forward, false = backward
 */
void motors_set(uint8_t speedLeft, bool dirLeft,
                uint8_t speedRight, bool dirRight);
