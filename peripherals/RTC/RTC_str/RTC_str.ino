// Script para configurar RTC.

//Carga librerias.
#include <Wire.h>
#include "RTClib.h"

//Inicializa objetos
RTC_DS1307 RTC;

boolean adjutTime = false;

void setup () {
  
  Serial.begin(9600); // Establece la velocidad de datos del puerto serie
  Wire.begin(); // Inicia el puerto I2C 
  Serial.println("I2C communicacion started!");
  
  RTC.begin(); // Inicia la comunicación con el RTC
  Serial.println("RTC initialized!");
  

  if (adjutTime) {
   RTC.adjust(DateTime(__DATE__, __TIME__)); 
    Serial.println("Time adjusted!!");
  }
}


void loop(){
  char folderName[50];
  char fileName[50];
  printNow();
  getFolderName_test(folderName);
  getFileName_test(fileName);
  Serial.print(folderName);
  Serial.print("/");
  Serial.println(fileName);
 
  delay(1000); // La información se actualiza cada 1 seg.
}

