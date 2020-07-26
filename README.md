## libILI9486
A ILI9486 display driver for Arduino boards. Basically this library allows you to draw
pretty pictures on your LCD!

## Supported Hardware
#### MCU
- AVR boards. tested on Nano / Uno. should work on Mega.
- SAM boards. tested on sam3x8e (Arduino Due).
- STM32 boards. tested on STN32F411CE (Black Pill).

### Interface
- Supports 8 bit parallel interface. with plans to support 16bit, when I actually have a     16bit display to test with


## Getting Started
Clone this repo to your Arduino library folder as you would any other Arduino library.
Include the header file. Note because this is a C library its a little different.
```
extern "C"
{
  #include "ILI9486.h"
}
```
Then configure the pins you intend to use for your display. Mapping them as below.
Note this is an `AVR` example. See other board examples under `test` folder
```
struct DisplayInterface displayInterface;

// initialize interface struct
initializeDisplayInterface(&displayInterface);

// setup interface
displayInterface.RESX_BANK = BANK_C;
displayInterface.RESX_PIN = PORTC1;

displayInterface.CSX_BANK = BANK_C;
displayInterface.CSX_PIN = PORTC2;

displayInterface.CX_BANK = BANK_C;
displayInterface.CX_PIN = PORTC3;

displayInterface.WRX_BANK = BANK_B;
displayInterface.WRX_PIN = PORTB2;

displayInterface.RDX_BANK = BANK_B;
displayInterface.RDX_PIN = PORTB3;

displayInterface.DB0_BANK = BANK_B;
displayInterface.DB0_PIN = PORTB0;

displayInterface.DB1_BANK = BANK_B;
displayInterface.DB1_PIN = PORTB1;

displayInterface.DB2_BANK = BANK_D;
displayInterface.DB2_PIN = PORTD2;

displayInterface.DB3_BANK = BANK_D;
displayInterface.DB3_PIN = PORTD3;

displayInterface.DB4_BANK = BANK_D;
displayInterface.DB4_PIN = PORTD4;

displayInterface.DB5_BANK = BANK_D;
displayInterface.DB5_PIN = PORTD5;

displayInterface.DB6_BANK = BANK_D;
displayInterface.DB6_PIN = PORTD6;

displayInterface.DB7_BANK = BANK_D;
displayInterface.DB7_PIN = PORTD7;
```
Finally "build" the interface, and initialize the display
```
buildDisplayInterface(&displayInterface, false);
initializeDisplay(&displayInterface);
```
At this point you are ready to draw to the display! All available functions are listed in "ILI9486.h" with descriptions. There are also complete examples / tests under the `test` directory. These tests require [Arduino-Makefile](https://github.com/sudar/Arduino-Makefile) to build, except for STM32 which requires `arduino-cli` (because Arduino Make doesn't seem to support it).

## Pro tip
Compiling your program with `-O2` gives a massive boost to performance, without taking up to much more FLASH.
