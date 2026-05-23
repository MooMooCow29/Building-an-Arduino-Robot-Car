/**
 * test_pid.ino
 * ------------
 * PID tuning and encoder verification sketch.
 *
 * Phase 1 (0-2s, open-loop): Hold robot off the ground.
 *   Read "L:" and "R:" pulse counts.
 *   Set TARGET_SPEED_PULSES in config.h to the average.
 *
 * Phase 2 (after 2s, PID active): Place robot on the ground.
 *   Watch pwmL and pwmR converge toward steady state.
 *   Tuning guide:
 *     Oscillates rapidly    -> reduce PID_KP
 *     Converges slowly      -> increase PID_KP
 *     SS error persists     -> increase PID_KI
 *     Long-period oscillation -> reduce PID_KI
 *
 * Open Serial Monitor at 9600 baud.
 *
 * Author : [Your Name]
 * Date   : 2026
 * License: MIT
 */

#include "config.h"
#include "motors.h"
#include "encoder.h"
#include "pid.h"

static PIDController pidLeft;
static PIDController pidRight;

static float         pwmLeft  = DRIVE_SPEED;
static float         pwmRight = DRIVE_SPEED;

static unsigned long lastUpdate  = 0;
static unsigned long phaseStart  = 0;
static bool          pidActive   = false;

const float DT_S = PID_INTERVAL_MS / 1000.0f;

void setup() {
  Serial.begin(SERIAL_BAUD);
  motors_init();
  encoder_init();

  pid_init(&pidLeft,  PID_KP, PID_KI, PID_KD,
           PID_INTEGRAL_LIMIT, 0.0f, 255.0f);
  pid_init(&pidRight, PID_KP, PID_KI, PID_KD,
           PID_INTEGRAL_LIMIT, 0.0f, 255.0f);

  Serial.println(F("=== PID Tuning Test ==="));
  Serial.println(F("Phase 1: Open-loop (2s) — hold robot off ground"));
  Serial.println(F("Phase 2: PID active — place on ground"));
  Serial.println(F("L=left  R=right  pwmL  pwmR  err"));

  motors_forward(DRIVE_SPEED);
  phaseStart = millis();
}

void loop() {
  unsigned long now = millis();
  if (!pidActive && (now - phaseStart >= 2000)) {
    pidActive = true;
    Serial.println(F("--- Phase 2: PID active ---"));
  }

  if (now - lastUpdate >= PID_INTERVAL_MS) {
    long sL = encoder_get_speed(MOTOR_LEFT);
    long sR = encoder_get_speed(MOTOR_RIGHT);

    if (pidActive) {
      pwmLeft  = pid_compute(&pidLeft,  TARGET_SPEED_PULSES, (float)sL, DT_S);
      pwmRight = pid_compute(&pidRight, TARGET_SPEED_PULSES, (float)sR, DT_S);
      motors_set((uint8_t)pwmLeft, true, (uint8_t)pwmRight, true);
    }

    Serial.print(F("L:")); Serial.print(sL);
    Serial.print(F(" R:")); Serial.print(sR);
    if (pidActive) {
      Serial.print(F(" pwmL:")); Serial.print((int)pwmLeft);
      Serial.print(F(" pwmR:")); Serial.print((int)pwmRight);
      Serial.print(F(" err:"));  Serial.print(TARGET_SPEED_PULSES - sL);
    } else {
      Serial.print(F("  [open-loop PWM ")); Serial.print(DRIVE_SPEED); Serial.print(F("]"));
    }
    Serial.println();
    lastUpdate = now;
  }
}
