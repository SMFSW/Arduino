#include <dht.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(5, 6, 9, 10, 11, 12);


void setup()
{
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.clear();
  
  Serial.begin(9600);
}

void loop()
{
    anaHumidity();
    anaTemp();
}

