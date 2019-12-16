#include <DallasTemperature.h>
#include <OneWire.h>
#include <LiquidCrystal_I2C.h>

const int temp_data_pin = 4;
OneWire oneWire(temp_data_pin);
DallasTemperature sensor(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  sensor.begin();
  
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("C:");
  lcd.setCursor(0,1);
  lcd.print("F:");

}

void loop() {
  // put your main code here, to run repeatedly:
   sensor.requestTemperatures();
   float celsius, fahrenheit;
   celsius = sensor.getTempCByIndex(0);
   fahrenheit = sensor.getTempFByIndex(0);
   Serial.print("Celsius:");
   Serial.print(celsius);
   Serial.print("\tFahrenheit:");
   Serial.println(fahrenheit);
   lcd.setCursor(5, 0);
   lcd.print(String(celsius));
   lcd.setCursor(5, 1);
   lcd.print(String(fahrenheit));
   delay(1000);
}
