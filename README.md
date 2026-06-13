# dpd-201
## EDUCATIONAL PURPOSES ONLY
Adapted Version of niq_ro's repo

Sources:

Info: https://nicuflorica.blogspot.com/2021/07/afisaj-vfd-modul-datecs-dpd-201.html

Inversing code: https://github.com/tehniq3/DPD-201/tree/main

Datasheet: https://www.datecs.bg/en/downloads/pdf?id=DPD201_user.pdf

Schematics/Firmware: https://zipstore.ru/blog/ugolok-1rezistor-0/#rez8

# ENGLISH VERSION

#### **ATTENTION: It may be dangerous. The author is not responsible for your devices. Good luck**.

### 1.1: How to connect?

1: Solder the wires to specific CN1 areas (Fig. 1)

<img width="1278" height="884" alt="image" src="https://github.com/user-attachments/assets/753abad4-72d1-4517-872f-791b0f36ed58" />

### Fig. 1:**Power soldering points**. 

After soldering Power Points, solder wires to the TP6 point for input, and TP10 point for connecting Arduino ground with the Display ground.
**ATTENTION: TO AVOID CORRUPTIONS AND SOME VOLTAGE SPIKES, ADD THE DIODE TO TP6 WIRE END (ex. 1N5822)** 

2: Connect the Arduino as the scheme(the diagram is approximate! may be incorrect!)
<img width="632" height="431" alt="image" src="https://github.com/user-attachments/assets/5030fc5e-a5a9-4ddf-a63d-27102ef63bbf" />
### Fig. 2: Connection Block Diagram.


### 1.2 Using the display
After connecting, you need to upload code.

You can use these functions for:
- PrintR() for printing raw text(cyrillic text and some symbols doesn't works)
- PrintA() for printing ASCII symbols
- cls() for screen cleaning

### 1.3: Additional info.
By the specifications, display works at 10-24V voltage range. For the most correct display work, you can use 12V 1A Power supply, 19V Laptop Charger, or 24V DC PSU.
Since the display uses the L7805 D-PAK to power the logic, it is strongly recommended to use power supplies from 10 to 12V.
By specification, display can use US, ESC, CAN, CLR and some other commands, they can be sended by printA() 
I think, this display uses inverted TTL logic that inverts by the chip on the cash registrator M/B (example: MAX232 in Datecs FP-3530T), so we need to invert it by the software.

If you can improve something or tell about some issues, use the issues tab or contact this email: solidarqrd@gmail.com

UPD:
Disassembled the original firmware of display, and found something interesting.

```
/* WARNING: Instruction at (CODE,0x0a9d) overlaps instruction at (CODE,0x0a9c)
    */

byte FUN_CODE_09e8(byte param_1,undefined1 *param_2,byte param_3,char *param_4,byte *param_5,
                  char param_6,byte param_7_00,byte *param_8,byte param_9,byte param_10,
                  char param_11)

{
  byte *pbVar1;
  char cVar2;
  undefined1 uVar3;
  char cVar4;
  byte bVar5;
  byte bVar6;
  byte bVar7;
  
  if (2e.4 != '\x01') {
    bVar6 = FIFLG;
    FIFLG = *param_2;
    BANK2_R0 = BANK2_R0 | bVar6;
    bVar6 = FUN_CODE_0980(bVar6,(byte *)(param_4 + '\x01'),(char)param_8);
    return bVar6;
  }
  cVar2 = P0.0;
  if (cVar2 != '\0') {
    P0.0 = 0;
  }
  *param_5 = *param_5 - 1;
  uVar3 = FIFLG;
  DPXL = uVar3;
  pbVar1 = (byte *)*param_4;
  *param_4 = (param_1 + param_9) - ((char)param_4 - ((CARRY1(param_1,param_9) << 7) >> 7));
  if (24.0 != '\x01') {
    BANK2_R1 = BANK2_R1 | (byte)pbVar1;
    bVar6 = FUN_CODE_0949(pbVar1,param_2,param_3,param_5,param_6,param_7_00,param_8 + '\x01',param_9
                          ,param_10,param_11 - 1);
    return bVar6;
  }
  cVar2 = *param_4;
  *param_4 = (char)(pbVar1 + param_9) -
             (char)(param_4 + -((CARRY1((byte)pbVar1,param_9) << 7) >> 7));
  if (20.0 == '\x01') {
    bVar6 = param_9 << 1 | param_9 >> 7;
    uVar3 = P0;
    nop();
  }
  else {
    cVar4 = FIFLG;
    FIFLG = cVar2;
    nop();
    bVar7 = cVar4 + (param_9 -
                    (((pbVar1 + param_9 < param_4 + -((CARRY1((byte)pbVar1,param_9) << 7) >> 7)) <<
                     7) >> 7));
    BANK0_R0 = BANK0_R0 | bVar7;
    nop();
    *param_5 = *param_5 - 1;
    bVar5 = 0;
    bVar6 = bVar7;
    if (param_3 != 0) {
      bVar6 = bVar7 / param_3;
      bVar5 = bVar7 % param_3;
    }
    if (20.0 != '\x01') {
      bVar6 = FUN_CODE_0900(param_2,bVar5,(byte)(param_4 + -1),(char)param_5,param_6,param_8,param_9
                           );
      return bVar6;
    }
  }
  DAT_INTMEM_40 = DAT_INTMEM_40 + -1;
  return bVar6 << 1 | bVar6 >> 7; // HERE
}

```

So, it's not just the inverting.
I suppose that the final Arduino code will be like that:
```
inline uint8_t eD(uint8_t x) {
uint8_t a = (x>>1)|(x<<7); // ROL8(x, 1)
uint8_t b = a ^ 0x42; // XOR
return b;
}
```
...But it doesn't works.
I tested, and found this:
With this test code I can send text directly from Serial monitor.
```
#include <SoftwareSerial.h>

#define LTX 2
#define LRX -1

SoftwareSerial lcd(LRX, LTX, true);

void setup()
{
    Serial.begin(9600);
    lcd.begin(9600);
    lcd.write(12);
    lcd.print("test");
    delay(1000);
    lcd.write(12);
    Serial.println(F("READY"));
}

void loop()
{
    if (Serial.available())
    {
        int value = Serial.read();
            lcd.write(eD((uint8_t)value));
    }
}
inline uint8_t eD(uint8_t x) {
return x;
}
```
...But if we will use hardware serial instead of software, there are no more methods except `~((x<<1)|(x>>7))` formula... Soo, that's it ._.


# РУССКАЯ ВЕРСИЯ

#### **ВНИМАНИЕ: Это может быть опасно. Автор не несёт ответственности за ваши устройства. Удачи**.

### 1.1: Как подключить?

1: Припаяйте провода к соответствующим контактам CN1 (Рис. 1)

<img width="1278" height="884" alt="image" src="https://github.com/user-attachments/assets/753abad4-72d1-4517-872f-791b0f36ed58" />

### Рис. 1: **Точки пайки питания**.

После пайки точек питания припаяйте провода к точке TP6 для входного сигнала и к точке TP10 для соединения земли Arduino с землёй дисплея.  
**ВНИМАНИЕ: ЧТОБЫ ИЗБЕЖАТЬ ПОВРЕЖДЕНИЙ И СКАЧКОВ НАПРЯЖЕНИЯ, ДОБАВЬТЕ ДИОД НА КОНЕЦ ПРОВОДА TP6 (например, 1N5822)**

2: Подключите Arduino по схеме (диаграмма приблизительная, может быть неточной!)
<img width="632" height="431" alt="image" src="https://github.com/user-attachments/assets/5030fc5e-a5a9-4ddf-a63d-27102ef63bbf" />

### Рис. 2: Блок-схема подключения.

### 1.2 Использование дисплея
После подключения необходимо загрузить код.

Вы можете использовать следующие функции:
- printR() для вывода сырого текста (кириллица и некоторые символы не работают)

- printA() для вывода ASCII-символов

- cls() для очистки экрана


### 1.3: Дополнительная информация
Согласно спецификации, дисплей работает в диапазоне напряжений 10–24В. Для наиболее корректной работы можно использовать блок питания 12В 1А, зарядное устройство ноутбука 19В или источник питания 24В DC.
Исходя из того, что дисплей использует L7805 D-PAK для питания логики, категорически рекомендуется использовать источники питания от 10 до 12в.

По спецификациям, дисплей также умеет использовать US, ESC, CAN, CLR, и другие комманды, их можно отправлять последовательно по printA.

Предположительно, этот дисплей использует инвертированную TTL-логику, которая инвертируется микросхемой на материнской плате кассового аппарата (например: MAX232 в Datecs FP-3530T), поэтому её необходимо инвертировать программно.

Если вы хотите предложить доработку, либо сообщить о проблеме, то помимо специальной вкладки на GitHub вы можете сообщить о чём-либо по данной почте: solidarqrd@gmail.com



