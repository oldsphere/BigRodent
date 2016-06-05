/*
    BigRodent
    - termialSD_new -  
    Date: 23/02/2015
    Author: Carlos Rubio Abujas <oldsphere@gmail.com>

    Description:
      Main file of a console based file browser for arduino:
      
          write (file) (text)  -> Write (text) on (file) in the current folder
          ls -> List dirs on current folder
          mkdir (folder) -> Makes (folder) directory on current folder
          rmdir (folder) -> Removes (folder) directory
          echo (text) -> Print (text) by Serial port
          cd (dir) -> changes directory to (dir). If no (dir) is supplied returns to root directory.
                      (Still pending to add ".." utility to go to previous folder.)
*/

#include <SD.h>
#include <SPI.h>

#define SD_PIN 4
#define BUFFER_SIZE 50

Sd2Card card;
SdFile currentFolder;
SdVolume volume; 
SdFile root;

bool isFileOpen = false;

void setup() {  
  Serial.begin(9600);
  while(!Serial) {};
  Serial.println("Started Serial port");
  
  pinMode(10,OUTPUT); //SS PIN for SD Card.
  
  if (!card.init(SPI_HALF_SPEED, SD_PIN)) {
    Serial.println("SD not found!"); 
    while (true) {}
  }
  else {
    if (!volume.init(card)) {
      Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
      while (true) {}
    }
    
    root.openRoot(volume);
    currentFolder = root;
    Serial.println("SD initialized successfully!");
  }
}

void loop() {
    if (Serial.available() > 0) {
     char message[BUFFER_SIZE], command[BUFFER_SIZE], argument[BUFFER_SIZE];
     
     // Read Serial input
     readSerial(message);
     
     // Process serial input.
     getArgument(message,argument);
     getCommand(message,command);
         
     Serial.println(argument);    
         
     //process instruction.
     console(command,argument);
  }
  delay(1000); 
};



