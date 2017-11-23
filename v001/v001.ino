#include <SoftwareSerial.h>
#include <Nextion.h>
#include <iarduino_RTC.h>
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"

SoftwareSerial nextion(2, 3);
iarduino_RTC time(RTC_DS1307);
Adafruit_SHT31 sht31 = Adafruit_SHT31();
Nextion myNextion(nextion, 9600);

void setup() {
  delay(100);
  Serial.begin(9600);
  myNextion.init();
  time.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  //time.settime(0,37,00,23,11,17,4);  // 0s,37min,00h, 23/11/17, thu  --- set time!!
  if (! sht31.begin(0x44)) 
  {
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
  
}

void loop() {
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();
  String message = myNextion.listen();
  if(message != ""){
    Serial.println(message);
  }
  
  if ( (! isnan(t)) and (! isnan(h)) ) 
  {
    //Serial.print("Temp *C = ");Serial.println(t);Serial.print("Hum. % = "); Serial.println(h); //for debug
    myNextion.setComponentText("t0", String(t));
    myNextion.setComponentText("h0", String(h));
  } 
  else 
  { 
    Serial.println("Failed to read temperature");
  }

  Serial.println();
  delay(1000);  
}
