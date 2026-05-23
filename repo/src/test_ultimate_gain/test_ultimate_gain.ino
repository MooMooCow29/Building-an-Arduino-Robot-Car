/**
 * test_ultimate_gain.ino
 * -----------------------
 * Closed-loop ultimate gain characterisation for Ziegler-Nichols.
 *
 * WARNING: This test intentionally approaches marginal stability.
 *          Hold the robot off the ground throughout.
 *
 * PROCEDURE:
 *   1. Set TEST_KP low (start at 2.0). Ki = Kd = 0 enforced below.
 *   2. Upload. Open Serial Monitor at 9600 baud.
 *   3. Observe the L (left speed) column.
 *   4. Increase TEST_KP by ~20% each run and re-upload.
 *   5. Continue until speed oscillates with constant amplitude indefinitely.
 *   6. Record that TEST_KP value as Ku (ultimate gain).
 *   7. Measure the time between successive peaks in the output: that is Pu (s).
 *   8. Compute ZN gains:
 *        Kp = 0.6 * Ku
 *        Ki = Kp / (Pu / 2)
 *        Kd = Kp * (Pu / 8)
 *   9. Enter results in v3_ziegler_nichols/config.h
 *
 * Author : [Your Name]
 * Date   : 2026
 * License: MIT
 */

#include "config.h"
#include "motors.h"
#include "encoder.h"
#include "pid.h"

#define TEST_KP   10.0f    // <-- Increase this each run until oscillation
#define TEST_KI    0.0f    // Must be zero for this test
#define TEST_KD    0.0f    // Must be zero for this test

static PIDController pidLeft;
static PIDController pidRight;
static unsigned long lastUpdate = 0;
const  float         DT_S = PID_INTERVAL_MS / 1000.0f;

void setup() {
  Serial.begin(SERIAL_BAUD);
  motors_init();
  encoder_init();

  pid_init(&pidLeft,  TEST_KP, TEST_KI, TEST_KD,
           255.0f, 0.0f, 255.0f);
  pid_init(&pidRight, TEST_KP, TEST_KI, TEST_KD,
           255.0f, 0.0f, 255.0f);

  Serial.print(F("Ultimate Gain Test | Kp = "));
  Serial.println(TEST_KP);
  Serial.println(F("time_ms,L,R,pwmL,pwmR"));
  delay(500);
}

void loop() {
  unsigned long now = millis();
  if (now - lastUpdate >= PID_INTERVAL_MS) {
    long sL = encoder_get_speed(MOTOR_LEFT);
    long sR = encoder_get_speed(MOTOR_RIGHT);

    float pwmL = pid_compute(&pidLeft,  TARGET_SPEED_PULSES, (float)sL, DT_S);
    float pwmR = pid_compute(&pidRight, TARGET_SPEED_PULSES, (float)sR, DT_S);

    motors_set((uint8_t)pwmL, true, (uint8_t)pwmR, true);

    Serial.print(now); Serial.print(F(","));
    Serial.print(sL);  Serial.print(F(","));
    Serial.print(sR);  Serial.print(F(","));
    Serial.print((int)pwmL); Serial.print(F(","));
    Serial.println((int)pwmR);

    lastUpdate = now;
  }
}
