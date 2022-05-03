# Movuino Capacitive Touch Shield
## Table of content
* [Installation](#installation)
* [Setup](#setup)
* [Examples](#examples)

## Installation
This project use Arduino IDE 2.0. To install the latest version, check their [website](https://www.arduino.cc/en/software).

## Setup
Depending on the controller (Movuino, Arduino Uno, ...), which collects the data of the shield, the I2C address of the FT5316 may change. We recommend checking the I2C addresses connected to the microcontroller before starting the examples. Upload the script from `\I2C_Scanner` and see the addr on the serial monitor.
> ***NOTE:*** Generally, the address of the MPU9250 of the Movuino is set at **0x69** and the FT5316 at **0x38**.

Change the `I2C_ADDRESS` in the header files of the corresponding libraries (*e.g.* `_CapacitiveTouch.h` or `MPU9250.h`) if needed.

## Examples
For this shield, we have differents example depending on the tasks:

### CapacitiveTouch_Raw
This program allow us to quickly check if the shield (communication and FT5316) works fine. It prints in raw the data: _e.g._ `8267,8513,8368,8127,8095,10246,6772,8603,8552,8262,8270,10256,8641,8726,8455,8155,7828,9874,8822,8922,8723,8389,8361,10233` 

### CapacitiveTouch_Raw_AutoCalibration
This program allow us to see values close to 0 after calibrating the raw data. The calibration is done by calculating the mean of the first elements. You can add a threshold directly in the header and change the number of elements for the mean (`CALIBRATION_MAX`).

### CapacitiveTouch_TouchPoints
This program allow us to visualize the data on a matrix with Processing. The Movuino prints the data with the following structure (see Fig. 1): `zCCxAAAxAAAx...xAAA`
> ***NOTE:*** `z` for start, `CC` for index of the column, `AAA` for data coming from the capacitiv touch controller (FT5316) and `x` for seperator.

![TouchPoints.png](https://github.com/SensitivePen/Shield_CapacitiveTouch/blob/wim-dev/docs/images/CapacitiveTouch_TouchPointsProcessing.png)

Fig. 1 Overview of TouchPoints with Processing