/**
 * ultrasonic.h
 * ------------
 * HC-SR04 ultrasonic distance sensor API.
 *
 * Distance formula:  distance_cm = echo_duration_us / 58
 * Valid range:       2 cm to 400 cm
 * Returns NO_OBSTACLE_CM (999) on timeout.
 *
 * Author : [Your Name]
 * Date   : 2026
 * License: MIT
 */

#pragma once
#include <Arduino.h>

void ultrasonic_init();
long ultrasonic_read_cm();
long ultrasonic_read_us();
