#include <LiquidCrystal_I2C.h>
#define TEMP_PIN A0
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

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
//  Serial.println("Hello World");
//  delay(1000);
  int rawvoltage = analogRead(TEMP_PIN);
//  Serial.print("Analog val:");
//  Serial.println(rawvoltage);
  float millivolts= (rawvoltage/2048.0) * 3300;
  float celsius= millivolts/10;
  Serial.print(celsius);
  Serial.print(" degrees Celsius, ");
  float fahrenheit = (celsius * 9)/5 + 32;
  Serial.print(fahrenheit);
  Serial.println(" degrees Fahrenheit");
  lcd.setCursor(5, 0);
  lcd.print(String(celsius));
  lcd.setCursor(5, 1);
  lcd.print(String(fahrenheit));
  delay(1000);
}
