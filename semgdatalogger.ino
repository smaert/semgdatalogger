/* SD Card dataloger with SPI
 *  The code is based on SD Datalogger tutorial by Tom Igoe https://www.arduino.cc/en/Tutorial/Datalogger
 *   
 *   This code also contains a timer - for desired logging interval change value of "interval"
 *   
 *   By defoault, the logging frequency is set to 10 Hz  (interval = 100ms). This allows capture of max 4 channels + relative time in ms. 
 *   The latter is added in order to check the stability. 
 *   
 *   When connecting the sensors, one should start with A0.. 
 */

#include <SPI.h>
#include <SD.h>

//Variables for timer
long interval = 100; // change for desired logging frequency
long t0 = 0; 

// SD-card shield dependent variable. Note, that the CS pin might not be 10 in the case of stackable sd-card shields
const int chipSelect = 10;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
  while (!Serial) {
      }
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}

void loop() {
  // make a string for assembling the data to log:
  String dataString = "";
  
  unsigned long t1 = millis();  // capture the current time
// compare the time to the previously stored time

   if (t1 - t0 >= interval) 
   {
          t0 = t1; //"reset" counter
  
  // read three sensor data and concatenate it to a string:
  for (int analogPin = 0; analogPin < 2; analogPin++) {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ",";
    }
  }
   dataString += millis();
   
// open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    //  Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
   }
  }
}
