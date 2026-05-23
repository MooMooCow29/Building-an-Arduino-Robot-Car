# Wiring Diagram

## Power Architecture

```
9V Battery (+) ──── L298N VS  (motor supply)
9V Battery (–) ──── L298N GND ──── Arduino GND  ← MUST be connected
L298N 5V out   ──── Arduino 5V pin
Arduino 5V     ──── HC-SR04 VCC
Arduino 5V     ──── L298N VSS (logic supply)

Common GND:  Battery(–) = L298N GND = Arduino GND = HC-SR04 GND
```

## Signal Connections

```
Arduino Pin 9  (PWM) ──── L298N ENA   (Left motor speed)
Arduino Pin 8        ──── L298N IN1   (Left motor dir A)
Arduino Pin 7        ──── L298N IN2   (Left motor dir B)
Arduino Pin 6  (PWM) ──── L298N ENB   (Right motor speed)
Arduino Pin 5        ──── L298N IN3   (Right motor dir A)
Arduino Pin 4        ──── L298N IN4   (Right motor dir B)
Arduino Pin 13       ──── HC-SR04 Trig
Arduino Pin 12       ──── HC-SR04 Echo
Arduino Pin 2 (INT0) ──── Left encoder signal   (v2/v3/v4 only)
Arduino Pin 3 (INT1) ──── Right encoder signal  (v2/v3/v4 only)
```

## Motor Connections

```
L298N OUT1 ──── Left motor  (+)
L298N OUT2 ──── Left motor  (–)
L298N OUT3 ──── Right motor (+)
L298N OUT4 ──── Right motor (–)
```

If either motor runs in the wrong direction, swap its wire pair
at the screw terminals (OUT1/OUT2 or OUT3/OUT4).

## L298N Direction Truth Table

| IN1 | IN2 | Motor A Behaviour |
|-----|-----|-------------------|
| HIGH | LOW | Forward |
| LOW | HIGH | Reverse |
| HIGH | HIGH | Brake |
| LOW | LOW | Coast |

Same logic applies to IN3/IN4 for Motor B.

## Notes

- Keep motor power wires physically separated from signal wires.
- Add 100µF electrolytic capacitor across battery terminals
  (close to L298N VS/GND) to absorb motor inrush current spikes.
- The L298N 5V jumper must be present when VS <= 12V.
  Remove it if using VS > 12V to avoid regulator damage.
