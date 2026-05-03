#include <SoftwareSerial.h>

SoftwareSerial lcd(255, 2);
// EXAMPLE w/o TESTS
// https://github.com/ajejdsn/dpd-201
// =D

void setup()
{
  lcd.begin(9600);
  Serial.begin(9600);
  // lcd.write(201); // ESC
  lcd.write(231); // CLS
  delay(300);
  rein();
  Serial.println("Display is ready!");
}

void loop() {
  
}



void cls() {
  lcd.write(231);
}


void printR(String text) {
   for (int i = 0; i < text.length(); i++)
  {
    lcd.write(255-2*text[i]);   
  }
}

void printA(int symbolsa) {

    lcd.write(255-2*symbolsa);   
}

void rein()
{
  lcd.write(231); // CLS
  delay(50);
} 
