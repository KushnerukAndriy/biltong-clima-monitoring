#include <SoftwareSerial.h>
#include <Nextion.h>
#include <Wire.h>
#include "RTClib.h"
#include "Adafruit_SHT31.h"

SoftwareSerial nextion(3, 2);
Nextion myNextion(nextion, 9600);
Adafruit_SHT31 sht31 = Adafruit_SHT31();

RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int* configs[4];
uint32_t refTime;
uint32_t refIndex = 0;
int* currentConfig;
int currentConfigIndex = 0;
int currentVentStatus = 1;
int defaultHum = 65;
int maxHum =65;

int CH1 = 12;
int CH2 = 11;
int CH3 = 10;
int CH4 = 9;

void setupFan(int* currentConfig) {
  uint32_t currentTime = rtc.now().unixtime();
  
  int arrSize = currentConfig[0];
  uint32_t currentDelay = currentConfig[1 + refIndex * 2 ];
  uint32_t currentVoltage = currentConfig[1 + refIndex * 2 + 1];
  
  uint32_t refDiff = currentTime - refTime;
//  Serial.println(refDiff);
//  Serial.println(currentDelay);
//  Serial.println(currentVoltage);;
  
  if(refDiff >= currentDelay) {
    refIndex++;
    if(refIndex >= arrSize) refIndex = 0;
    refTime = currentTime;
  }
  digitalWrite(CH1, currentVoltage);
}

void setup() {
  while (!Serial);
  delay(1000);
  Serial.begin(9600);
  
  if ( (! sht31.begin(0x44)) and (! rtc.begin()) ) {
    Serial.println("Couldn't find SHT31 or RTC");
    while (1) delay(1);
  }
  else {
    Serial.println("RTC/SHT31 --- OK");
    rtc.adjust(DateTime(2017, 12, 5, 17, 21, 0));
  }

  myNextion.init();
  refTime = rtc.now().unixtime();
  configs[0] = new int[3] {1, 0, 0};
  configs[1] = new int[5] {2, 60, 0, 2400, 1}; // _______, time1, value1, time2, value2
  configs[2] = new int[5] {2, 60, 0, 1200, 1}; 
  configs[3] = new int[3] {1, 0, 1};
  
  pinMode(CH1, OUTPUT);
  pinMode(CH2, OUTPUT);
  pinMode(CH3, OUTPUT);
  //pinMode(CH4, OUTPUT);
  digitalWrite(CH1, 1);
  digitalWrite(CH2, 1);
  digitalWrite(CH3, 1);
  //digitalWrite(CH4, 1);
  
  if ( currentVentStatus == 1) {
    setupFan(configs[1]);
    myNextion.setComponentText("v_stat", String(currentVentStatus));
  }
}


void loop () {
  delay(1000);
  DateTime now = rtc.now();
  //Serial.print(now.hour(), DEC);
  //Serial.print(':');
  //Serial.print(now.minute(), DEC);
  //Serial.println("");
  int t = sht31.readTemperature();
  int h = sht31.readHumidity();
  String message = myNextion.listen();
  if (message != "") {
    Serial.println(message);
    if (message == "lt1") {
          digitalWrite(CH3, 0);   
      }
    else if (message == "lt2") {
          digitalWrite(CH3, 1);
       }
    else if (message == "68 48,0,0") {
      defaultHum = 65;
      maxHum = 65;
    }
    else if (message == "68 49,0,0") {
      defaultHum = 80;
      maxHum = 80;
    }
    else if (message == "68 50,0,0") {
      defaultHum = 0;
      maxHum = 0;
    }
  }

  if ( (! isnan(t)) and (! isnan(h)) ) {
   myNextion.setComponentText("t0", String(t));
   myNextion.setComponentText("t1", String(h));
   myNextion.setComponentText("v_stat", int(1));
   //Serial.println( t);Serial.println( h);
  } 
  else { 
   Serial.println("Failed to read temperature");
  }
  if ( defaultHum != 0) {
    if (h <= defaultHum) {
      digitalWrite(CH2, 1);
    }
    else if (h > maxHum) {
      digitalWrite(CH2, 0);
    }
  }
  else {
    digitalWrite(CH2, 1);
  }
}

