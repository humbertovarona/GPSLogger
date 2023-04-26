   
#include <SoftwareSerial.h>
#include <TinyGPS.h>
    
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <RTClib.h>
#include <OneWire.h>
	
File dataFile;
DateTime now;
RTC_DS1307 RTC;
  
const int chipSelect = 7;
    long lat,lon;
    SoftwareSerial gpsSerial(4, 6);
    TinyGPS gps;
     
void setup(){
    
    Serial.begin(115200);
    gpsSerial.begin(9600);
      
    Wire.begin();
    RTC.begin();
    if (! RTC.isrunning()) {
        Serial.println("RTC is not running!");
        RTC.adjust(DateTime(__DATE__, __TIME__));
    }

    Serial.print("Configuring SD card");

    if (!SD.begin(chipSelect)) {
        Serial.println("Card not present or failed");
        return;
    }
    Serial.println("Card configured");

    now = RTC.now();
    dataFile = SD.open("gps_LOGS.txt", FILE_WRITE);
    dataFile.print(now.year(),DEC);
    dataFile.print('/');
    dataFile.print(now.month(),DEC);
    dataFile.print('/');
    dataFile.print(now.day(),DEC);
    dataFile.println(" ");
    dataFile.println("Longitude              Latitude              Time");
    dataFile.close();

}
     
void loop(){
    now = RTC.now();
    while(gpsSerial.available()){
        if(gps.encode(gpsSerial.read())){
            gps.get_position(&lat,&lon);
            dataFile = SD.open("gpsLOG.csv", FILE_WRITE);
            if (dataFile) {
                dataFile.print(lon/1000000);
                dataFile.print(";");
                dataFile.print(lat%1000000);
                dataFile.print(";");
                dataFile.print(now.hour(),DEC);
                dataFile.print(":");
                dataFile.print(now.minute(),DEC);
                dataFile.print(":");
                dataFile.println(now.second(),DEC);
                dataFile.close();
                Serial.println("Recorded data");
            }
            else {
                Serial.println("error opening gpslog.txt");
            }
            delay(10000);
        }
    }
}
