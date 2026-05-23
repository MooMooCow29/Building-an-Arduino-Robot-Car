/**
 * pid.cpp  —  v2/v3: Standard PID (no feedforward)
 * -------------------------------------------------
 * Discrete PID controller implementation.
 *
 * The three terms:
 *   P: responds to current error — fast, leaves steady-state offset
 *   I: responds to accumulated past error — eliminates SS offset
 *   D: responds to rate of change — reduces overshoot, amplifies noise
 *
 * Author : [Your Name]
 * Date   : 2026
 * License: MIT
 */

#include "pid.h"
#include <Arduino.h>

void pid_init(PIDController* pid,
              float kp, float ki, float kd,
              float integralLimit,
              float outputMin, float outputMax) {
  pid->kp            = kp;
  pid->ki            = ki;
  pid->kd            = kd;
  pid->integral      = 0.0f;
  pid->previousError = 0.0f;
  pid->integralLimit = integralLimit;
  pid->outputMin     = outputMin;
  pid->outputMax     = outputMax;
}

float pid_compute(PIDController* pid,
                  float setpoint, float measured, float dt_s) {
  float error = setpoint - measured;

  // Proportional
  float P = pid->kp * error;

  // Integral with anti-windup clamping
  pid->integral += pid->ki * error * dt_s;
  if      (pid->integral >  pid->integralLimit) pid->integral =  pid->integralLimit;
  else if (pid->integral < -pid->integralLimit) pid->integral = -pid->integralLimit;
  float I = pid->integral;

  // Derivative (set kd=0 to disable for velocity control)
  float D = (dt_s > 0.0f)
            ? pid->kd * (error - pid->previousError) / dt_s
            : 0.0f;
  pid->previousError = error;

  float output = P + I + D;
  return constrain(output, pid->outputMin, pid->outputMax);
}

void pid_reset(PIDController* pid) {
  pid->integral      = 0.0f;
  pid->previousError = 0.0f;
}
