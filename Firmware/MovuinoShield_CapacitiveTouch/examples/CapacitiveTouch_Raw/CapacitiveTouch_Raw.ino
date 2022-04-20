#include "_CapacitiveTouch.h"

CapacitiveTouch capa=CapacitiveTouch();

void setup() {
  Serial.begin(115200);
  capa.begin();
  while (!Serial);
  Serial.println("CapacitiveTouch get Raw data");
}

void loop() {
  String raw="";
  if (capa.updated()) {
   for (int i = 0; i < ROWS_USE * NUM_COLUMNS; i++) {
      if (capa.grid[i] > 0)raw+=capa.grid[i];
      if (i != ROWS_USE * NUM_COLUMNS - 1)
        raw+=",";
    }
  }
  Serial.println(raw+";");
  delay(1000);
}
