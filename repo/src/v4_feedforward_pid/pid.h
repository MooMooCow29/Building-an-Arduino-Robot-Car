/**
 * pid.h  —  v4: Feedforward + PID
 * ---------------------------------
 * Extends the standard PID controller with a feedforward term.
 *
 * Architecture:
 *   output = feedforward + P + I + D
 *          = (kff * setpoint) + Kp*e + Ki*integral + Kd*derivative
 *
 * The feedforward term provides the baseline PWM needed to reach
 * the setpoint instantly, without waiting for error to accumulate.
 * The PID terms then only correct the residual model error.
 *
 * kff = 1/K = 1 / (steady-state speed / step PWM)
 *           = step PWM / steady-state speed
 *
 * HOW TO MEASURE kff:
 *   Run test_feedforward sketch open-loop at DRIVE_SPEED.
 *   Read steady-state encoder count.
 *   Kff = DRIVE_SPEED / average_count
 *   Repeat at 2-3 PWM values and average.
 *
 * Set kff = 0.0 to disable feedforward and run PID-only.
 *
 * Article: "Feedforward Control: The Missing Half of Every PID Tutorial"
 * Published: Level Up Coding — medium.com/@ks683557
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
  float kff;              // Feedforward gain (1/K). Set 0.0 to disable.
  float integral;
  float previousError;
  float integralLimit;
  float outputMin;
  float outputMax;
} PIDController;

void  pid_init   (PIDController* pid,
                  float kp, float ki, float kd, float kff,
                  float integralLimit,
                  float outputMin, float outputMax);

float pid_compute(PIDController* pid,
                  float setpoint, float measured, float dt_s);

void  pid_reset  (PIDController* pid);
