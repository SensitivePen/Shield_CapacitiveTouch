#include "_CapacitiveTouch.h"

#define NUM_TX_USED             20
#define NUM_RX_USED             10

#define GAIN                    50

CapacitiveTouch capa=CapacitiveTouch();

void setup() {
  Serial.begin(115200);
  while (!Serial);
  if (DEBUG) Serial.println("CapacitiveTouch get Raw data with calibration");
  capa.init();
  capa.setGain(GAIN);
  delay(100);
  capa.calibrate();
}

void loop() {
  String raw="";
  if(capa.updated()) {
    for (int txAddr=0;txAddr<NUM_TX_USED;txAddr++){
      for(int rxAddr=0;rxAddr<NUM_RX_USED;rxAddr++){
          raw+=10+abs(capa.grid[txAddr][rxAddr]);
          raw+=",";
      }
    }
    raw.remove(raw.length()-1);
    Serial.println(raw);
  }
  delay(10);
}