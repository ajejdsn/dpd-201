#include <SoftwareSerial.h>
#define LTX 2 
#define LRX -1 
// we don't need RX ^
SoftwareSerial vfd(LRX, LTX);
// EXAMPLE w/o TESTS
// https://github.com/ajejdsn/dpd-201
// =D

void setup()
{
  vfd.begin(9600);
  rein();
}
void loop() {
  
}

void printA(byte x) {
    vfd.write(eD(x));   // print ASCII (ex. A=65)
}

void rein() // clear screen
{
  vfd.write(231); // CLS
  delay(50);
} 

inline uint8_t eD(uint8_t x) {
  asm volatile (
    "LSL %0 \n\t" // ROL
    "ADC %0,__zero_reg__ \n\t" // x+0 + carry... wtf...
    "COM %0 \n\t" // NOT (~)
    :"+r" (x) // return
    );
    return x;
}

void printR(const char* t) {
  while (*t) {
    vfd.write(eD((uint8_t)*t)); // write ASCII text
    t++;
  }
}

/* 
 *  2412/30720 bytes of ROM
 *  126/2048 bytes of RAM.
 *  If I'll use hardware serial, it'll be better.
 *  Although, it's already looks good.
 */
