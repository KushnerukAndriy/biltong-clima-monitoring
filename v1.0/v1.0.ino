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
  lcd.print("Biltong Monitoring"); //  титульний запис
  dht1.begin(); // вмикаємо датчик 1
  dht2.begin(); // вмикаємо датчик 2
}

void loop() {

  float h1 = dht1.readHumidity();     // присвоюємо значення вологості 1 датчика в змінну
  float t1 = dht1.readTemperature();  // присвоюємо значення температури 1 датчика в змінну
  float h2 = dht2.readHumidity();     // присвоюємо значення вологості 2 датчика в змінну
  float t2 = dht2.readTemperature();  // присвоюємо значення температури 2 датчика в змінну

  if (isnan(t1) || isnan(h1)) {       // перевіряємо чи існують дані з першого датчика
    lcd.setCursor(0, 1);               // ставимо курсор в 2 стрічку
    lcd.print("Failed to read from DHT1"); // якщо дані не існують - пишем про це
  }
  else if (isnan(t2) || isnan(h2)) { // перевіряємо чи існують дані з другого датчика
    lcd.setCursor(0, 2);            // ставимо курсор в 3 стрічку
    lcd.print("Failed to read from DHT2"); // якщо дані не існують - пишем про це
  }
  else { // якщо дані існують тоді:
    lcd.setCursor(0, 1);  // ставимо курсор в 2 стрічку
    lcd.print("h1=");     // виводимо напис h1
    lcd.print(h1);        // вивлдимо значення h1 - вологість першого датчику
    lcd.print("%");       // виводимо знак % 
    lcd.print(" t1=");    // виводимо напис t1 
    lcd.print(t1);        // вовдимо значення t1 - температура першого датчику
    lcd.print("C");       // виводимо знак C
    lcd.setCursor(0, 2);  // ставимо курсор в 3 стрічку
    lcd.print("h2=");     // виводимо напис h2
    lcd.print(h2);        // вивлдимо значення h2 - вологість першого датчику
    lcd.print("%");       // виводимо знак % 
    lcd.print(" t2=");    // виводимо напис t2 
    lcd.print(t2);        // вовдимо значення t2 - температура другого датчику
    lcd.print("C");       // виводимо знак C
    lcd.setCursor(1, 3);  // ставимо курсор в 4 стрічку
    lcd.print("h=");      // виводимо напис h
    lcd.print((h1+h2)/2); // рахуємо середнє значення вологості і виводимо
    lcd.print("%");       // виводимо знак % 
    lcd.setCursor(10, 3); // ставимо курсор в 4 стрічку на 11 позицію
    lcd.print(" t=");     //   виводимо напис t2 
    lcd.print((t1+t2)/2); // рахуємо середнє значення температури і виводимо
    lcd.print("C");       // виводимо знак C
  }
}
