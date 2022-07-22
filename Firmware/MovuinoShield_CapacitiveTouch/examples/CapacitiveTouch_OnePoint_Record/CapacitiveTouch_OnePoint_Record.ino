#include "_CapacitiveTouch.h"
#include "_Recorder.h"

#define NUM_TX_USED             6
#define NUM_RX_USED             4

#define TX_USED                 6
#define RX_USED                 1

#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN                 21
#define SCL_PIN                 22
#endif

#define GAIN                    50

// Command for serial messages
#define CMD_FORMAT_SPIFF 'f' // Format the SPIFF
#define CMD_CREATE_FILE 'c'  // Create a new file in the SPIFF
#define CMD_RECORD 'r'      // Recording data
#define CMD_ADD_LINE 'a'     // Add a ne line in the SPIFFS (usefull for debugging)
#define CMD_READ_FILE 's'  // Read all files
#define CMD_LISTING_DIR 'l'  // List files in the directory
#define CMD_SPIFF_INFO 'i'   // Get informations about the spiff

bool wRecord = false;
unsigned int data=0;
String serialMessage="";

CapacitiveTouch capa;
MovuinoRecorder recorder;


void setup() {
  Serial.begin(115200);
  while (!Serial);
  if (DEBUG) Serial.println("CapacitiveTouch get Raw data");

  #ifdef _ESP32_HAL_I2C_H_ // For ESP32
  Wire.begin(SDA_PIN, SCL_PIN);
  capa.setWire(&Wire);
  #endif

  recorder.begin();

  capa.init();
  capa.setGain(GAIN);
}

void loop() {
  showMenu();
  while(!Serial.available()>0){
    delay(10);
  }
  char serialMessage = Serial.readStringUntil('\n')[0];
  switch (serialMessage)
    {
      case CMD_FORMAT_SPIFF:{
        Serial.println("Formating the SPIFFS (data files)...");
        recorder.formatSPIFFS();
        break;}
      case CMD_SPIFF_INFO:{
        Serial.println("Print info SPIFFS");
        recorder.printStateSPIFFS();
        break;
        }
      case CMD_READ_FILE:{
        Serial.println("reading all recorded files ");
        recorder.readAllRecords();
        break;
      }
      case CMD_RECORD:{
        wRecord = true;
        break;
        }
      case CMD_LISTING_DIR:{
        Serial.println("Listing directory");
        recorder.listDirectory();
        break;
      }
      default:{
        break;
      }
    }
  if (wRecord){
    Serial.println("Type our filename: ");
    while(!Serial.available()>0){
      delay(10);
    }
    String filename=Serial.readStringUntil('\n');
    Serial.println("your filename: "+filename);
    Serial.println("Type 's' to start and 'f' when you finish");
    while(!Serial.available()>0){
      delay(10);
    }
    char msg = Serial.readStringUntil('/n')[0];
    if (msg =='s'){
          record(filename);
    }
    else {
    Serial.println("Wrong character, try again.");
    }
    wRecord = false;
  }
}

void showMenu(){
  Serial.println("---MENU---");
  Serial.println("'f': Format the SPIFF");
  Serial.println("'i': Get infos about the SPIFF");
  Serial.println("'l': List files in the directory");
  Serial.println("'s': Read all files");
  Serial.println("'r': Record\n");
  Serial.println("What would you like to do?");
}

void record(String filename){
  recorder.newRecord(filename);
  recorder.defineColumns("sensor");
  char end = ' ';
  Serial.setTimeout(10);
  while(end!='f'){
      if(Serial.available()>0){
        end=Serial.readStringUntil('\n')[0];
      }
      data=capa.getRawData(RX_USED,TX_USED);
      //Serial.println(data);
      if (recorder.isRecording()){
        recorder.addRow();
        recorder.pushData(data);
      }
      delay(10);
  }
  recorder.stop();
  Serial.setTimeout(1000);
}


