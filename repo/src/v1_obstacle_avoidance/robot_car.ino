/**
 * ============================================================
 *  v1: Arduino 2WD Obstacle-Avoidance Robot (Open-Loop)
 * ============================================================
 *  Article: "Building an Arduino Robot Car from First Principles"
 *  Published: Level Up Coding — medium.com/@ks683557
 *  GitHub:    github.com/MooMooCow29/Building-an-Arduino-Robot-Car
 *
 *  Hardware:
 *    Arduino Uno R3, L298N Motor Driver, HC-SR04, 2x TT Motors
 *
 *  Behaviour:
 *    Drives forward. Detects obstacles within STOP_DISTANCE.
 *    Reverses, turns right, resumes. Non-blocking state machine.
 *
 *  Pin Map:
 *    Left  motor : ENA=9(PWM), IN1=8, IN2=7
 *    Right motor : ENB=6(PWM), IN3=5, IN4=4
 *    HC-SR04     : TRIG=13, ECHO=12
 *
 *  Author : [Your Name]
 *  Date   : 2026
 *  License: MIT
 * ============================================================
 */

#include "config.h"
#include "motors.h"
#include "ultrasonic.h"

// ── State Machine ─────────────────────────────────────────────
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
  Serial.println(F("=== v1: Obstacle Avoidance Robot ==="));
  Serial.println(F("github.com/MooMooCow29/Building-an-Arduino-Robot-Car"));
  delay(1000);
}

// ── loop() ───────────────────────────────────────────────────
void loop() {
  unsigned long now      = millis();
  long          distance = ultrasonic_read_cm();

  // Debug print every 200 ms
  static unsigned long lastPrint = 0;
  if (now - lastPrint >= 200) {
    Serial.print(F("State: ")); Serial.print(state_name(currentState));
    Serial.print(F(" | dist: ")); Serial.print(distance);
    Serial.println(F(" cm"));
    lastPrint = now;
  }

  switch (currentState) {

    case STATE_FORWARD:
      motors_forward(DRIVE_SPEED);
      if (distance > 0 && distance < STOP_DISTANCE) {
        motors_stop();
        currentState   = STATE_STOP;
        stateStartTime = now;
        Serial.print(F("Obstacle at ")); Serial.print(distance);
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
        currentState = STATE_FORWARD;
        Serial.println(F("Resuming forward."));
      }
      break;
  }
}
