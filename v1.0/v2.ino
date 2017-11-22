#include <SoftwareSerial.h>
#include <Nextion.h>  // https://github.com/bborncr/nextion
#include <iarduino_RTC.h> //http://iarduino.ru/file/235.html 0x68 http://wiki.iarduino.ru/page/chasy-realnogo-vremeni-rtc-trema-modul/

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

//    if(millis()%1000==0){ // если прошла 1 секунда
//    Serial.println(time.gettime("d-m-Y, H:i:s, D")); // выводим время
//    delay(1); // приостанавливаем на 1 мс, чтоб не выводить время несколько раз за 1мс
//  }


//ventilation
//vt0 --- 10/40
//vt1 --- 10/20
//vt2 --- on
//vt4 --- off
//humidity
//68 48,0,0 --- middle
//68 49,0,0 --- hight
//68 50,0,0 --- off
//lamp
//lt0 --- auto
//lt1 --- on
//lt2 --- off
//
//    if (message == "lt1"){
//        digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
//        delay(5000);                       // wait for a second
//        digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
//    }
