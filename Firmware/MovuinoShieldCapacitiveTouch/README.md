# Movuino Capacitive Touch Shield
## Table of content
* [Installation](#installation)
* [Setup](#setup)
* [Examples](#examples)

## Installation
This project use Arduino IDE 2.0. To install the latest version, check their [website](https://www.arduino.cc/en/software).

## Setup
Depending on the controller (Movuino, Arduino Uno, ...), which collects the data of the shield, the I2C address of the FT5316 may change. We recommend checking the I2C addresses connected to the microcontroller before starting the examples. Upload the script from `\I2C_Scanner` and see the addr on the serial monitor.
> ***NOTE:*** Generally, the address of the MPU9250 of the Movuino is set at **0x69** and the FT5316 at **0x76**.

Change the `I2C_ADDRESS` in the header files of the corresponding libraries (*e.g.* `_CapacitiveTouch.h` or `MPU9250.h`) if needed.

## Examples
For this shield, we have differents example depending on the tasks:

### CapacitiveTouch_Raw
This program allow us to quickly check if the shield (communication and FT5316) works fine. It prints in raw the data: `2,324,,12,,,543,435,,3;`