#include "_CapacitiveTouch.h"

#define NUM_TX_USED             4
#define NUM_RX_USED             6

CapacitiveTouch capa=CapacitiveTouch();

void setup() {
  Serial.begin(115200);
  while (!Serial);
  if (DEBUG) Serial.println("CapacitiveTouch get data in zCCxAAAxAAAx...xAAA");
  capa.init();
  delay(100);
  capa.calibrate();
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
  delay(50);
}