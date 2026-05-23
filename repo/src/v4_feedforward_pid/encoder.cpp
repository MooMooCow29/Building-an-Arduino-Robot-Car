/**
 * encoder.cpp
 * -----------
 * ISR-based pulse counting for two optical wheel encoders.
 *
 * Author : [Your Name]
 * Date   : 2026
 * License: MIT
 */

#include "encoder.h"

static volatile long _leftPulses  = 0;
static volatile long _rightPulses = 0;

static void leftISR()  { _leftPulses++;  }
static void rightISR() { _rightPulses++; }

void encoder_init() {
  pinMode(ENCODER_LEFT_PIN,  INPUT_PULLUP);
  pinMode(ENCODER_RIGHT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT_PIN),  leftISR,  RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT_PIN), rightISR, RISING);
}

long encoder_get_speed(MotorSide side) {
  long count;
  if (side == MOTOR_LEFT) {
    noInterrupts();
    count       = _leftPulses;
    _leftPulses = 0;
    interrupts();
  } else {
    noInterrupts();
    count        = _rightPulses;
    _rightPulses = 0;
    interrupts();
  }
  return count;
}

void encoder_reset_all() {
  noInterrupts();
  _leftPulses  = 0;
  _rightPulses = 0;
  interrupts();
}
