#include "DHT.h"
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // Виставляємо адрасе екрану і  тип

DHT dht1(2, DHT22);
DHT dht2(3, DHT21);
int Relay1 = 8;

void setup() {
  pinMode(Relay1, OUTPUT);
  lcd.begin(); // вмикаємо екран
  lcd.backlight(); // вмикаємо підсвітку 
  lcd.setCursor(1, 0); // загальга назва 
  lcd.print("Biltong Monitoring"); //  титульний запис
  dht1.begin(); // вмикаємо датчик 1
  dht2.begin(); // вмикаємо датчик 2
}

void loop() {

    float h1 = dht1.readHumidity();     // присвоюємо значення вологості 1 датчика в змінну
    float t1 = dht1.readTemperature();  // присвоюємо значення температури 1 датчика в змінну
    float h2 = dht2.readHumidity();     // присвоюємо значення вологості 2 датчика в змінну
    float t2 = dht2.readTemperature();  // присвоюємо значення температури 2 датчика в змінну
    lcd.setCursor(0, 1);  // ставимо курсор в 2 стрічку
    lcd.print(String("h1=") + String(h1) + String("%"));
    lcd.setCursor(11, 1);
    lcd.print(String("t1=") + String(t1) + String("C"));
    lcd.setCursor(0, 2);
    lcd.print(String("h2=") + String(h2) + String("%"));     
    lcd.setCursor(11, 2);
    lcd.print(String("t2=") + String(t2) + String("C"));
    if ( h2 < 63 )      // Якщо вологість падає нижче за 63% вмикаємо реле
        { digitalWrite(Relay1, LOW);
          lcd.setCursor(0, 3);
          lcd.print ("Relay 1");
        }
    if ( h2 > 73 )     // Якщо вологість піднімається вище 73% вимикаємо реле
        { digitalWrite(Relay1, HIGH);
          lcd.setCursor(0, 3);
          lcd.print ("Relay 0");
        }
        
}
