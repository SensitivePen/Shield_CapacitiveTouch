// https://www.newhavendisplay.com/appnotes/datasheets/touchpanel/FT5x16_registers.pdf
// https://www.buydisplay.com/download/ic/FT5206.pdf
// http://optisimon.com/raspberrypi/touch/ft5406/2016/07/13/raspberry-pi-7-inch-touchscreen-hacking/

// https://github.com/azzazza/patch_kernel_q415/blob/master/drivers/input/touchscreen/ft5x06_ts.c
// https://github.com/optisimon/ft5406-capacitive-touch/blob/master/CapacitanceVisualizer/FT5406.hpp
// https://github.com/hyvapetteri/touchscreen-cardiography

#include <Arduino.h>
#include <Wire.h>

#define I2C_ADDRESS         0x38

#define DEBUG               true

// RAW
#define NUM_TX             21
#define NUM_RX             12

class CapacitiveTouch {
    public:
    const uint8_t address;
    CapacitiveTouch(uint8_t address=I2C_ADDRESS);
    void setWire(TwoWire *wire);
    void setGain(uint8_t val);
    bool init();
    bool isInit = false;

    bool updated();
    unsigned int getRawData(int rx, int tx);

    // RAW
    short grid[NUM_TX][NUM_RX];
    short calibrateGrid[NUM_TX][NUM_RX];
    void calibrate(float variance);

    private:
    TwoWire* myWire;
    void autoCalib();
    void getRawData();
    int calibrationSteps = 0;
    bool calibrationDone = false;
    void beginWireIfNull();
    uint8_t i2cWriteByte(uint8_t Address, uint8_t Register, uint8_t Data);
    uint8_t i2cRead(uint8_t Address, uint8_t Register, uint8_t NBytes, byte *Data);
};
