/**
 * pid.h
 * -----
 * Generic discrete PID controller.
 *
 * Designed to be instantiated once per controlled axis.
 * In this project, one PIDController is created for the
 * left motor and one for the right motor, allowing each
 * to be tuned independently.
 *
 * Features:
 *   - Anti-windup: integral clamped to [-integralLimit, +integralLimit]
 *   - Output clamping: result constrained to [outputMin, outputMax]
 *   - Derivative on error (set kd = 0.0 to disable for velocity control)
 *   - pid_reset() to clear state on setpoint step changes
 *
 * Tuning procedure:
 *   1. Set ki = 0, kd = 0. Increase kp until response is fast
 *      but not oscillating. Back off slightly.
 *   2. Increase ki gradually until steady-state error is eliminated.
 *      Reduce if slow oscillations appear.
 *   3. Optionally add a small kd to improve transient response.
 *      Reduce or zero if output becomes noisy.
 *
 * Author : [Your Name]
 * Date   : 2026
 * License: MIT
 */

#pragma once

// ── PID Controller State ──────────────────────────────────────
typedef struct {
  // Gains
  float kp;             // Proportional gain
  float ki;             // Integral gain
  float kd;             // Derivative gain

  // Internal state
  float integral;       // Accumulated integral term
  float previousError;  // Error from previous iteration (for derivative)

  // Limits
  float integralLimit;  // Anti-windup clamp (+/-)
  float outputMin;      // Minimum controller output
  float outputMax;      // Maximum controller output
} PIDController;

/**
 * Initialise a PID controller with the given gains and limits.
 * Must be called before pid_compute().
 *
 * kp, ki, kd       : Controller gains (start with ki=0, kd=0)
 * integralLimit     : Maximum absolute value of the integral accumulator
 * outputMin/Max     : Output clamp range (e.g. 0.0 to 255.0 for PWM)
 */
void pid_init(PIDController* pid,
              float kp, float ki, float kd,
              float integralLimit,
              float outputMin, float outputMax);

/**
 * Compute one PID iteration and return the controller output.
 *
 * pid      : Pointer to an initialised PIDController
 * setpoint : Target value (e.g. target pulses per 100 ms window)
 * measured : Actual measured value (e.g. encoder pulses this window)
 * dt_s     : Time step in seconds — MUST be consistent across calls
 *
 * Returns the clamped controller output (e.g. PWM 0–255).
 *
 * Call at a fixed interval matching dt_s. Inconsistent timing
 * corrupts the integral and derivative terms.
 */
float pid_compute(PIDController* pid,
                  float setpoint,
                  float measured,
                  float dt_s);

/**
 * Reset the integral accumulator and stored previous error to zero.
 *
 * Call this when:
 *   - The motor is stopped (prevents windup carry-over on restart)
 *   - The setpoint changes significantly
 *   - Switching between forward and reverse
 */
void pid_reset(PIDController* pid);
