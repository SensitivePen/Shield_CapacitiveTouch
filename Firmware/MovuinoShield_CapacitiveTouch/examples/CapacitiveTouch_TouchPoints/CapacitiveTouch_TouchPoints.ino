#include "_CapacitiveTouch.h"

#define NUM_TX_USED             4
#define NUM_RX_USED             6

#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN                 21
#define SCL_PIN                 22
#endif

#define GAIN                    50

CapacitiveTouch capa;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  if (DEBUG) Serial.println("CapacitiveTouch get data in zCCxAAAxAAAx...xAAA");
    #ifdef _ESP32_HAL_I2C_H_ // For ESP32
  Wire.begin(SDA_PIN, SCL_PIN);
  capa.setWire(&Wire);
  #endif
  capa.init();
  capa.setGain(GAIN);
  delay(100);
  capa.calibrate(0.);
}

void loop() {
  if(capa.updated()) {
    for (int txAddr=0;txAddr<NUM_TX_USED;txAddr++){
      // Write the row reference on serial (zCCxAAAxAAAx...xAAA)
      Serial.print("z"); // start
      Serial.print(txAddr); // the index of the column
      for(int rxAddr=0;rxAddr<NUM_RX_USED;rxAddr++){
          Serial.print("x"); // "x" used as seperator
          Serial.print(capa.grid[txAddr][rxAddr]);
      }
      Serial.println("");
    }
  }
  delay(10);
}