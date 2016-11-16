#include "DHT.h"
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // Виставляємо адрасе екрану і  тип

DHT dht1(2, DHT22);
DHT dht2(3, DHT21);

void setup() {
  lcd.begin(); // вмикаємо екран
  lcd.backlight(); // вмикаємо підсвітку 
  lcd.setCursor(1, 0); // загальга назва 
  lcd.print("Biltong Monitoring"); //  напис;
  dht1.begin(); 
  dht2.begin();
}

void loop() {

  float h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature();
  float h2 = dht2.readHumidity();
  float t2 = dht2.readTemperature();

  if (isnan(t1) || isnan(h1)) {
    //lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Failed to read from DHT1");

    //    Serial.println("Failed to read from DHT1");
  }
  else if (isnan(t2) || isnan(h2)) {
    //lcd.clear();
    lcd.setCursor(0, 2);
    lcd.print("Failed to read from DHT2");

    //    Serial.println("Failed to read from DHT");
  }
  else {

    //-----------------dht 1
    lcd.setCursor(0, 1);
    lcd.print("h1=");
    lcd.print(h1);
    lcd.print("%");
    lcd.print(" t1=");
    lcd.print(t1);
    lcd.print("C");
    //-----------------dht 2
    lcd.setCursor(0, 2);
    lcd.print("h2=");
    lcd.print(h2);
    lcd.print("%");
    lcd.print(" t2=");
    lcd.print(t2);
    lcd.print("C");
    // serredne
    lcd.setCursor(1, 3);
    lcd.print("h=");
    lcd.print((h1+h2)/2);
    lcd.print("%");
    lcd.setCursor(10, 3);
    lcd.print(" t=");
    lcd.print((t1+t2)/2);
    lcd.print("C");
  }
}
