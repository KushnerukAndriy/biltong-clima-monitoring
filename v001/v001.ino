#include <SoftwareSerial.h>
#include <Nextion.h>
#include <Wire.h>
#include "RTClib.h"
#include "Adafruit_SHT31.h"

SoftwareSerial nextion(3, 2);
Nextion myNextion(nextion, 9600);
Adafruit_SHT31 sht31 = Adafruit_SHT31();

RTC_DS1307 rtc;

int CH1 = 12;
int CH2 = 11;
int CH3 = 10;
int CH4 = 9;

void setup() {
  while (!Serial);
  delay(1000);
  Serial.begin(9600);
  myNextion.init();
  
  if ( (! sht31.begin(0x44)) and (! rtc.begin()) ) {
    Serial.println("Couldn't find SHT31 or couldn't find RTC");
    while (1) delay(1);
  }
  else {
    Serial.println("RTC SHT31 --- OK");
  }
  
  pinMode(CH1, OUTPUT);
  pinMode(CH2, OUTPUT);
  pinMode(CH3, OUTPUT);
  pinMode(CH4, OUTPUT);
  digitalWrite(CH1, 1);
  digitalWrite(CH2, 1);
  digitalWrite(CH3, 1);
  digitalWrite(CH4, 1);
}

void loop () {
  delay(1000);
  int t = sht31.readTemperature();
  int h = sht31.readHumidity();
  String message = myNextion.listen();
  if(message != ""){
    Serial.println(message);
  }

  if ( (! isnan(t)) and (! isnan(h)) ) {
   myNextion.setComponentText("t0", String(t));
   myNextion.setComponentText("t1", String(h));
   Serial.println( t);
   Serial.println( h);
  } 
  else { 
   Serial.println("Failed to read temperature");
  }
}


//  
//  if ( message == "vt0" ) currentConfigIndex = 1;
//  if ( message == "vt1" ) currentConfigIndex = 2;
//  if ( message == "vt2" ) currentConfigIndex = 3;
//  if ( message == "vt4" ) currentConfigIndex = 0;
//
//  setupFan(configs[currentConfigIndex]);
//

