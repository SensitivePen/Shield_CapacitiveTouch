#include "_CapacitiveTouch.h"

#define MODE_TEST           0x40

#define CALIBRATION_MAX     15
#define CALIB_THRESHOLD     20 

CapacitiveTouch::CapacitiveTouch(uint8_t address):
  address(address){
    myWire = NULL;
}

//// ============================== SETUP ==============================

void CapacitiveTouch::setWire(TwoWire* wire){
  myWire=wire;
}

void CapacitiveTouch::beginWireIfNull(){
  if (myWire==NULL){
    myWire=&Wire;
    myWire->begin();
    myWire->setClock(400000); // https://www.arduino.cc/en/Reference/WireSetClock
  }
}

//// ============================== INIT ==============================

bool CapacitiveTouch::init(){
    beginWireIfNull();

    // Initialization: set device mode to test mode
    uint8_t stat = i2cWriteByte(I2C_ADDRESS,0x00,MODE_TEST);

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
    return isInit;
}

// NOT working -> calibration not finished
void CapacitiveTouch::autoCalib(){
    if (DEBUG) Serial.println("Autocalibration start.");
    beginWireIfNull();
    i2cWriteByte(I2C_ADDRESS,0x02,0x04);

    delay(300);
    for(int i=0;i<100;i++){
        byte reading[1];
        i2cRead(I2C_ADDRESS, 0x00, 1, reading);

        if ( ((reading[0] & 0x70) >> 4) == 0x00)  // calibration finish
        {
            if (DEBUG) Serial.println("Autocalibration done.");
            break;
        }
        delay(100);
    }
    // save calibration result
    i2cWriteByte(I2C_ADDRESS,0x02,0x05);
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
    beginWireIfNull();
    // Start scan
    i2cWriteByte(I2C_ADDRESS,0x00,0xc0);

    // Read data
    for(unsigned int txAddr=0;txAddr<NUM_TX;txAddr++){
        byte result[2 * NUM_RX];

        // Start transmission
        uint8_t st = i2cWriteByte(I2C_ADDRESS,0x01,txAddr);
        if (st < 0 && DEBUG) Serial.println("i2c write failed");

        delayMicroseconds(100); // Wait at least 50us

        uint8_t r = i2cRead(I2C_ADDRESS, 0x10, 2*NUM_RX, result);

        if (r>0 && DEBUG) Serial.println("PROBLEM");

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

//// ============================== GAIN ==============================

void CapacitiveTouch::setGain(uint8_t gain) {
    i2cWriteByte(I2C_ADDRESS,0x07,gain);
    if (DEBUG) Serial.println("Gain set to "+String(gain));
}

//// ============================== WRITE ==============================

uint8_t CapacitiveTouch::i2cWriteByte(uint8_t Address, uint8_t Register, uint8_t Data){
    myWire->beginTransmission(Address);
    myWire->write(Register);
    myWire->write(Data);
    return myWire->endTransmission(Address);

}

//// ============================== READ ==============================

uint8_t CapacitiveTouch::i2cRead(uint8_t Address, uint8_t Register, uint8_t NBytes, byte *Data){
  myWire->beginTransmission(Address);
  myWire->write(Register);
  uint8_t result = myWire->endTransmission();
  if (result != 0) return result;
  myWire->requestFrom(Address,NBytes);
  uint8_t index = 0;
  while(myWire->available()){
    byte d = myWire->read();
    if (index<NBytes){
      Data[index++] = d;
    }
  }
  return 0;
}