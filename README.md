# dpd-201
## EDUCATIONAL PURPOSES ONLY
Adapted Version of niq_ro's repo

ima beginner, pls dont judge too harshly, thats my first project (o.-)

Sources:

Info: https://nicuflorica.blogspot.com/2021/07/afisaj-vfd-modul-datecs-dpd-201.html

Inversing code: https://github.com/tehniq3/DPD-201/tree/main

Datasheet: https://www.datecs.bg/en/downloads/pdf?id=DPD201_user.pdf

Schematics/Firmware: https://zipstore.ru/blog/ugolok-1rezistor-0/#rez8

DISPLAY CONNECT:
Arduino:D2
STM32:PA9 for TX, PA10 for RX

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
By the specifications, display works at 10-24V voltage range. For the most correct display work, you can use 12V 1A Power supply.
Since the display uses the L7805 D-PAK to power the logic, it is strongly recommended to use power supplies from 10 to 12V.
~~By specification, display can use US, ESC, CAN, CLR and some other commands, they can be sended by printA()~~
~~I think, this display uses inverted TTL logic that inverts by the chip on the cash registrator M/B (example: MAX232 in Datecs FP-3530T), so we need to invert it by the software.~~

If you can improve something or tell about some issues, use the issues tab or contact this email: solidarqrd@gmail.com

### UPD1: 
There are some mistakes in my troughts.

Researched the display and cash register schematics, and find out this thing:
In the ECR motherboard we have a MAX232 or ADM202 IC at the Customer Display port.

...But in the display schematic we have a transistor cascade instead of IC, it will remove -12V signal, but all the common-emitter circuits has one problem: The signal inverts.

..So the display developers used a software inverting in the CPU code. Somehow it works not only with 12V signals, but also device can normally receive 5V, and, I guess, 3.3V TTL.

~~Also I guess that we can make even the complete UART, because one of the CN1 pins goes to the C1162A Dout pin(U2). However, I don't know what the logic level used in output, so it'll be risky.~~ nah dout has a separate testpoint, another mistake -_-
### UPD 2:
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
### UPD3:

Working at the STM32 version of code. Used IC: STM32F103C8T6

Found this thing:

...Probably, code works, but we need to use a 3.3v-5v level shifter circuit because display works with 5-12v logic levels only, gonna solder it and publish all the schematics once upon a time. 

### UPD4:

Another code update, and... Yeah, it works! Honestly, that was pretty difficult.

But at least, I have a 1st version of STM32 code version. ^^


## UPD5 2026 SEP 25: <br>
You can bypass old and slow controller.<br>
How? Pretty easy. <br>
You can connect to some testpoints on the board:<br>
- TP4(CL)
- TP5(LS)
- TP2(CLK)
- TP3(DIN);

  
  And control high-voltage shift registers by Arduino, without U1<br>
  
Based on my calculations, you can send up to 1420 bits in total if you light up every single symbol (40 symbols with 7x5 segments each, plus 20 annunciators).

If that's the case and we assume the panel has 20 GRID lines, we get:
1420 / 20 = 71 bits per GRID

So with each multiplexing step, you just need to shift in about 71 bits, latch them with LS, and switch over to the next GRID.

Two MSC1162A chips give you 80 outputs, so 71 bits fit easily into a simple two-chip cascade.
The MSC1162A packs a 40-bit shift register and a 40-bit latch, topping out at a 4 MHz CLK rate.

And here's where the math kicks in.

At the max clock of 4 MHz:
T<sub>CLK</sub> = 250 ns

For 71 bits:
T<sub>shift</sub> = 71 * 250 ns = 17.75 μs

So loading data for a single GRID takes a mere 17.75 μs.
Now for the best part.

The MSC1162A datasheet doesn't just limit CLK to 4 MHz, it also specifies:
f<sub>DRV(max)</sub> = 15 kHz

Meaning the max switching frequency for the output driver is 15 kHz.
If we really are dealing with 20 GRID lines, then based on this parameter:

$$f_{frame(max)} = \frac{15\text{ kHz}}{20}$$

That gives us a maximum refresh rate of 750 Hz across 20 GRID lines. Pretty neat, right?

On top of that, at 750 Hz:
$$T_{GRID} = \frac{1}{15\text{ kHz}} = 66.67\ \mu\text{s}$$

Out of this window, sending 71 bits at 4 MHz takes up 17.75 μs, leaving a solid 48.92 μs for the active display window and other timing logic.

So running it at 4 MHz actually makes total sense. The MSC1162A is built for exactly this — the datasheet explicitly lists a 4 MHz data transfer speed.

... But 750 Hz is far from a walk in the park. That’s pushing the driver right to its limit. <br>
It matches the driver outputs' absolute maximum frequency specified in the datasheet: f_DRV(max) = 15 kHz. So 750 Hz is <br>literally running the MSC1162A right on the edge of its specs, not leaving any safety margin.<br>

In this setup, each GRID gets:<br>
T_GRID = 1 / 15 kHz = 66.67 µs<br>

And shifting 71 bits at CLK = 4 MHz takes:<br>
T_shift = 71 * 250 ns = 17.75 µs<br>

Leaving 48.92 µs. So on the digital side, everything fits comfortably.<br>
However, the phosphor and analog side require separate, crazy-delicate attention.<br>
A 10–15 µs persistence/excitation delay isn't a problem for 750 Hz on its own. The full frame period is only 1.333 ms, and<br> a single GRID is active for roughly 66.7 µs. Even if the phosphor keeps glowing for another 15 µs after turning off, that’s ~22.5% of the GRID interval.<br>
<br>
Fuck, I got confused. But basically, 750 Hz is still totally doable<br>.
<br>
Something else bothers me way more: the MSC1162A datasheet lists pretty huge output transition times.<br>
Under V_HV = 65V conditions, t_DLH / t_DHL can reach up to 1 ms, and t_TLH / t_THL up to 5 ms.<br>
This looks super weird compared to the 15 kHz figure, so I wouldn't take these maximum values literally as a requirement to<br> wait 5 ms after every toggle, otherwise the chip's own spec would contradict its claimed f_DRV limit.<br>
Still, it's a solid reason to check the actual scope traces on this specific panel.<br>
<br>
Another key point: you can shift data in parallel with the current GRID's illumination, because the MSC1162A has separate<br> 40-bit shift registers and latches.<br>
While you're pushing in the next data set, the outputs keep holding the data from the latch.<br>
Then, LS transfers the new set into the output latch.<br>
<br>
So I'd picture the flow like this:<br>
<br>
GRID N is lit -> protocol shifts data for GRID N+1 -> turn off GRID N -> trigger LS -> switch to GRID N+1.<br>
<br>
That way, those 17.75 µs of data transfer don't eat into the active display time at all.<br>
<br>
And here’s my main takeaway:<br>
<br>
600 Hz is the safest bet. That's EXACTLY what we should target.<br>
<br>
Whew.... What the fuck...<br>

