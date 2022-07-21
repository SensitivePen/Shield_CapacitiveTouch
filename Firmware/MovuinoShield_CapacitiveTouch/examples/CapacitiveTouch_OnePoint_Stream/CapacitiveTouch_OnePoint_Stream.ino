#include "_CapacitiveTouch.h"

#define NUM_TX_USED             6
#define NUM_RX_USED             4

#define TX_USED                 6
#define RX_USED                 1

#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN                 21
#define SCL_PIN                 22
#endif

#define GAIN                    50

unsigned int data=0;

CapacitiveTouch capa;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  if (DEBUG) Serial.println("CapacitiveTouch get Raw data");

  #ifdef _ESP32_HAL_I2C_H_ // For ESP32
  Wire.begin(SDA_PIN, SCL_PIN);
  capa.setWire(&Wire);
  #endif

  capa.init();
  capa.setGain(GAIN);
  delay(100);
}

void loop() {
  data=capa.getRawData(RX_USED,TX_USED);
  Serial.println(data);
  delay(10);
}
