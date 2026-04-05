/**
 * test_ultrasonic.ino
 * -------------------
 * Standalone HC-SR04 sensor test sketch.
 * Upload this to verify sensor wiring and readings BEFORE uploading
 * the main sketch.
 *
 * Open the Serial Monitor at 9600 baud.
 * Move your hand toward and away from the sensor.
 * Distance readings should update every 100 ms and track smoothly.
 *
 * Troubleshooting:
 *   - Always reads 999: Echo pin not connected or Trig/Echo swapped.
 *   - Always reads 0  : pulseIn may be timing out instantly; check VCC.
 *   - Erratic readings: insufficient power; add 100 µF cap on 5V rail.
 *   - No change < 2 cm: normal — sensor blind zone below 2 cm.
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
  long raw_us = ultrasonic_read_us();
  long dist_cm = ultrasonic_read_cm();

  Serial.print(F("Raw: "));
  Serial.print(raw_us);
  Serial.print(F(" us  |  Distance: "));

  if (dist_cm == NO_OBSTACLE_CM) {
    Serial.println(F("--- (no echo / out of range)"));
  } else {
    Serial.print(dist_cm);
    Serial.println(F(" cm"));
  }

  delay(100);
}
