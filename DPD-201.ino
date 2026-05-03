#include <SoftwareSerial.h>

SoftwareSerial lcd(255, 2);
// EXAMPLE w.TESTS
// https://github.com/ajejdsn/dpd-201
// =D




String text7 = "   DATECS DPD-201   ";
String text8 = "                         BY:S.A.      ";
void setup()
{
  lcd.begin(9600);
  Serial.begin(9600);
  // lcd.write(201); // ESC
  lcd.write(231); // CLS
  delay(300);
  test();
}

void loop() {
  
}
void test()
{
  //if (Serial.available()) {
   // int value = Serial.parseInt();
   // lcd.write(value);              
 // }
 rein();  
 for (int i = 0; i < text7.length(); i++)
  {
    lcd.write(255-2*text7[i]);   
  }
  delay(3000);
  // rein();
for (int i = 0; i < text8.length(); i++)
  {
    lcd.write(255-2*text8[i]);   
  }
delay(5000);
rein();  
printR("PrintR Test");
delay(3000);
rein();
printA(112); // p
printA(114); // r
printA(105); // i
printA(110); // n
printA(116); // t
printA(65); // A
printA(10); // LF(New line)
printA(62); // >
printA(95); // _
printA(60); // <
delay(15000);
rein();
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

void printA(int symbol) {

    lcd.write(255-2*symbol);   
}

void rein()
{
  lcd.write(231); // CLS
  delay(50);
} 
