/**
 * encoder.cpp
 * -----------
 * ISR-based pulse counting for two optical wheel encoders.
 *
 * Why volatile?
 *   Variables modified inside an ISR must be declared volatile.
 *   Without it, the compiler may cache the value in a CPU register
 *   and the main loop will never observe ISR updates, producing
 *   a counter that appears permanently zero.
 *
 * Why noInterrupts() during read?
 *   The ATmega328P is an 8-bit processor. A long (32-bit) variable
 *   cannot be read or written in a single instruction. An ISR firing
 *   between the two 16-bit halves of the read would produce a
 *   corrupted value. Disabling interrupts for the ~4 CPU cycles
 *   needed to copy and reset the counter prevents this race condition.
 *
 * Author : [Your Name]
 * Date   : 2026
 * License: MIT
 */

#include "encoder.h"

// ── ISR-modified pulse counters ───────────────────────────────
static volatile long _leftPulses  = 0;
static volatile long _rightPulses = 0;

// ── Interrupt Service Routines ────────────────────────────────
static void leftISR()  { _leftPulses++;  }
static void rightISR() { _rightPulses++; }

// ── Public API ────────────────────────────────────────────────

void encoder_init() {
  pinMode(ENCODER_LEFT_PIN,  INPUT_PULLUP);
  pinMode(ENCODER_RIGHT_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT_PIN),
                  leftISR,  RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT_PIN),
                  rightISR, RISING);
}

long encoder_get_speed(MotorSide side) {
  long count;
  if (side == MOTOR_LEFT) {
    noInterrupts();
    count        = _leftPulses;
    _leftPulses  = 0;
    interrupts();
  } else {
    noInterrupts();
    count         = _rightPulses;
    _rightPulses  = 0;
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
