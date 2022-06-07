#if !defined(_MOVUINOESP32_MPU9250_)
#define _MOVUINOESP32_MPU9250_

#include <Arduino.h>
#include <Wire.h>
#include <I2Cdev.h>
#include "MPU9250.h"

#define MPU_I2C_ADDRESS 0x69

class MovuinoMPU9250
{
private:
  MPU9250 _imu;

public:
  float ax, ay, az; // store accelerometre values
  float gx, gy, gz; // store gyroscope values
  float mx, my, mz; // store magneto values

  MovuinoMPU9250();
  ~MovuinoMPU9250();

  void begin();
  void update();
  void printData();

  void magnometerCalibration();
  void accelerometerCalibration();
  void gyroscopeCalibration();
};

#endif // _MOVUINOESP32_MPU9250_