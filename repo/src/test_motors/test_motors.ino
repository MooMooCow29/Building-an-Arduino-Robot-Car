/**
 * test_motors.ino
 * ---------------
 * Motor wiring verification sketch.
 * Upload BEFORE any main sketch to confirm motor direction.
 *
 * Expected behaviour:
 *   [1] Left motor FORWARD  -> left wheel spins forward
 *   [2] Left motor BACKWARD -> left wheel spins backward
 *   [3] Right motor FORWARD -> right wheel spins forward
 *   [4] Right motor BACKWARD-> right wheel spins backward
 *   [5] Both FORWARD        -> robot would drive straight
 *   [6] Both BACKWARD       -> robot would reverse
 *   [7] Turn RIGHT (pivot)  -> left forward, right stopped
 *   [8] Turn LEFT  (pivot)  -> right forward, left stopped
 *   [9] Spin RIGHT in place -> left forward, right backward
 *
 * If a motor runs in the wrong direction:
 *   Swap its wire pair at the L298N screw terminals.
 *
 * Open Serial Monitor at 9600 baud.
 *
 * Author : [Your Name]
 * Date   : 2026
 * License: MIT
 */

#include "config.h"
#include "motors.h"

#define TEST_SPEED  160
#define STEP_MS    1000

void setup() {
  Serial.begin(SERIAL_BAUD);
  motors_init();
  Serial.println(F("Motor Test — starting in 2s"));
  delay(2000);
}

void loop() {
  Serial.println(F("[1] Left FORWARD"));
  motors_set(TEST_SPEED, true, 0, true);
  delay(STEP_MS); motors_stop(); delay(300);

  Serial.println(F("[2] Left BACKWARD"));
  motors_set(TEST_SPEED, false, 0, true);
  delay(STEP_MS); motors_stop(); delay(300);

  Serial.println(F("[3] Right FORWARD"));
  motors_set(0, true, TEST_SPEED, true);
  delay(STEP_MS); motors_stop(); delay(300);

  Serial.println(F("[4] Right BACKWARD"));
  motors_set(0, true, TEST_SPEED, false);
  delay(STEP_MS); motors_stop(); delay(300);

  Serial.println(F("[5] Both FORWARD"));
  motors_forward(TEST_SPEED);
  delay(STEP_MS); motors_stop(); delay(300);

  Serial.println(F("[6] Both BACKWARD"));
  motors_backward(TEST_SPEED);
  delay(STEP_MS); motors_stop(); delay(300);

  Serial.println(F("[7] Turn RIGHT"));
  motors_turn_right(TEST_SPEED);
  delay(STEP_MS); motors_stop(); delay(300);

  Serial.println(F("[8] Turn LEFT"));
  motors_turn_left(TEST_SPEED);
  delay(STEP_MS); motors_stop(); delay(300);

  Serial.println(F("[9] Spin RIGHT"));
  motors_spin_right(TEST_SPEED);
  delay(STEP_MS); motors_stop(); delay(300);

  Serial.println(F("== Cycle complete. Repeating in 3s =="));
  delay(3000);
}
