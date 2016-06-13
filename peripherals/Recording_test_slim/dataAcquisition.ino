/*
 *  BigRodent
 *  - dataAcquisition - 
 *  Date: 12/06/2016
 *  Author: Carlos Rubio Abujas <oldsphere@gmail.com>
 *  
 *  Description:
 *    <tbd>
 */ 

void getReportLine(char* line) {
    char datestr[15];
    char timestr[15];  

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    char strH[6], strT[6];

    dtostrf(h,2,1,strH);
    dtostrf(t,2,1,strT);

    if(!RTC.isrunning()){
        printNow();
        Serial.println(F("Error while running RTC"));
        return;
    }
    DateTime now = RTC.now();

    if (isnan(t) || isnan(h)) { 
       printNow();
       Serial.println(F("Failed to read data from DHT")); 
       return;
    }

    getDate(now, datestr);
    getTime(now, timestr);

    sprintf(line, "%s;%s;%s;%s" , datestr,timestr,strT,strH);
}


