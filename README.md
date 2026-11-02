# CryoBox – Portable Cold-Chain Vaccine Carrier

Portable thermoelectric carrier designed to keep vaccines between **2 °C and 8 °C** during transport, using a PIC18F57Q43 microcontroller, Peltier cooling and PD temperature control.

Course project for *Microcontroladores* (NRC 554), Universidad Peruana de Ciencias Aplicadas (UPC), Lima, Peru.


## Problem

In Peru, vaccines sent to peripheral health centers are often transported in passive coolers (ice or dry ice) with no active temperature regulation, which risks breaking the 2–8 °C cold chain. CryoBox explores a low-cost, portable alternative with active temperature control.

## How it works

- **Sensing:** two DHT22 sensors inside the chamber; the system averages both readings to improve reliability.
- **Control:** a PD controller running on the PIC18F57Q43 computes a PWM duty cycle that regulates the cooling fan through a motor driver.
- **Display:** a 16x2 LCD shows average temperature and humidity in real time.
- **Alert:** two output pins (RA3, RA4) are activated when the average temperature exceeds 8 °C.
- **Safety:** if either sensor reports an error, the fan is switched off and the error codes are shown on the LCD.
- **Enclosure:** 3D-printed structure (263 x 218 x 300 mm, about 17.2 L) designed in Autodesk Fusion, with a central divider forming two chambers of about 8.6 L each.

## Hardware

| Component | Quantity |
|---|---|
| PIC18F57Q43 (Curiosity Nano) | 1 |
| Peltier cell | 1 |
| DHT22 temperature/humidity sensor | 2 |
| DC fan | 2 |
| Heat sink | 2 |
| Motor driver (H-bridge) | 1 |
| LCD 16x2 | 1 |
| 5 V power supply | 1 |

Total bill of materials: **S/474** (includes tools and 3D-printing filament). See the report for the full table.

## Repository contents

| Path | Description |
|---|---|
| `cryobox_main.c` | Main firmware (Embedded C, MPLAB X IDE, XC8) |
| `docs/` | Full project report (Spanish) |
| `images/` | Electrical diagram, 3D model and prototype photo |

## Build notes

- IDE: MPLAB X IDE with the XC8 compiler. Clock: 16 MHz internal oscillator.
- The firmware includes three local headers, `cabeza.h`, `LCD.h` and `DHT22.h`. **They are not included in this repository**, so the project does not compile as is. The main file is published to document the control logic described in the report.
- The temperature target is set by the `setpoint` variable, and the controller gains by `KP` and `KD`.

## Limitations

- No remote monitoring (no IoT or cloud connection).
- Cooling capacity is limited by the Peltier module and its heat dissipation.
- It does not replace large-scale hospital refrigeration without an external power source or battery.
- Opening detection with an infrared sensor was proposed in the report but is **not implemented** in the firmware.

## Team

- A. P. Montes Ricaldi
- E. A. Pomasonco Ventura
- E. J. Santillán Barrionuevo
- S. D. Beraún Ballarte

My part of the project: thermal theory (heat transfer, enclosure sizing, airflow estimate) and the innovation contributions section of the report.

## License

MIT
