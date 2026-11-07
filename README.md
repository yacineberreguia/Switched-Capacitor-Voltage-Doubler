# Bootstrap-Referenced Switched-Capacitor Voltage Doubler

Despite completing the hardware implementation and extensive debugging, the prototype has not yet achieved its intended output performance effeciency


## Overview

An inductorless 9V → ~18V voltage doubler built from a half-bridge gate driver
(IR2104) and two power MOSFETs (IRLZ44N) logic level compatible for the pico,  using a flying capacitor and two
Schottky diodes as the actual voltage-multiplication stage — the same
bootstrap principle that normally powers a half-bridge driver's own high-side
gate, repurposed here to double the output rail instead.

A Raspberry Pi Pico generates the switching signal and displays live Vin,
Vout, and the resulting ratio on an OLED for bench verification.

## Goal

Build and characterize a switched-capacitor (charge pump) voltage doubler as
an alternative to inductor-based boost conversion

## Full Bill of Materials (if you wanna make it)

| Qty | Part | Role |
|---|---|---|
| 1 | IR2104 (DIP8) | Half-bridge gate driver |
| 2 | IRLZ44N | Logic-level N-MOSFET, half-bridge |
| 3 | 1N5819 Schottky diode | Bootstrap (D3) + doubler (D1, D2) |
| 1 | 1µF 50V cap | Bootstrap cap (VB–VS) |
| 1 | 2.2µF 50V cap | Flying capacitor (Cfly) |
| 2 | 4.7µF 50V cap | Output filter, in parallel |
| 2 | 10Ω resistor | HO/LO gate series resistors |
| 2 | 10kΩ resistor | Gate-Source pulldowns (Q1, Q2) |
| 1 | 10kΩ resistor | SD pin pull-up to VDD |
| 1 | 100Ω resistor | GP15 → IN series resistor |
| 1 | 10kΩ + 1 × 2.2kΩ | Vout sense divider |
| 1 | 10kΩ + 1 × 4.7kΩ | Vin sense divider |
| 1 | Raspberry Pi Pico | PWM generation + ADC monitoring |
| 1 | SSD1306 128×64 OLED (I2C) | Live telemetry display |
| 1 | 9V battery | Power source under test |