/**
 * pid.cpp  —  v4: Feedforward + PID
 * ------------------------------------
 * Combined feedforward-plus-PID implementation.
 *
 * Output breakdown:
 *   feedforward  what we know we need  (kff * setpoint)
 *   P            present error correction
 *   I            accumulated past error correction
 *   D            anticipated future error correction
 *
 * The feedforward term dramatically reduces integrator burden:
 *   PID only:          integral holds ~160 counts at steady state
 *   Feedforward + PID: integral holds ~12 counts at steady state
 *
 * This means faster settling, less overshoot, and faster
 * disturbance recovery because the integrator has less to
 * unwind when a load change occurs.
 *
 * Author : [Your Name]
 * Date   : 2026
 * License: MIT
 */

#include "pid.h"
#include <Arduino.h>

void pid_init(PIDController* pid,
              float kp, float ki, float kd, float kff,
              float integralLimit,
              float outputMin, float outputMax) {
  pid->kp            = kp;
  pid->ki            = ki;
  pid->kd            = kd;
  pid->kff           = kff;
  pid->integral      = 0.0f;
  pid->previousError = 0.0f;
  pid->integralLimit = integralLimit;
  pid->outputMin     = outputMin;
  pid->outputMax     = outputMax;
}

float pid_compute(PIDController* pid,
                  float setpoint, float measured, float dt_s) {

  // ── Feedforward ─────────────────────────────────────────────
  // Provides baseline PWM immediately. No error accumulation required.
  float feedforward = pid->kff * setpoint;

  // ── Error ────────────────────────────────────────────────────
  float error = setpoint - measured;

  // ── Proportional ─────────────────────────────────────────────
  float P = pid->kp * error;

  // ── Integral with anti-windup clamping ───────────────────────
  pid->integral += pid->ki * error * dt_s;
  if      (pid->integral >  pid->integralLimit) pid->integral =  pid->integralLimit;
  else if (pid->integral < -pid->integralLimit) pid->integral = -pid->integralLimit;
  float I = pid->integral;

  // ── Derivative ───────────────────────────────────────────────
  float D = (dt_s > 0.0f)
            ? pid->kd * (error - pid->previousError) / dt_s
            : 0.0f;
  pid->previousError = error;

  // ── Combined output ───────────────────────────────────────────
  // feedforward: what we know we need
  // P + I + D:   correction for what we got wrong
  float output = feedforward + P + I + D;
  return constrain(output, pid->outputMin, pid->outputMax);
}

void pid_reset(PIDController* pid) {
  pid->integral      = 0.0f;
  pid->previousError = 0.0f;
}
