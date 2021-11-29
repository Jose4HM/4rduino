#include <Wire.h> // This library is already built in to the Arduino IDE
#include <LiquidCrystal_I2C.h> //This library you can add via Include Library > Manage Library > 
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  Serial.begin(115200);
 Wire.begin(D5, D6); //(SDA, SCL) 
 lcd.home();  // initializing the LCD
 lcd.backlight(); // Enable or Turn On the backlight

lcd.setCursor(4, 0); //(Column 5 out of 16 and Row 1 out of 2)
lcd.print("Mindstorm"); // Start Printing
lcd.setCursor(3, 1);//(Column 4 out of 16 and Row 2 out of 2)
lcd.print("Engineering"); // Start Printing
}
void loop()
{
 // No Action!
}
