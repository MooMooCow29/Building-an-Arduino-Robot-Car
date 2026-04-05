# Wiring Diagram

## Connection Overview

```
                    ┌───────────────────────────────┐
                    │          Arduino Uno           │
                    │                               │
                    │  Pin 9  (PWM) ─────────────── │─── ENA   ┐
                    │  Pin 8        ─────────────── │─── IN1   │
                    │  Pin 7        ─────────────── │─── IN2   │  L298N
                    │  Pin 6  (PWM) ─────────────── │─── ENB   │  Motor
                    │  Pin 5        ─────────────── │─── IN3   │  Driver
                    │  Pin 4        ─────────────── │─── IN4   │
                    │                               │          │
                    │  Pin 13 ─────────────────── Trig  HC-SR04│
                    │  Pin 12 ─────────────────── Echo        │
                    │                               │          │
                    │  5V ──── L298N VSS ────────── │          │
                    │  5V ──── HC-SR04 VCC          │          │
                    │  GND ─── L298N GND ─── Bat(–)─│          │
                    │  GND ─── HC-SR04 GND          │          │
                    └───────────────────────────────┘          │
                                                               │
              ┌────────────────────────────────────────────────┘
              │
              │    OUT1 ──── Left Motor (+)
              │    OUT2 ──── Left Motor (–)
              │    OUT3 ──── Right Motor (+)
              │    OUT4 ──── Right Motor (–)
              │
              │    VS  ──── Battery (+) 9V
              │    GND ──── Battery (–) ──── Arduino GND
              │
              └─── (onboard 5V reg feeds back to Arduino 5V)
```

## Power Rail Summary

```
9V Battery (+) ──── L298N VS
9V Battery (–) ──── L298N GND
                ──── Arduino GND    ← MUST be connected

L298N 5V out   ──── Arduino 5V pin  (only valid when VS ≤ 12V)
Arduino 5V     ──── HC-SR04 VCC
Arduino 5V     ──── L298N VSS (logic supply)

Common GND:  Battery(–) = L298N GND = Arduino GND = HC-SR04 GND
```

## Notes

- Keep motor power wires (VS, GND, OUT1–OUT4) physically separated
  from signal wires (IN1–IN4, ENA, ENB, Trig, Echo) to reduce noise.
- Twist the motor wire pairs together to reduce EMI.
- Add a 100 µF electrolytic capacitor across the battery terminals
  (close to the L298N VS/GND pins) to absorb motor inrush spikes.
- The L298N 5V jumper must be **present** when VS ≤ 12V to enable
  the onboard regulator. Remove it if using VS > 12V to avoid damage.
