/**
 * test_feedforward.ino
 * ---------------------
 * Feedforward gain (Kff) measurement and FF+PID tuning sketch.
 *
 * PHASE 1 (0-3s, open-loop) — Measure Kff:
 *   Hold robot off the ground.
 *   Read steady-state "L:" and "R:" pulse counts.
 *   Kff = DRIVE_SPEED / average_count
 *   Repeat at PWM 120, 150, 180 and average all three.
 *   Enter result as PID_KFF in v4_feedforward_pid/config.h
 *
 * PHASE 2 (after 3s, FF+PID active):
 *   Place robot on the ground.
 *   Watch pwmL/pwmR. They should converge quickly with low overshoot.
 *   If overshoot is high: reduce PID_KI (not Kff).
 *   If residual SS error: slightly increase PID_KI.
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

static unsigned long lastUpdate = 0;
static unsigned long phaseStart = 0;
static bool          ffActive   = false;

const float DT_S = PID_INTERVAL_MS / 1000.0f;

void setup() {
  Serial.begin(SERIAL_BAUD);
  motors_init();
  encoder_init();

  pid_init(&pidLeft,  PID_KP, PID_KI, PID_KD, PID_KFF,
           PID_INTEGRAL_LIMIT, 0.0f, 255.0f);
  pid_init(&pidRight, PID_KP, PID_KI, PID_KD, PID_KFF,
           PID_INTEGRAL_LIMIT, 0.0f, 255.0f);

  Serial.println(F("=== Feedforward Measurement + Tuning Test ==="));
  Serial.print(F("Current PID_KFF = ")); Serial.println(PID_KFF);
  Serial.println(F("Phase 1: Open-loop (3s) — hold off ground to measure Kff"));
  Serial.println(F("Phase 2: FF+PID active — place on ground"));
  Serial.println(F("L  R  [pwmL  pwmR  err]"));

  motors_forward(DRIVE_SPEED);
  phaseStart = millis();
}

void loop() {
  unsigned long now = millis();

  if (!ffActive && (now - phaseStart >= 3000)) {
    ffActive = true;
    pid_reset(&pidLeft); pid_reset(&pidRight);
    Serial.println(F("--- Phase 2: FF+PID active ---"));
  }

  if (now - lastUpdate >= PID_INTERVAL_MS) {
    long sL = encoder_get_speed(MOTOR_LEFT);
    long sR = encoder_get_speed(MOTOR_RIGHT);

    if (ffActive) {
      pwmLeft  = pid_compute(&pidLeft,  TARGET_SPEED_PULSES, (float)sL, DT_S);
      pwmRight = pid_compute(&pidRight, TARGET_SPEED_PULSES, (float)sR, DT_S);
      motors_set((uint8_t)pwmLeft, true, (uint8_t)pwmRight, true);
    }

    Serial.print(F("L:")); Serial.print(sL);
    Serial.print(F(" R:")); Serial.print(sR);
    if (ffActive) {
      Serial.print(F(" pwmL:")); Serial.print((int)pwmLeft);
      Serial.print(F(" pwmR:")); Serial.print((int)pwmRight);
      Serial.print(F(" err:"));  Serial.print(TARGET_SPEED_PULSES - sL);
      // Hint: Kff = DRIVE_SPEED / measured_open_loop_speed
      Serial.print(F("  |hint Kff~"));
      if (sL > 0) Serial.print((float)DRIVE_SPEED / (float)sL, 2);
    } else {
      Serial.print(F("  <- use avg for Kff = "));
      Serial.print((float)DRIVE_SPEED / max(1L, sL), 2);
    }
    Serial.println();
    lastUpdate = now;
  }
}
