/**
 * test_step_response.ino
 * -----------------------
 * Open-loop step response characterisation for Ziegler-Nichols tuning.
 *
 * PROCEDURE:
 *   1. Upload. Hold robot off the ground. Open Serial Monitor at 9600 baud.
 *   2. Send any character to trigger the step.
 *   3. The sketch applies STEP_PWM and records speed for 3 seconds.
 *   4. Copy the CSV output into a spreadsheet. Plot speed vs time.
 *   5. Draw a tangent to the steepest part of the S-curve.
 *   6. Extract:
 *        L   = where tangent crosses the baseline (dead time, seconds)
 *        tau = time difference to where tangent crosses final value (seconds)
 *        K   = (final speed - 0) / STEP_PWM  (process gain)
 *   7. Compute ZN gains:
 *        Kp = 1.2 * tau / (K * L)
 *        Ki = Kp / (2 * L)
 *        Kd = Kp * (0.5 * L)
 *   8. Enter results in v3_ziegler_nichols/config.h
 *
 * Author : [Your Name]
 * Date   : 2026
 * License: MIT
 */

#include "config.h"
#include "motors.h"
#include "encoder.h"

#define STEP_PWM           180
#define SAMPLE_INTERVAL_MS 100
#define RECORD_DURATION_MS 3000

void setup() {
  Serial.begin(SERIAL_BAUD);
  motors_init();
  encoder_init();
  motors_stop();

  Serial.println(F("Step Response Test"));
  Serial.println(F("Send any character to begin..."));
  while (!Serial.available()) {}
  Serial.read();

  Serial.println(F("time_ms,speed_L,speed_R"));
  delay(200);

  unsigned long startTime  = millis();
  unsigned long lastSample = startTime;
  motors_set(STEP_PWM, true, STEP_PWM, true);

  while (millis() - startTime < RECORD_DURATION_MS) {
    unsigned long now = millis();
    if (now - lastSample >= SAMPLE_INTERVAL_MS) {
      long sL = encoder_get_speed(MOTOR_LEFT);
      long sR = encoder_get_speed(MOTOR_RIGHT);
      Serial.print(now - startTime); Serial.print(F(","));
      Serial.print(sL); Serial.print(F(","));
      Serial.println(sR);
      lastSample = now;
    }
  }

  motors_stop();
  Serial.println(F("--- Test complete. Copy CSV data above into a spreadsheet. ---"));
  while (true) {}
}

void loop() {}
