#include "_CapacitiveTouch.h"

CapacitiveTouch capa=CapacitiveTouch();

void setup() {
  Serial.begin(9600);
  capa.begin();
}

void loop() {
  if (capa.updated()) {
   for (int i = 0; i < NUM_COLUMNS; i++) {
      // Write the row reference on serial (zCCxAAAxAAAx...xAAA)
      Serial.print("z"); // start
      Serial.print(i); // the index of the column
      for (int j=0;j<ROWS_USE;j++){
        Serial.print("x"); // "x" used as seperator
        Serial.print(capa.grid[ROWS_USE*i+j]);
      }
      Serial.println("");
    }
  }
  delay(50);
}