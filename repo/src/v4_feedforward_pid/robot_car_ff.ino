/**
 * ============================================================
 *  v4: Arduino 2WD Robot — Feedforward + PID Speed Control
 * ============================================================
 *  Article: "Feedforward Control: The Missing Half of Every PID Tutorial"
 *  Published: Level Up Coding — medium.com/@ks683557
 *  GitHub:    github.com/MooMooCow29/Building-an-Arduino-Robot-Car
 *
 *  What feedforward adds over v3:
 *    Provides the baseline PWM immediately on setpoint change
 *    without waiting for error to accumulate. The PID then
 *    corrects only the residual model error (~10% of total effort).
 *
 *    Result vs PID-only:
 *      Settling time:    ~650ms -> ~190ms
 *      Overshoot:          ~24% -> ~6%
 *      Disturbance recovery: 400ms -> 150ms
 *      Integral SS value:   ~160 -> ~12 counts
 *
 *  No structural hardware changes from v2/v3.
 *  Only config.h and pid.h/cpp change.
 *
 *  Author : [Your Name]
 *  Date   : 2026
 *  License: MIT
 * ============================================================
 */

#include "config.h"
#include "motors.h"
#include "ultrasonic.h"
#include "encoder.h"
#include "pid.h"

// ── PID instances ────────────────────────────────────────────
static PIDController pidLeft;
static PIDController pidRight;

// ── Current PWM outputs ───────────────────────────────────────
static float pwmLeft  = DRIVE_SPEED;
static float pwmRight = DRIVE_SPEED;

// ── PID timing ────────────────────────────────────────────────
static unsigned long lastPIDUpdate = 0;
static const float   DT_S = PID_INTERVAL_MS / 1000.0f;

// ── State machine ─────────────────────────────────────────────
enum RobotState { STATE_FORWARD, STATE_STOP, STATE_REVERSING, STATE_TURNING };
static RobotState    currentState   = STATE_FORWARD;
static unsigned long stateStartTime = 0;

static const char* state_name(RobotState s) {
  switch (s) {
    case STATE_FORWARD:   return "FORWARD  ";
    case STATE_STOP:      return "STOP     ";
    case STATE_REVERSING: return "REVERSING";
    case STATE_TURNING:   return "TURNING  ";
    default:              return "UNKNOWN  ";
  }
}

// ── setup() ──────────────────────────────────────────────────
void setup() {
  Serial.begin(SERIAL_BAUD);
  motors_init();
  ultrasonic_init();
  encoder_init();

  // Feedforward gain (PID_KFF) passed as 4th argument
  pid_init(&pidLeft,
           PID_KP, PID_KI, PID_KD, PID_KFF,
           PID_INTEGRAL_LIMIT, 0.0f, 255.0f);
  pid_init(&pidRight,
           PID_KP, PID_KI, PID_KD, PID_KFF,
           PID_INTEGRAL_LIMIT, 0.0f, 255.0f);

  Serial.println(F("=== v4: Feedforward + PID Robot ==="));
  Serial.print(F("Kff: "));   Serial.print(PID_KFF);
  Serial.print(F("  Target: ")); Serial.print(TARGET_SPEED_PULSES);
  Serial.println(F(" pulses/100ms"));
  delay(1000);
}

// ── loop() ───────────────────────────────────────────────────
void loop() {
  unsigned long now      = millis();
  long          distance = ultrasonic_read_cm();

  // PID update at fixed interval
  if (now - lastPIDUpdate >= PID_INTERVAL_MS) {
    long speedL = encoder_get_speed(MOTOR_LEFT);
    long speedR = encoder_get_speed(MOTOR_RIGHT);

    if (currentState == STATE_FORWARD) {
      pwmLeft  = pid_compute(&pidLeft,  TARGET_SPEED_PULSES, (float)speedL, DT_S);
      pwmRight = pid_compute(&pidRight, TARGET_SPEED_PULSES, (float)speedR, DT_S);
      motors_set((uint8_t)pwmLeft, true, (uint8_t)pwmRight, true);
    }

    Serial.print(F("State:")); Serial.print(state_name(currentState));
    Serial.print(F(" L:"));    Serial.print(speedL);
    Serial.print(F(" R:"));    Serial.print(speedR);
    Serial.print(F(" pwmL:")); Serial.print((int)pwmLeft);
    Serial.print(F(" pwmR:")); Serial.print((int)pwmRight);
    Serial.print(F(" dist:")); Serial.print(distance);
    Serial.println(F(" cm"));

    lastPIDUpdate = now;
  }

  // State machine
  switch (currentState) {
    case STATE_FORWARD:
      if (distance > 0 && distance < STOP_DISTANCE) {
        motors_stop();
        pid_reset(&pidLeft); pid_reset(&pidRight);
        encoder_reset_all();
        currentState   = STATE_STOP;
        stateStartTime = now;
      }
      break;
    case STATE_STOP:
      if (now - stateStartTime >= STOP_PAUSE_MS) {
        motors_backward(REVERSE_SPEED);
        currentState   = STATE_REVERSING;
        stateStartTime = now;
      }
      break;
    case STATE_REVERSING:
      if (now - stateStartTime >= REVERSE_TIME_MS) {
        motors_stop(); delay(100);
        motors_turn_right(TURN_SPEED);
        currentState   = STATE_TURNING;
        stateStartTime = now;
      }
      break;
    case STATE_TURNING:
      if (now - stateStartTime >= TURN_TIME_MS) {
        motors_stop();
        pid_reset(&pidLeft); pid_reset(&pidRight);
        encoder_reset_all();
        currentState = STATE_FORWARD;
      }
      break;
  }
}
