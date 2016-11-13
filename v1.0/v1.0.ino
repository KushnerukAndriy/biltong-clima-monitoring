#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Wire.h> 

#define DHTPIN 2 // Пін для датчика температури
#define DHTTYPE DHT22 // Тип датчика температури

LiquidCrystal_I2C lcd(0x27, 20, 4); // Виставляємо адрасе екрану і  тип

DHT dht(DHTPIN, DHTTYPE); // Задаємо датчик
void setup()
{
  dht.begin(); // вмикаємо датчик температури
	lcd.begin(); // вмикаємо екран
	lcd.backlight(); // вмикаємо підсвітку 
  lcd.setCursor(1, 0); // загальга назва 
	lcd.print("Biltong Monitoring"); //  напис
}

void loop()
{
	float h = dht.readHumidity(); 
  float t = dht.readTemperature();

  if (isnan(t) || isnan(h)) { 
    Serial.println("Failed to read from DHT"); 
  } 
  else { 
    lcd.setCursor(0, 1);
    lcd.print("t = ");
    lcd.print(t);
    lcd.print("C");
    lcd.setCursor(0, 2);
    lcd.print("h = ");
    lcd.print(h);
    lcd.print("%");
  } 
}
