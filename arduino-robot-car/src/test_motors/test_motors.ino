/**
 * test_motors.ino
 * ---------------
 * Standalone motor test sketch.
 * Upload this to verify motor wiring BEFORE uploading the main sketch.
 *
 * Each motor is tested individually in forward and reverse, followed by
 * a combined forward/backward/turn sequence. Observe the Serial Monitor
 * at 9600 baud to follow the test sequence in real time.
 *
 * Expected behaviour:
 *   1. Left motor forward  → left wheel spins forward
 *   2. Left motor backward → left wheel spins backward
 *   3. Right motor forward → right wheel spins forward
 *   4. Right motor backward→ right wheel spins backward
 *   5. Both forward        → robot would drive straight
 *   6. Turn right          → left wheel spins, right stops
 *   7. Turn left           → right wheel spins, left stops
 *
 * If a motor runs in the wrong direction, swap its two wires at the
 * L298N screw terminals (OUT1/OUT2 or OUT3/OUT4).
 */

#include "../robot_car/config.h"
#include "../robot_car/motors.h"

#define TEST_SPEED   160
#define STEP_MS      1000   // Duration of each test step

void setup() {
  Serial.begin(SERIAL_BAUD);
  motors_init();
  Serial.println(F("Motor Test — begin in 2 s"));
  delay(2000);
}

void loop() {
  // ── Left motor only ────────────────────────────────────────
  Serial.println(F("[1] Left motor FORWARD"));
  motors_set(TEST_SPEED, true, 0, true);
  delay(STEP_MS);
  motors_stop();
  delay(300);

  Serial.println(F("[2] Left motor BACKWARD"));
  motors_set(TEST_SPEED, false, 0, true);
  delay(STEP_MS);
  motors_stop();
  delay(300);

  // ── Right motor only ───────────────────────────────────────
  Serial.println(F("[3] Right motor FORWARD"));
  motors_set(0, true, TEST_SPEED, true);
  delay(STEP_MS);
  motors_stop();
  delay(300);

  Serial.println(F("[4] Right motor BACKWARD"));
  motors_set(0, true, TEST_SPEED, false);
  delay(STEP_MS);
  motors_stop();
  delay(300);

  // ── Both motors ────────────────────────────────────────────
  Serial.println(F("[5] Both FORWARD"));
  motors_forward(TEST_SPEED);
  delay(STEP_MS);
  motors_stop();
  delay(300);

  Serial.println(F("[6] Both BACKWARD"));
  motors_backward(TEST_SPEED);
  delay(STEP_MS);
  motors_stop();
  delay(300);

  // ── Turns ──────────────────────────────────────────────────
  Serial.println(F("[7] Turn RIGHT (pivot)"));
  motors_turn_right(TEST_SPEED);
  delay(STEP_MS);
  motors_stop();
  delay(300);

  Serial.println(F("[8] Turn LEFT (pivot)"));
  motors_turn_left(TEST_SPEED);
  delay(STEP_MS);
  motors_stop();
  delay(300);

  Serial.println(F("[9] Spin RIGHT (in place)"));
  motors_spin_right(TEST_SPEED);
  delay(STEP_MS);
  motors_stop();
  delay(300);

  Serial.println(F("== Test cycle complete. Repeating in 3 s =="));
  delay(3000);
}
