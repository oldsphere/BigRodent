/*
 *  BigRodent
 *  - dataAcquisition - 
 *  Date: 12/06/2016
 *  Author: Carlos Rubio Abujas <oldsphere@gmail.com>
 *  
 *  Description:
 *    <tbd>
 */ 

void printDHT() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(t) || isnan(h)) { 
       printNow();
       Serial.println("Failed to read data from DHT"); return; }
    else {
      printNow();
      Serial.print("Humidity ");Serial.print(h);Serial.print(" %\t");
      Serial.print("Temperature ");Serial.print(t);Serial.println(" *C");
    }
}

void getReportLine(char* line) {
    char datestr[20];
    char timestr[20];  
    
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    char strH[10], strT[10];

    dtostrf(h,2,1,strH);
    dtostrf(t,2,1,strT);

    if(!RTC.isrunning()){
        printNow();
        Serial.println("Error while running RTC");
        return;
    }
    DateTime now = RTC.now();

    if (isnan(t) || isnan(h)) { 
       printNow();
       Serial.println("Failed to read data from DHT"); 
       return;
    }

    getDate(now, datestr);
    getTime(now, timestr);

    sprintf(line, "%s;%s;%s;%s" , datestr,timestr,strT,strH);
}


