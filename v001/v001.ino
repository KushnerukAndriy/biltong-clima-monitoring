#include <SoftwareSerial.h>
#include <Nextion.h>
#include <Wire.h>
#include "RTClib.h"
#include "Adafruit_SHT31.h"

SoftwareSerial nextion(3, 2);
Nextion myNextion(nextion, 9600);
Adafruit_SHT31 sht31 = Adafruit_SHT31();

RTC_DS1307 rtc;

int* configs[4];
uint32_t refTime;
uint32_t refIndex = 0;
int* currentConfig;
int currentConfigIndex = 0;
int cycle = 0;
int hr;
int mn;

String newMessage;
// Default status
String humMode = "hy0";
String ventMode = "vt0";
String lampMode = "lt2";
int currentHum = 65;
// Relays
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
  
  if(refDiff >= currentDelay) {
    refIndex++;
    if(refIndex >= arrSize) refIndex = 0;
    refTime = currentTime;
  }
  digitalWrite(CH1, currentVoltage);
  if (currentVoltage == 0){
    myNextion.setComponentText("other.v_stat", "on");
  }
  else if (currentVoltage == 1) {
    myNextion.setComponentText("other.v_stat", "off");
  }
}

void humidity(String str) {
  if (str == "68 48,0,0") {
    humMode = "hy0";
    currentHum = 65;
    myNextion.setComponentText("other.hy_txt", String(humMode));
    //Serial.println(humMode);//for debug
    return;
  }
  if (str == "68 49,0,0") {
    humMode = "hy1";
    currentHum = 78;
    myNextion.setComponentText("other.hy_txt", String(humMode));
    //Serial.println(humMode);//for debug
    return;
  }
  if (str == "68 50,0,0") {
    humMode = "hy2";
    currentHum = 0;
    myNextion.setComponentText("other.hy_txt", String(humMode));
    //Serial.println(humMode);//for debug
    return;
  }
}

void ventilation(String str) {
  if (str == "vt0") {
    ventMode = "vt0";
    myNextion.setComponentText("other.vt_txt", String(ventMode));
    //Serial.println(ventMode);//for debug
    return;
  }
  if (str == "vt1") {
    ventMode = "vt1";
    myNextion.setComponentText("other.vt_txt", String(ventMode));
    //Serial.println(ventMode);//for debug
    return;
  }
  if (str == "vt2") {
    ventMode = "vt2";
    myNextion.setComponentText("other.vt_txt", String(ventMode));
    //Serial.println(ventMode);//for debug
    return;
  }
  if (str == "vt4") {
    ventMode = "vt4";
    myNextion.setComponentText("other.vt_txt", String(ventMode));
    //Serial.println(ventMode);//for debug
    return;
  }
}

void lamp(String str) {
  if (str == "lt0") {
    lampMode = "lt0";
    myNextion.setComponentText("other.lt_txt", String(lampMode));
    //Serial.println(lampMode);//for debug
    return;
  }
  if (str == "lt1") {
    lampMode = "lt1";
    digitalWrite(CH3, 0);
    myNextion.setComponentText("other.lt_txt", String(lampMode));
    //Serial.println(lampMode);//for debug
    return;
  }
  if (str == "lt2") {
    lampMode = "lt2";
    digitalWrite(CH3, 1);
    myNextion.setComponentText("other.lt_txt", String(lampMode));
    //Serial.println(lampMode);//for debug
    return;
  }
}

void sendStatus(int i, int h, int m) {
  if ( i == 3) {
    myNextion.setComponentText("other.hy_txt", String(humMode));
  }
  else if ( i == 5 ) {
    myNextion.setComponentText("other.vt_txt", String(ventMode));
  }
  else if ( i == 6 ) {
    myNextion.setComponentText("other.hr", String(h));
    myNextion.setComponentText("other.mn", String(m));
  }
  else if ( i == 7 ) {
    myNextion.setComponentText("other.lt_txt", String(lampMode));
  }
  else if ( i == 11 ) {
    cycle = 0;
  }
}

void humControl(int i) {
  if ( humMode == "hy0") {
    if (i <= currentHum) {
      digitalWrite(CH2, 1);
      myNextion.setComponentText("other.h_stat", "off");
    }
    else if (i > currentHum) {
      digitalWrite(CH2, 0);
      myNextion.setComponentText("other.h_stat", "on");
    }
  }
  else if ( humMode == "hy1" ) {
    if (i < currentHum) {
      digitalWrite(CH2, 1);
      myNextion.setComponentText("other.h_stat", "off");
    }
    else if (i > currentHum) {
      digitalWrite(CH2, 0);
      myNextion.setComponentText("other.h_stat", "on");
    }  
  }
  else if ( currentHum == 0 ) {
    digitalWrite(CH2, 1);
    myNextion.setComponentText("other.h_stat", "off");
  }
}
void setup() {
  while (!Serial);
  delay(1000);
  Serial.begin(9600);
  myNextion.init();
  refTime = rtc.now().unixtime();
  
  if ( (! sht31.begin(0x44)) and (! rtc.begin()) ) {
    Serial.println("Couldn't find SHT31 or RTC");
    while (1) delay(1);
  }
  else {
    Serial.println("RTC/SHT31 --- OK");
    //rtc.adjust(DateTime(2017, 12, 28, 20, 18, 30));
  }

  // init vent configs
  configs[0] = new int[3] {1, 0, 0};
  configs[1] = new int[5] {2, 60, 0, 2400, 1}; // _______, time1, value1, time2, value2
  configs[2] = new int[5] {2, 60, 0, 1200, 1}; 
  configs[3] = new int[3] {1, 0, 1};

  // init relays
  pinMode(CH1, OUTPUT);
  pinMode(CH2, OUTPUT);
  pinMode(CH3, OUTPUT);
  //pinMode(CH4, OUTPUT);
  digitalWrite(CH1, 1);
  digitalWrite(CH2, 1);
  digitalWrite(CH3, 1);
  //digitalWrite(CH4, 1);
  
}

void loop () {
  delay(1000);
  // INIT
  DateTime now = rtc.now();
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  Serial.print(now.minute(), DEC);
  Serial.print(":");
  Serial.println(now.second(), DEC);
  hr = now.hour();
  mn = now.minute();
  if ((now.hour() == 0) and (now.minute() == 30) and (lampMode == "lt0")){
    digitalWrite(CH3, 0);
  }
  else if ((now.hour() == 0) and (now.minute() == 35)) {
    digitalWrite(CH3, 1);
    lampMode = "lt0";
  }
  int t = sht31.readTemperature();
  int h = sht31.readHumidity();
  // Check SHT31 connection and sent 1 per 10 cycle
  if ( (! isnan(t)) and (! isnan(h)) and (cycle ==10)) {\
    myNextion.setComponentText("t0", String(t));
    myNextion.setComponentText("t1", String(h));
  } 
  else { 
   //Serial.println("Failed to read temperature");
  }

  String message = myNextion.listen(); //check for message
  if(message != ""){ // if a message is received...
    newMessage=message;
    //Serial.println(message); //...print it out
  }
  //Serial.println(newMessage);

    humidity(newMessage);
    ventilation(newMessage);
    lamp(newMessage);

  //hum
  humControl(h);

  //vent
  if (ventMode == "vt0"){
    int currentConfigIndex = 0;
    uint32_t refIndex = 0;
    setupFan(configs[1]);
  }
  else if (ventMode == "vt1"){
    int currentConfigIndex = 0;
    uint32_t refIndex = 0;
    setupFan(configs[2]);
  }
  else if (ventMode == "vt2"){
    int currentConfigIndex = 0;
    uint32_t refIndex = 0;
    setupFan(configs[0]);
  }
  else if (ventMode == "vt4"){
    int currentConfigIndex = 0;
    uint32_t refIndex = 0;
    myNextion.setComponentText("other.v_stat", "off");
    setupFan(configs[3]);
  }
  cycle++;
  sendStatus(cycle, hr, mn);
}
