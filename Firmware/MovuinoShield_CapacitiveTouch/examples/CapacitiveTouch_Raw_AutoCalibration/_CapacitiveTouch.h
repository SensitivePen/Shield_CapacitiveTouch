// https://www.newhavendisplay.com/appnotes/datasheets/touchpanel/FT5x16_registers.pdf
// https://www.buydisplay.com/download/ic/FT5206.pdf
// http://optisimon.com/raspberrypi/touch/ft5406/2016/07/13/raspberry-pi-7-inch-touchscreen-hacking/

// https://github.com/azzazza/patch_kernel_q415/blob/master/drivers/input/touchscreen/ft5x06_ts.c
// https://github.com/optisimon/ft5406-capacitive-touch/blob/master/CapacitanceVisualizer/FT5406.hpp
// https://github.com/hyvapetteri/touchscreen-cardiography

#include "Wire.h"

const uint16_t I2C_ADDRESS = 0x38;

#define MODE_TEST           0x40
#define DEBUG               false

// RAW
#define NUM_TX             21
#define NUM_RX             12


#define CALIBRATION_MAX     15
#define CALIB_THRESHOLD     0 

class CapacitiveTouch {
    public:
    CapacitiveTouch();
    bool init();
    bool isInit = false;

    bool updated();

    // RAW
    short grid[NUM_TX][NUM_RX];
    short calibrateGrid[NUM_TX][NUM_RX];
    void calibrate(float variance);
    void setGain(int val);


    private:
    void autoCalib();
    void getRawData();
    int calibrationSteps = 0;
    bool calibrationDone = false;
};

CapacitiveTouch::CapacitiveTouch(){}

//// ============================== INIT ==============================

bool CapacitiveTouch::init(){

    // Setup I2C
    //digitalWrite(SDA,LOW);
    //digitalWrite(SCL,LOW);

    Wire.begin();
    Wire.setClock(100000); // 400000 https://www.arduino.cc/en/Reference/WireSetClock

    // Initialization: set device mode to test mode
    int stat = 0;
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(byte(0x00));
    Wire.write(byte(MODE_TEST));
    stat = Wire.endTransmission(I2C_ADDRESS);

    if(stat==0) isInit = true;
    else if(DEBUG) {
        Serial.print("CapacitiveTouch status:");
        if (stat == 1 ) Serial.println("1: too long for the buffer");
        if (stat == 2 ) Serial.println("2: NACK  transmit");
        if (stat == 3 ) Serial.println("3: NACK transmit data");
        if (stat == 4 ) Serial.println("4: other ");
    }

    if(DEBUG && isInit) Serial.println("CapacitiveTouch initialized.");
    else if (DEBUG) Serial.println("CapacitiveTouch not init");
    
    // Init calibrateGrid to zero
    for (unsigned int rxAddr=0;rxAddr<NUM_RX;rxAddr++){
        for(unsigned int txAddr=0;txAddr<NUM_TX;txAddr++){
                calibrateGrid[txAddr][rxAddr]=0;
        }
    }
    // setGain(100);
    return isInit;
}

// NOT working -> calibration not finished
void CapacitiveTouch::autoCalib(){
    if (DEBUG) Serial.println("Autocalibration start.");
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(byte(0x2));
    Wire.write(byte(0x4));
    Wire.endTransmission(I2C_ADDRESS);

    delay(300);
    for(int i=0;i<100;i++){
        Wire.beginTransmission(I2C_ADDRESS);
        Wire.write(0x00);
        Wire.requestFrom(I2C_ADDRESS,uint8_t(1),false);

        byte reading = Wire.read();

        if ( ((reading & 0x70) >> 4) == 0x0)  // calibration finish
        {
            if (DEBUG) Serial.println("Autocalibration done.");
            break;
        }
        delay(100);
    }
    // save calibration result
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(byte(0x02));
    Wire.write(byte(0x5));
    Wire.endTransmission(I2C_ADDRESS);
}

//// ============================== UPDATE ==============================

bool CapacitiveTouch::updated(){
    if (!isInit) return false;
    else {
        getRawData();
        return true;
  }
}

void CapacitiveTouch::getRawData(){

    // Start scan
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(byte(0x00));
    Wire.write(byte(0xc0));
    Wire.endTransmission();

    // Read data
    for(unsigned int txAddr=0;txAddr<NUM_TX;txAddr++){
        byte result[2 * NUM_RX];

        // Start transmission
        Wire.beginTransmission(I2C_ADDRESS);
        Wire.write(byte(0x01));
        Wire.write(txAddr);
        unsigned int st = Wire.endTransmission();
        if (st < 0 && DEBUG) Serial.println("i2c write failed");

        delayMicroseconds(100); // Wait at least 50us

        Wire.beginTransmission(I2C_ADDRESS);
        Wire.write(0x10);
        Wire.endTransmission(false);
        Wire.requestFrom(I2C_ADDRESS,uint8_t(2*NUM_RX),false);
        unsigned int g = 0;
        while (Wire.available()) {
            result[g++] = Wire.read();
        }

        for (unsigned int rxAddr=0;rxAddr<NUM_RX;rxAddr++){
            unsigned int output = (result[2 * rxAddr] << 8) | (result[2 * rxAddr + 1]);

            if(calibrationSteps == CALIBRATION_MAX){
                grid[NUM_TX-1-txAddr][rxAddr]=output-calibrateGrid[NUM_TX-1-txAddr][rxAddr]+CALIB_THRESHOLD;
            } else {
                grid[NUM_TX-1-txAddr][rxAddr]=output;
            }
        }
    }
}

//// ============================== CALIBRATION ==============================

void CapacitiveTouch::calibrate(float variance=0.){
    while (calibrationSteps != CALIBRATION_MAX){
        getRawData();
        if (grid[0][0] < 5000) return;
        if (calibrationSteps == 0 && !calibrationDone){
            memcpy(calibrateGrid,grid,sizeof(grid));
        } else {
                for (unsigned int rxAddr=0;rxAddr<NUM_RX;rxAddr++){
                    for(unsigned int txAddr=0;txAddr<NUM_TX;txAddr++){
                        calibrateGrid[txAddr][rxAddr]=(grid[txAddr][rxAddr]+calibrateGrid[txAddr][rxAddr])/2;
                    }
                }
        }
        calibrationSteps++;
    }
    calibrationDone = true;
    if (DEBUG) Serial.println("Calibration done");
    delay(1000);
}

void CapacitiveTouch::setGain(int gain) {
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(byte(0x07));
    Wire.write(byte(gain));
    Wire.endTransmission(I2C_ADDRESS);
    if(DEBUG){
        Serial.print("Gain set to ");
        Serial.println((gain));
    }
}
