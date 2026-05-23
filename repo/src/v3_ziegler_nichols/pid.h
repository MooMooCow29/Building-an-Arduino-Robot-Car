/**
 * pid.h  —  v2/v3: Standard PID (no feedforward)
 * ------------------------------------------------
 * Generic discrete PID controller.
 * Instantiate one PIDController per motor.
 *
 * Features:
 *   - Anti-windup: integral clamped to [-integralLimit, +integralLimit]
 *   - Output clamping: result constrained to [outputMin, outputMax]
 *   - pid_reset() to clear state on motor stop or setpoint step changes
 *
 * Tuning (Ziegler-Nichols step response method):
 *   1. Run test_step_response sketch. Extract K, L, tau.
 *   2. Compute: Kp = 1.2*tau/(K*L), Ti = 2L, Td = 0.5L
 *               Ki = Kp/Ti,  Kd = Kp*Td
 *   3. If overshoot > 25%, reduce all gains by 20-30%.
 *
 * Author : [Your Name]
 * Date   : 2026
 * License: MIT
 */

#pragma once

typedef struct {
  float kp;
  float ki;
  float kd;
  float integral;
  float previousError;
  float integralLimit;
  float outputMin;
  float outputMax;
} PIDController;

void  pid_init   (PIDController* pid,
                  float kp, float ki, float kd,
                  float integralLimit,
                  float outputMin, float outputMax);

float pid_compute(PIDController* pid,
                  float setpoint, float measured, float dt_s);

void  pid_reset  (PIDController* pid);
