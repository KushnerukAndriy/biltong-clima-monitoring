#include <SoftwareSerial.h>
#include <Nextion.h>
#include <iarduino_RTC.h>

SoftwareSerial nextion(2, 3);
iarduino_RTC time(RTC_DS1307);

Nextion myNextion(nextion, 9600);

void setup() {
  delay(100);
  Serial.begin(9600);
  myNextion.init();
  time.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  //time.settime(0,37,00,23,11,17,4);  // 0s,37min,00h, 23/11/17, thu  --- uncomennt only when set time!!
  
}

void loop() {
  String message = myNextion.listen();
  if(message != ""){
    Serial.println(message);
  }  
}
