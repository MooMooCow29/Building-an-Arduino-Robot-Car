/**
 * motors.h
 * --------
 * Public API for the L298N dual H-bridge motor driver.
 *
 * Physical note: the two TT motors are mirror-mounted on the chassis,
 * so "forward" on the left motor uses opposite IN polarity to the right.
 * This is handled internally — callers use semantic functions only.
 *
 * Speed values: PWM duty cycle 0 (stopped) to 255 (full speed).
 *
 * Author : [Your Name]
 * Date   : 2026
 * License: MIT
 */

#pragma once
#include <Arduino.h>

void motors_init();
void motors_forward(uint8_t speed);
void motors_backward(uint8_t speed);
void motors_turn_right(uint8_t speed);
void motors_turn_left(uint8_t speed);
void motors_spin_right(uint8_t speed);
void motors_spin_left(uint8_t speed);
void motors_brake();
void motors_stop();
void motors_set(uint8_t speedLeft, bool dirLeft,
                uint8_t speedRight, bool dirRight);
