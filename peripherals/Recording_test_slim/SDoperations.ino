/*
 *  BigRodent
 *  - SDoperations - 
 *  Date: 12/06/2016
 *  Author: Carlos Rubio Abujas <oldsphere@gmail.com>
 *  
 *  Description:
 *    <tbd>
 */ 

bool existFolder(char* folderName) {
    SdFile newFolder;
    if (newFolder.open(&root,folderName,O_READ)) { 
        newFolder.close();
        return true;
    }
    newFolder.close();
    return false;
}

void createFolder() {
    char folderName[10];
    SdFile newFolder;

    getFolderName(folderName);

    if (existFolder(folderName)) {
      return;
    }
    
    if (!newFolder.makeDir(&root,folderName)) {
      printNow();
      Serial.print(F("Error creating folder \""));
      Serial.print(folderName);
      Serial.println(F("\""));
      newFolder.close();
      return;
    }
    
    printNow();
    Serial.print(" - Folder \"");
    Serial.print(folderName);
    Serial.println("\" created");
}

void updateReport(char* line) {
    char fileName[10];
    char folderName[10];
    bool created = false;
    SdFile newFile;
    SdFile folder;

    // Ensures the folder exists.
    getFolderName(folderName);
    createFolder(); 
    /*
    
    if (!existFolder(folderName)) { 
        Serial.print(F("Folder \""));
        Serial.print(folderName);
        Serial.println(F("\" doesn't exists. Creating it"));
        
     }
     */

    //Moves the current directory to folder
    if (!folder.open(&root,folderName,O_READ)) {
      printNow();
      Serial.print(F("Unable to open folder \""));
      Serial.print(folderName);
      Serial.println("\"");
      return;
    }

    //Check if the report file exists
    getFileName(fileName);
    /*
    if (!newFile.open(&folder,fileName, O_READ)) {
        printNow();
        Serial.print(" - File \"");
        Serial.print(fileName);
        Serial.println("\" created");
        created = true;
    }
    newFile.close();
    */
        
    if (!newFile.open(&root,fileName,O_WRITE|O_CREAT|O_APPEND)) {
      printNow();
      Serial.print(F("Error creating file \""));
      Serial.print(fileName);
      Serial.println("\"");
      folder.close();
      return;
    }

    SPI.begin();
    if (created) { 
       newFile.println(F("Date;Time;Temp[*C];Humid[%]"));
    }
    newFile.println(line);
    newFile.close();
    folder.close();
    SPI.end();
    printNow();
    Serial.print("File  \"");Serial.print(fileName);Serial.print("\" updated [ ");
    Serial.print(line); Serial.println(" ]");
}

