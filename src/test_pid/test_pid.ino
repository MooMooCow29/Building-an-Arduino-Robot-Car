/**
 * test_pid.ino
 * ------------
 * PID tuning and encoder verification sketch.
 *
 * USE THIS BEFORE uploading robot_car_pid.ino.
 * It runs both motors at DRIVE_SPEED open-loop for 2 seconds,
 * prints the encoder pulse counts per 100 ms window,
 * then applies the PID controller and prints the corrected
 * PWM values alongside the measured speeds.
 *
 * HOW TO USE:
 *   Phase 1 — Open-loop measurement (first 2 seconds):
 *     Read the "L:" and "R:" pulse counts.
 *     These are your baseline measurements.
 *     Set TARGET_SPEED_PULSES in config.h to the average of
 *     left and right readings during this phase.
 *
 *   Phase 2 — PID active:
 *     Watch pwmL and pwmR. They should converge from DRIVE_SPEED
 *     to whatever value is needed to reach TARGET_SPEED_PULSES.
 *     If they oscillate wildly, reduce PID_KP.
 *     If they converge slowly and undershoot, increase PID_KP.
 *     If a small steady-state error remains after convergence,
 *     increase PID_KI.
 *
 * Hold the robot off the ground during Phase 1 so the wheels
 * spin freely. Lower to the ground for Phase 2.
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

PIDController pidLeft;
PIDController pidRight;

float pwmLeft  = DRIVE_SPEED;
float pwmRight = DRIVE_SPEED;

unsigned long lastPIDUpdate  = 0;
unsigned long phaseStartTime = 0;
bool          pidActive      = false;

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
  Serial.println(F("Phase 1: Open-loop (2 s) — hold robot off ground"));
  Serial.println(F("Phase 2: PID active    — place robot on ground"));
  Serial.println(F("--------------------------------------------"));
  Serial.println(F("L=left pulses  R=right pulses  pwmL  pwmR"));

  motors_forward(DRIVE_SPEED);
  phaseStartTime = millis();
}

void loop() {
  unsigned long now = millis();

  // Switch to PID after 2 seconds
  if (!pidActive && (now - phaseStartTime >= 2000)) {
    pidActive = true;
    Serial.println(F("--- Phase 2: PID active ---"));
  }

  // PID / print at fixed interval
  if (now - lastPIDUpdate >= PID_INTERVAL_MS) {
    long speedL = encoder_get_speed(MOTOR_LEFT);
    long speedR = encoder_get_speed(MOTOR_RIGHT);

    if (pidActive) {
      pwmLeft  = pid_compute(&pidLeft,
                             TARGET_SPEED_PULSES, (float)speedL, DT_S);
      pwmRight = pid_compute(&pidRight,
                             TARGET_SPEED_PULSES, (float)speedR, DT_S);
      motors_set((uint8_t)pwmLeft, true, (uint8_t)pwmRight, true);
    }

    Serial.print(F("L:")); Serial.print(speedL);
    Serial.print(F("  R:")); Serial.print(speedR);
    if (pidActive) {
      Serial.print(F("  pwmL:")); Serial.print((int)pwmLeft);
      Serial.print(F("  pwmR:")); Serial.print((int)pwmRight);
      Serial.print(F("  err:")); Serial.print(TARGET_SPEED_PULSES - speedL);
    } else {
      Serial.print(F("  [open-loop PWM "));
      Serial.print(DRIVE_SPEED);
      Serial.print(F("]"));
    }
    Serial.println();

    lastPIDUpdate = now;
  }
}
