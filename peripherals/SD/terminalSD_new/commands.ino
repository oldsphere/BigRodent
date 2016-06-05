/*
    BigRodent
    - commands -  
    Date: 23/02/2015
    Author: Carlos Rubio Abujas <oldsphere@gmail.com>

    Description:
        Definition of console functions and auxiliars.

        console: Receives the command and argument and performs the actions specified.
        readSerial: Read the Serial buffer and saves the results on the provided char vector.
        getPart: Process a sentence spliting by defined delimiter
        getCommands: Process a sentence and returns the first word
        getArguments: Process a sentence and returns everithing but the first word
*/

bool echoMode = true;

void console(char* command, char* argument) {

     if (echoMode) {
        Serial.print('>');
        Serial.print(command);
        Serial.print(" ");
        Serial.println(argument);
     }

     if (strcmp(command, "write") == 0) {
       //write on open file
       SdFile document;
       char documentName[BUFFER_SIZE], content[2*BUFFER_SIZE];
       
       getPart(argument," ",-1,content);
       getPart(argument," ",1,documentName);
               
       if (!document.open(&currentFolder,documentName,O_WRITE|O_CREAT|O_APPEND)) {
         Serial.print("Impossible to write on \"");
         Serial.print(documentName);
         Serial.println("\"");
       }
       else {
         SPI.begin();
         
         Serial.print("Write the following sentence: ");
         Serial.println(content);
         document.println(content);
         document.close();
         
         SPI.end();
         Serial.print("Lines has been added to file \"");
         Serial.print(documentName);
         Serial.println("\"");
         
       }      
     }
     
     else if (strcmp(command, "ls") == 0) {
       // display the content of current folder.
       
       if (strcmp(argument,NULL) == 0 ) {
         Serial.println("Contents of current folder");
         currentFolder.ls(LS_DATE | LS_SIZE);
       }
       else {
         SdFile inputFolder;
         if(!inputFolder.open(&currentFolder,argument,O_READ)) {
           Serial.print("Unable to display \"");
           Serial.print(argument);
           Serial.println("\"");
         }
         else {
           inputFolder.ls(LS_DATE | LS_SIZE);  
         }
       }
     }
     
     
     else if (strcmp(command, "mkdir") == 0) {
       // creates a folder.
       SdFile newFolder;
       char folderName[BUFFER_SIZE];
       
       getPart(argument," ",1,folderName);
       
     if (!newFolder.makeDir(&currentFolder,folderName)) {
         Serial.println("Error during making a new file");
     }
     else {
       Serial.print("Folder \"");
       Serial.print(folderName);
       Serial.println("\" succesfully created!");
     }  
     
    }
     
     
     else if (strcmp(command, "rmdir") == 0) {
       // creates a folder.
       SdFile removalFolder;
       if(!removalFolder.open(&currentFolder,argument,O_READ)) {
         Serial.print("Unable to remove \"");
         Serial.print(argument);
         Serial.println("\"");
       }
       else {
         if (!removalFolder.rmDir()) {
           Serial.print("1- unable to remove \"");
           Serial.print(argument);
           Serial.println("\"");
         }  
       }
     }
     
     
     else if (strcmp(command, "echo") == 0) {
       // Replies the argument.
         Serial.println(argument);
     }     
   
     
     else if (strcmp(command, "cd") == 0) {
       // open file.
       SdFile openFile;
       char openName[BUFFER_SIZE];
                   
       if (strcmp(argument,NULL) == 0) {
         currentFolder = root;
         Serial.println("Back to root folder");
         return;
       }

       if (strcmp(argument, ".. ") == 0) {
         getPart(argument,"/",-1,openName);
         Serial.print("Prior folder is: \"");
         Serial.print( openName );
         Serial.println("\"");
         return;
       }

       if (strcmp(argument, ". ") == 0) {
         Serial.print("same directory");
         return;
       }
       
       getPart(argument," ",1,openName);
       
       if(!openFile.open(&currentFolder,openName,O_READ)) {
         Serial.print("Unable to open \"");
         Serial.print(openName);
         Serial.println("\"");
       }
       else {
         currentFolder = openFile;
       }
     }   
    
     else {
       // Unrecognized command.
       Serial.print("\"");
       Serial.print(command);
       Serial.println("\" command not recognized"); 
     }

  Serial.println("");
}

void readSerial (char *outString) {
/*  
    Author: Carlos Rubio Abujas <oldsphere@gmail.com>
    Date: 02/20/2015
    Description:
    Read the Serial buffer and saves the results on the provided char vector.

    Arguments:
      outString - Pointer to the char output vector
      bufferSize - The amount of characters the output vector can manage.
*/
 
  int i=0;
  while (Serial.available() > 0) {
     outString[i] = Serial.read();
     i++;
  }
  outString[i] = '\0'; // Set the final of the string.
}

void getCommand(char* instruction, char* command) {
  getPart(instruction," ",1,command);  
}

void getArgument(char* instruction, char* argument) {
  getPart(instruction," ",-1,argument);  
}

void getPart(char *textLine,char *delimiter , int part, char* outStr) {
/*
  Author: Carlos Rubio Abujas <oldsphere@gmail.com>
  Date: 21/02/2015
  Description:  
    The aim of this function is to analice a string and split the potential command from the 
    potential argument.
    To do so two operation modes can be estbalished:
      - If the interested part is the command is a particular word the part argument must be 
      a possitive number with the position of the word to return.
      - If the interested part is the rest of the text line but a number of first words the
      part argument must be a negative number with the position of the desires word (with the 
      word excluded.
      
     Arguments:
       textLine - Char array with the text line to process.
       delimiter - The delimiter between words.
       part - the position for split the text line
       outStr - output char array.
*/
  
  char cmd[BUFFER_SIZE] , arg[BUFFER_SIZE] , *token;
  
  int i;
  i = 1;

  strcpy(arg,"");
  strcpy(cmd,"");
  token = strtok(textLine,delimiter);
  while (token != NULL) {
    
    if (i == abs(part)) {
      //Save command.
      strcpy(cmd,token);  
    }
    
    if (i > abs(part)) {
      //Attach argument
      strcat(arg,token);
      strcat(arg,delimiter);  
    }
    
    token = strtok(NULL,delimiter);
    i++;     
  }
  
  if(part>0) 
    strcpy(outStr,cmd);
  else
    strcpy(outStr,arg);    
}


