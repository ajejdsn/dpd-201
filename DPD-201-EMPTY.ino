#include <SoftwareSerial.h>
#define LTX 2 
#define LRX -1 
// we don't need RX ^
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

void printA(byte x) {

    lcd.write(eD(x));   // print ASCII (ex. A=65)
}

void rein() // clear screen
{
  lcd.write(231); // CLS
  delay(50);
} 

inline uint8_t eD(uint8_t x) {
  return ~((x<<1)|(x>>7));  // encode
}

void printR(const char* text) {
  while (*text) {
    lcd.write(eD((uint8_t)*text)); // write ASCII text
    text++;
  }
}
