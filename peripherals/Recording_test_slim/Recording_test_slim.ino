/*
 *  BigRodent
 *  - Record_test - 
 *  Date: 12/06/2016
 *  Author: Carlos Rubio Abujas <oldsphere@gmail.com>
 *  
 *  Description:
 *    <tbd>
 */ 

#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include "DHT.h"
#include "RTClib.h"


#define DHTPIN A3
#define SD_PIN 4

// Init global objects
RTC_DS1307 RTC;
DHT dht(DHTPIN, DHT11);
SdFile root;

boolean adjutTime = false;

void setup () {

  Serial.begin(9600); // Establece la velocidad de datos del puerto serie
  
  Serial.print(F("Initiating I2C comunication..."));
  Wire.begin(); // Inicia el puerto I2C 
  Serial.println(F("OK"));

  Serial.print(F("Initiating RTC..."));
  while(!RTC.begin()) {} // Inicia la comunicación con el RTC
  Serial.println(F("OK"));

  if (adjutTime) {
   Serial.print(F("Adjusting time..."));
   RTC.adjust(DateTime(__DATE__, __TIME__));
   Serial.println(F("OK"));
  }

  Serial.print(F("Initiating SD..."));

  Sd2Card card;
  SdVolume volume;   
  pinMode(10,OUTPUT); 
  if (!card.init(SPI_HALF_SPEED, SD_PIN)) {
    Serial.println(F("FAIL"));
    Serial.println(F("SD not found!")); 
    while (true) {}
  }
  else {
    if (!volume.init(card)) {
      Serial.println(F("FAIL"));
      Serial.println(F("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card"));
      while (true) {}
    }
    
    root.openRoot(volume);
    Serial.println(F("OK"));
  }
  
  Serial.print(F("Initiating DHT sensor..."));
  dht.begin();
  Serial.println(F("OK"));

  // End starting report
  Serial.println(F("-----------------------------------------------------------"));
  Serial.println("");
}


void loop(){
  char line[60];
  
  getReportLine(line);
  updateReport(line);
 
  delay(5000); // La información se actualiza cada 1 seg.
}



