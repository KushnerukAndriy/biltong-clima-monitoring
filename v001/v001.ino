#include <SoftwareSerial.h>
#include <Nextion.h>
#include "RTClib.h"
#include <Wire.h>
#include "Adafruit_SHT31.h"

SoftwareSerial nextion(2, 3);
Nextion myNextion(nextion, 9600);
Adafruit_SHT31 sht31 = Adafruit_SHT31();
RTC_DS1307 rtc;
int* configs[4];

uint32_t refTime;
uint32_t refIndex = 0;
int* currentConfig;

void setup() {
  delay(1000);
  Serial.begin(9600);
  rtc.begin();
  myNextion.init();

  if ( (! sht31.begin(0x44)) or (! rtc.isrunning()) ){
    Serial.println("Somethinks wrong!!");
    while (1) delay(1);
  }

  configs[0] = new int[3] {1, 0, 0};             // перший параметр - це кількість значень, далі значення ідуть парами - 1 число кількість часу яке має пройти до переключення режиму, 2 число значення режиму
  configs[1] = new int[5] {2, 600, 95, 2400, 255}; // 45 120 це ті значення які попадуть у змінну вольтаж currentVoltage, 
  configs[2] = new int[5] {2, 600, 95, 1200, 255}; 
  configs[3] = new int[3] {1, 0, 255};

  refTime = rtc.now().unixtime();
}

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
 
  analogWrite(5, currentVoltage);
}

int currentConfigIndex = 0;

void loop() {
   String message = myNextion.listen();
   if(message != ""){
     Serial.println(message);
   }

  setupFan(configs[currentConfigIndex]);

  int t = sht31.readTemperature(); // float for real
  int h = sht31.readHumidity();

  if ( (! isnan(t)) and (! isnan(h)) ) {
   delay(200);
   myNextion.setComponentText("t0", String(t));
   myNextion.setComponentText("t1", String(h));
  } 
  else { 
   Serial.println("Failed to read temperature");
  }
}
