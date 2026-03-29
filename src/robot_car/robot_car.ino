/**
 * ============================================================
 *  Arduino 2WD Obstacle-Avoidance Robot Car
 * ============================================================
 *  Hardware:
 *    - Arduino Uno R3
 *    - L298N Dual H-Bridge Motor Driver
 *    - HC-SR04 Ultrasonic Distance Sensor
 *    - 2x TT DC Gear Motors (1:48, 6V)
 *    - 9V Battery
 *
 *  Behaviour:
 *    - Drives forward continuously
 *    - Detects obstacles within STOP_DISTANCE (cm)
 *    - Reverses briefly, turns right, then resumes
 *
 *  Pin Map:
 *    Motor A (Left)  : ENA=9 (PWM), IN1=8, IN2=7
 *    Motor B (Right) : ENB=6 (PWM), IN3=5, IN4=4
 *    HC-SR04         : TRIG=13, ECHO=12
 *
 *  Author : [Your Name]
 *  Date   : 2026
 *  License: MIT
 * ============================================================
 */

#include "config.h"
#include "motors.h"
#include "ultrasonic.h"

// ── State Machine ────────────────────────────────────────────
enum RobotState {
  STATE_FORWARD,
  STATE_STOP,
  STATE_REVERSING,
  STATE_TURNING
};

RobotState currentState = STATE_FORWARD;
unsigned long stateStartTime = 0;

// ── setup() ─────────────────────────────────────────────────
void setup() {
  Serial.begin(SERIAL_BAUD);
  motors_init();
  ultrasonic_init();

  Serial.println(F("=== Arduino Robot Car ==="));
  Serial.println(F("Initialised. Starting in 1 second..."));
  delay(1000);
}

// ── loop() ──────────────────────────────────────────────────
void loop() {
  unsigned long now = millis();
  long distance = ultrasonic_read_cm();

  // Debug output every 200 ms
  static unsigned long lastPrint = 0;
  if (now - lastPrint >= 200) {
    Serial.print(F("Distance: "));
    Serial.print(distance);
    Serial.print(F(" cm | State: "));
    Serial.println(state_name(currentState));
    lastPrint = now;
  }

  switch (currentState) {

    case STATE_FORWARD:
      motors_forward(DRIVE_SPEED);
      if (distance > 0 && distance < STOP_DISTANCE) {
        motors_stop();
        currentState = STATE_STOP;
        stateStartTime = now;
        Serial.print(F("Obstacle at "));
        Serial.print(distance);
        Serial.println(F(" cm — stopping."));
      }
      break;

    case STATE_STOP:
      if (now - stateStartTime >= STOP_PAUSE_MS) {
        motors_backward(REVERSE_SPEED);
        currentState = STATE_REVERSING;
        stateStartTime = now;
        Serial.println(F("Reversing..."));
      }
      break;

    case STATE_REVERSING:
      if (now - stateStartTime >= REVERSE_TIME_MS) {
        motors_stop();
        delay(100);
        motors_turn_right(TURN_SPEED);
        currentState = STATE_TURNING;
        stateStartTime = now;
        Serial.println(F("Turning right..."));
      }
      break;

    case STATE_TURNING:
      if (now - stateStartTime >= TURN_TIME_MS) {
        motors_stop();
        currentState = STATE_FORWARD;
        Serial.println(F("Resuming forward."));
      }
      break;
  }
}

// ── Helper: state name for serial debug ─────────────────────
const char* state_name(RobotState s) {
  switch (s) {
    case STATE_FORWARD:   return "FORWARD";
    case STATE_STOP:      return "STOP";
    case STATE_REVERSING: return "REVERSING";
    case STATE_TURNING:   return "TURNING";
    default:              return "UNKNOWN";
  }
}
