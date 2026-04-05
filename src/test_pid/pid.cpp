/**
 * pid.cpp
 * -------
 * Discrete PID controller implementation.
 *
 * The three terms and what they physically do:
 *
 *   P (Proportional): responds to the current error.
 *      Large error -> large correction. Eliminates most
 *      of the error quickly but leaves a small steady-state
 *      offset when a constant disturbance is present.
 *
 *   I (Integral): responds to accumulated past error.
 *      Eliminates steady-state error by building up a
 *      correction over time. Subject to windup if the
 *      motor is stalled; use integralLimit to constrain.
 *
 *   D (Derivative): responds to the rate of change of error.
 *      Anticipates developing errors and damps overshoot.
 *      Amplifies sensor noise; keep kd small or zero for
 *      encoder-based velocity control.
 *
 * Anti-windup:
 *   The integral is clamped to [-integralLimit, +integralLimit]
 *   on every iteration. This prevents runaway accumulation when
 *   the motor cannot reach the setpoint (e.g. robot is obstructed).
 *
 * Author : [Your Name]
 * Date   : 2026
 * License: MIT
 */

#include "pid.h"
#include <Arduino.h>    // for constrain()

void pid_init(PIDController* pid,
              float kp, float ki, float kd,
              float integralLimit,
              float outputMin, float outputMax) {
  pid->kp             = kp;
  pid->ki             = ki;
  pid->kd             = kd;
  pid->integral       = 0.0f;
  pid->previousError  = 0.0f;
  pid->integralLimit  = integralLimit;
  pid->outputMin      = outputMin;
  pid->outputMax      = outputMax;
}

float pid_compute(PIDController* pid,
                  float setpoint,
                  float measured,
                  float dt_s) {
  // ── Error ────────────────────────────────────────────────
  float error = setpoint - measured;

  // ── Proportional term ────────────────────────────────────
  float P = pid->kp * error;

  // ── Integral term with anti-windup clamping ──────────────
  pid->integral += pid->ki * error * dt_s;
  if      (pid->integral >  pid->integralLimit) pid->integral =  pid->integralLimit;
  else if (pid->integral < -pid->integralLimit) pid->integral = -pid->integralLimit;
  float I = pid->integral;

  // ── Derivative term (on error) ───────────────────────────
  // Set kd = 0.0 in config.h to disable for velocity control.
  float derivative = (dt_s > 0.0f)
                     ? (error - pid->previousError) / dt_s
                     : 0.0f;
  float D = pid->kd * derivative;
  pid->previousError = error;

  // ── Sum and clamp output ─────────────────────────────────
  float output = P + I + D;
  return constrain(output, pid->outputMin, pid->outputMax);
}

void pid_reset(PIDController* pid) {
  pid->integral      = 0.0f;
  pid->previousError = 0.0f;
}
