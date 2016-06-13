/*
 *  BigRodent
 *  - timestr - 
 *  Date: 12/06/2016
 *  Author: Carlos Rubio Abujas <oldsphere@gmail.com>
 *  
 *  Description:
 *    <tbd>
 */ 

void getMonthName(DateTime date, char* strMonth) {
// Return the name of the month given the timestamp.
    switch(date.month())
    {
      case 0: sprintf(strMonth, "Jan");  break;
      case 1: sprintf(strMonth, "Feb"); break;
      case 2: sprintf(strMonth, "Mar"); break;
      case 3: sprintf(strMonth, "Apr"); break;
      case 4: sprintf(strMonth, "May"); break;
      case 5: sprintf(strMonth, "Jun"); break;
      case 6: sprintf(strMonth, "Jul"); break;
      case 7: sprintf(strMonth, "Agu"); break;
      case 8: sprintf(strMonth, "Sep"); break;
      case 9: sprintf(strMonth, "Oct"); break;
      case 10: sprintf(strMonth, "Nov"); break;
      case 11: sprintf(strMonth, "Dec"); break;
      default: sprintf(strMonth, "ERR"); break;
    }
 }

void getFolderName(char* folderName) {
  char monthName[4]; 
  if(!RTC.isrunning()){
    Serial.println(F("Error while running RTC"));
  }
  else {
    DateTime now = RTC.now(); 
    getMonthName(now, monthName);
    sprintf(folderName, "%s%02d", monthName, now.day() );
  }
}

void getFileName(char *fileName) {
  if(!RTC.isrunning()){
    Serial.println(F("Error while running RTC"));
  }
  else {
    DateTime now = RTC.now(); 
    sprintf(fileName, "DAY%02d.csv", now.hour() );
  }
}


void getDate(DateTime date, char* strDate) {
//   Return the date in a string format.
    char delimiter = '/';
    sprintf(strDate, "%04d%c%02d%c%02d", date.year(), delimiter, date.month(), delimiter,  date.day()); }

void getTime(DateTime time, char* strTime) {
    char delimiter = ':';
    sprintf(strTime, "%02d%c%02d%c%02d" , time.hour(),delimiter, time.minute(),delimiter, time.second()); }

void printDate(DateTime date) {
    char datestr[20];
    getDate(date, datestr);
    Serial.print(datestr);
}

void printTime(DateTime date) {
    char strtime[20];
    getTime(date, strtime);
    Serial.print(strtime);
}

void printNow(){
  if(!RTC.isrunning()){
    Serial.println(F("Error while running RTC"));
  }
  else {
    DateTime now = RTC.now(); 
    printDate(now);
    Serial.print("  ");
    printTime(now);
    Serial.print(" - ");
  }
}
