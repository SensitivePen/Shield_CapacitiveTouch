// Simulate the data of the capacitive touch controller to test it on the matrix dataviz Processing code

#define NUM_COLUMNS       6
#define ROWS_USE          4

void setup() {
  Serial.begin(9600);

}

void loop() {
    simulateData();
}

void simulateData(){
  int grid[NUM_COLUMNS*ROWS_USE];
  initGrid(grid);
  while(true){
      for (int i = 0; i < NUM_COLUMNS; i++) {
      // Write the row reference on serial (zCCxAAAxAAAx...xAAA)
      Serial.print("z"); // start
      Serial.print(i); // the index of the column
      for (int j=0;j<ROWS_USE;j++){
        Serial.print("x"); // "x" used as seperator
        int r = random(0,256);
        Serial.print(grid[ROWS_USE*i+j]);
      }
      Serial.println("");
    }
    updateGrid(grid);
    delay(50);
  }
}

int *initGrid(int grid[NUM_COLUMNS*4]){
  for(int i=0;i<NUM_COLUMNS*ROWS_USE;i++){
    grid[i]=random(0,256);
  }
  return grid;
}

int *updateGrid(int grid[NUM_COLUMNS*ROWS_USE]){
  int max=3;
  for (int i = 0; i < NUM_COLUMNS*ROWS_USE; i++) {
      int r = random(0,max);
      if(grid[i]+r>255) grid[i]=0;
      else grid[i]=grid[i]+r;
    }
  return grid;
}