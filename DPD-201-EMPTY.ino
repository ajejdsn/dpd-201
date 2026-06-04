#include <SoftwareSerial.h>
#define LTX 2
#define LRX -1
SoftwareSerial lcd(LRX, LTX);
// EXAMPLE w/o TESTS
// https://github.com/ajejdsn/dpd-201
// =D

void setup()
{
  lcd.begin(9600);
  Serial.begin(9600);
  lcd.write(231); // CLS
  delay(300);
  rein();
  Serial.println(F("READY!!!"));
}
void loop() {
  
}

void printA(uint16_t x) {

    lcd.write(255-x<<1);   
}

void rein()
{
  lcd.write(231); // CLS
  delay(50);
} 

inline uint8_t eD(uint8_t x) {
  return 255 - (x << 1); 
}

void pR(const char* text) {
  while (*text) {
    lcd.write(eD((uint8_t)*text)); 
    text++;
  }
}
