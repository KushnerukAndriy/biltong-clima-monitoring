/*
You will need this 
https://github.com/bborncr/nextion
and this
http://openhardware.gridshield.net/home/nextion-lcd-getting-started-for-arduino
*/

#include <SoftwareSerial.h>
#include <Nextion.h>

SoftwareSerial nextion(2, 3);

Nextion myNextion(nextion, 9600);

void setup() {
  Serial.begin(9600);
  myNextion.init();
}

void loop() {
  String message = myNextion.listen();
  if(message != ""){
    Serial.println(message);
  }  
}