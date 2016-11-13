#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2 
#define DHTTYPE DHT22

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
	// initialize the SERIAL PORT
	Serial.begin(9600); 
  Serial.println("DHTxx test!");
  dht.begin(); 
	// initialize the LCD
	lcd.begin();
	// Turn on the blacklight and print a message.
	lcd.backlight();
  lcd.setCursor(6, 0);
	lcd.print("TEST");
}

void loop()
{
	float h = dht.readHumidity(); 
  float t = dht.readTemperature();

  if (isnan(t) || isnan(h)) { 
    Serial.println("Failed to read from DHT"); 
  } 
  else { 
    Serial.print("Humidity: "); 
    Serial.print(h); 
    Serial.print(" %\t"); 
    Serial.print("Temperature: "); 
    Serial.print(t); 
    Serial.println(" *C"); 
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
