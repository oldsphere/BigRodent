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
#include "RTClib.h"
#include "DHT.h"

#define DHTPIN A3
#define SD_PIN 4

// Init global objects
RTC_DS1307 RTC;
DHT dht(DHTPIN, DHT11);
SdFile root;

boolean adjutTime = false;

void setup () {

  Serial.begin(9600); // Establece la velocidad de datos del puerto serie
  
  Serial.print("Initiating I2C comunication...");
  Wire.begin(); // Inicia el puerto I2C 
  Serial.println("OK");

  Serial.print("Initiating RTC...");
  while(!RTC.begin()) {} // Inicia la comunicación con el RTC
  Serial.println("OK");

  if (adjutTime) {
   Serial.print("Adjusting time...");
   RTC.adjust(DateTime(__DATE__, __TIME__));
   Serial.println("OK");
  }

  Serial.print("Initiating SD...");

  Sd2Card card;
  SdVolume volume;   
  pinMode(10,OUTPUT); 
  if (!card.init(SPI_HALF_SPEED, SD_PIN)) {
    Serial.println("FAIL");
    Serial.println("SD not found!"); 
    while (true) {}
  }
  else {
    if (!volume.init(card)) {
      Serial.println("FAIL");
      Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
      while (true) {}
    }
    
    root.openRoot(volume);
    Serial.println("OK");
  }
  
  Serial.print("Initiating DHT sensor...");
  dht.begin();
  Serial.println("OK");

  // End starting report
  Serial.println("-----------------------------------------------------------");
  Serial.println("");
}


void loop(){
  char line[100];

  getReportLine(line);
  Serial.println(line);
  updateReport();
 
  delay(1000); // La información se actualiza cada 1 seg.
}



