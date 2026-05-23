/**
 * test_ultrasonic.ino
 * -------------------
 * HC-SR04 sensor verification sketch.
 * Move hand toward and away from sensor.
 * Readings should track smoothly every 100ms.
 *
 * Troubleshooting:
 *   Always reads 999  -> Echo pin not connected or Trig/Echo swapped
 *   Always reads 0    -> Check VCC connection
 *   Erratic readings  -> Add 100uF cap across 5V rail
 *   No change < 2cm   -> Normal; sensor blind zone below 2cm
 *
 * Open Serial Monitor at 9600 baud.
 *
 * Author : [Your Name]
 * Date   : 2026
 * License: MIT
 */

#include "config.h"
#include "ultrasonic.h"

void setup() {
  Serial.begin(SERIAL_BAUD);
  ultrasonic_init();
  Serial.println(F("HC-SR04 Sensor Test"));
  Serial.println(F("-------------------"));
}

void loop() {
  long raw_us  = ultrasonic_read_us();
  long dist_cm = ultrasonic_read_cm();

  Serial.print(F("Raw: ")); Serial.print(raw_us);
  Serial.print(F(" us  |  Distance: "));
  if (dist_cm == NO_OBSTACLE_CM) {
    Serial.println(F("--- (no echo / out of range)"));
  } else {
    Serial.print(dist_cm); Serial.println(F(" cm"));
  }
  delay(100);
}
