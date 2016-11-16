#include "stDHT.h"
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
DHT sens(DHT22); // Указать датчик DHT11, DHT21, DHT22
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() 
{
  lcd.begin();                        // вмикаємо екран
  lcd.backlight();                   // вмикаємо підсвітку 
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);
  pinMode(3, INPUT);
  digitalWrite(3, HIGH);
}

void loop() 
{
  delay(2000);
  float t = sens.readTemperature(2);  // чтение датчика на пине 2
  float h = sens.readHumidity(2);     // чтение датчика на пине 2
  float t2 = sens.readTemperature(3); // чтение датчика на пине 3
  float h2 = sens.readHumidity(3);  // чтение датчика на пине 3
  lcd.setCursor(1, 0); // загальга назва 
  lcd.print("Biltong Monitoring"); //  напис 
  lcd.setCursor(0, 1);
  lcd.print("w ");
  lcd.print("h=");
  lcd.print(h);
  lcd.print(" t=");
  lcd.print(t);
  lcd.setCursor(0, 2);
  lcd.print("b ");
  lcd.print("h=");
  lcd.print(h2);
  lcd.print(" t=");
  lcd.print(t2);
}

