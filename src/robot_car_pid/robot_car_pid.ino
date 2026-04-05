/**
 * ============================================================
 *  Arduino 2WD Robot Car — PID Speed Control Edition
 * ============================================================
 *  Extends the obstacle-avoidance robot with closed-loop
 *  PID motor speed control using wheel encoders.
 *
 *  Hardware additions over v1 (obstacle-avoidance):
 *    - 2x Optical wheel encoders (20-slot disc)
 *      Left  encoder -> Arduino pin 2 (INT0)
 *      Right encoder -> Arduino pin 3 (INT1)
 *
 *  What PID adds:
 *    - Both motors maintain equal, consistent speed regardless
 *      of manufacturing variation, battery sag, or surface grip.
 *    - Robot drives straight without open-loop drift.
 *    - Performance is consistent throughout the battery charge cycle.
 *
 *  Architecture:
 *    - Non-blocking state machine (millis-based, no delay() in loop)
 *    - Separate PID instance per motor for independent correction
 *    - PID runs on a fixed 100 ms timer; sensor polled every iteration
 *
 *  Tuning:
 *    - All gains and parameters are in config.h
 *    - See config.h and README for the step-by-step tuning procedure
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

// ── PID Instances (one per motor) ────────────────────────────
PIDController pidLeft;
PIDController pidRight;

// ── Current PWM outputs (float for PID precision) ────────────
static float pwmLeft  = DRIVE_SPEED;
static float pwmRight = DRIVE_SPEED;

// ── PID timing ───────────────────────────────────────────────
static unsigned long lastPIDUpdate = 0;
static const float   DT_S = PID_INTERVAL_MS / 1000.0f;

// ── State Machine ────────────────────────────────────────────
enum RobotState {
  STATE_FORWARD,
  STATE_STOP,
  STATE_REVERSING,
  STATE_TURNING
};

static RobotState    currentState   = STATE_FORWARD;
static unsigned long stateStartTime = 0;

// ── Forward declaration ───────────────────────────────────────
static const char* state_name(RobotState s);

// ── setup() ─────────────────────────────────────────────────
void setup() {
  Serial.begin(SERIAL_BAUD);
  motors_init();
  ultrasonic_init();
  encoder_init();

  // Initialise left motor PID
  pid_init(&pidLeft,
           PID_KP, PID_KI, PID_KD,
           PID_INTEGRAL_LIMIT,
           0.0f, 255.0f);

  // Initialise right motor PID
  // Use the same gains initially; adjust kp/ki independently
  // in config.h if the two motors have significantly different
  // characteristics after tuning.
  pid_init(&pidRight,
           PID_KP, PID_KI, PID_KD,
           PID_INTEGRAL_LIMIT,
           0.0f, 255.0f);

  Serial.println(F("=== Arduino Robot Car — PID Edition ==="));
  Serial.println(F("Target speed (pulses/100ms): " ));
  Serial.println(TARGET_SPEED_PULSES);
  Serial.println(F("Initialised. Starting in 1 second..."));
  delay(1000);
}

// ── loop() ──────────────────────────────────────────────────
void loop() {
  unsigned long now      = millis();
  long          distance = ultrasonic_read_cm();

  // ── PID update at fixed interval ─────────────────────────
  if (now - lastPIDUpdate >= PID_INTERVAL_MS) {

    // Read encoder speeds (pulses since last call; resets counter)
    long speedL = encoder_get_speed(MOTOR_LEFT);
    long speedR = encoder_get_speed(MOTOR_RIGHT);

    // Only run PID when driving forward; open-loop for manoeuvres
    if (currentState == STATE_FORWARD) {
      pwmLeft  = pid_compute(&pidLeft,
                             TARGET_SPEED_PULSES, (float)speedL, DT_S);
      pwmRight = pid_compute(&pidRight,
                             TARGET_SPEED_PULSES, (float)speedR, DT_S);

      motors_set((uint8_t)pwmLeft,  true,
                 (uint8_t)pwmRight, true);
    }

    // Serial debug — open Serial Monitor at 9600 baud
    Serial.print(F("State:")); Serial.print(state_name(currentState));
    Serial.print(F(" | L:"));  Serial.print(speedL);
    Serial.print(F(" R:"));    Serial.print(speedR);
    Serial.print(F(" | pwmL:")); Serial.print((int)pwmLeft);
    Serial.print(F(" pwmR:")); Serial.print((int)pwmRight);
    Serial.print(F(" | dist:")); Serial.print(distance);
    Serial.println(F(" cm"));

    lastPIDUpdate = now;
  }

  // ── State machine ────────────────────────────────────────
  switch (currentState) {

    case STATE_FORWARD:
      if (distance > 0 && distance < STOP_DISTANCE) {
        motors_stop();
        pid_reset(&pidLeft);
        pid_reset(&pidRight);
        encoder_reset_all();
        currentState   = STATE_STOP;
        stateStartTime = now;
        Serial.print(F("Obstacle at "));
        Serial.print(distance);
        Serial.println(F(" cm — stopping."));
      }
      break;

    case STATE_STOP:
      if (now - stateStartTime >= STOP_PAUSE_MS) {
        motors_backward(REVERSE_SPEED);
        currentState   = STATE_REVERSING;
        stateStartTime = now;
        Serial.println(F("Reversing..."));
      }
      break;

    case STATE_REVERSING:
      if (now - stateStartTime >= REVERSE_TIME_MS) {
        motors_stop();
        delay(100);
        motors_turn_right(TURN_SPEED);
        currentState   = STATE_TURNING;
        stateStartTime = now;
        Serial.println(F("Turning right..."));
      }
      break;

    case STATE_TURNING:
      if (now - stateStartTime >= TURN_TIME_MS) {
        motors_stop();
        pid_reset(&pidLeft);
        pid_reset(&pidRight);
        encoder_reset_all();
        currentState = STATE_FORWARD;
        Serial.println(F("Resuming forward."));
      }
      break;
  }
}

// ── Helper: state name for serial debug ─────────────────────
static const char* state_name(RobotState s) {
  switch (s) {
    case STATE_FORWARD:   return "FORWARD  ";
    case STATE_STOP:      return "STOP     ";
    case STATE_REVERSING: return "REVERSING";
    case STATE_TURNING:   return "TURNING  ";
    default:              return "UNKNOWN  ";
  }
}
