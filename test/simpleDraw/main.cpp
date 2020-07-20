#include "Arduino.h"
#include <avr/io.h>

extern "C"
{
  #include "mcu.h"
  #include "ILI9486.h"
}

struct DisplayInterface dInter;

uint32_t startMsec = 0;
void setup()
{
//  DDRB |= (1 << PORTB5);
//  PORTB |= (1 << PORTB5);
  Serial.begin(115200);

  // setup interface
  initializeDisplayInterface(&dInter);

  dInter.RESX_BANK = BANK_C;
  dInter.RESX_PIN = PORTC1;

  dInter.CSX_BANK = BANK_C;
  dInter.CSX_PIN = PORTC2;

  dInter.CX_BANK = BANK_C;
  dInter.CX_PIN = PORTC3;

  dInter.WRX_BANK = BANK_B;
  dInter.WRX_PIN = PORTB2;

  dInter.RDX_BANK = BANK_B;
  dInter.RDX_PIN = PORTB3;

  dInter.DB0_BANK = BANK_B;
  dInter.DB0_PIN = PORTB0;

  dInter.DB1_BANK = BANK_B;
  dInter.DB1_PIN = PORTB1;

  dInter.DB2_BANK = BANK_D;
  dInter.DB2_PIN = PORTD2;

  dInter.DB3_BANK = BANK_D;
  dInter.DB3_PIN = PORTD3;

  dInter.DB4_BANK = BANK_D;
  dInter.DB4_PIN = PORTD4;

  dInter.DB5_BANK = BANK_D;
  dInter.DB5_PIN = PORTD5;

  dInter.DB6_BANK = BANK_D;
  dInter.DB6_PIN = PORTD6;

  dInter.DB7_BANK = BANK_D;
  dInter.DB7_PIN = PORTD7;

  // optimize interface
  buildDisplayInterface(&dInter);

  Serial.write("Init Display...");
  initializeDisplay(&dInter);
  setDislayOrientation(&dInter, DISPLAY_ORIENTATION_PORTRAIT);
  Serial.write("Done\n");

  startMsec = millis();
}

uint16_t iterX = 0;
uint16_t iterY = 0;
uint16_t clearColor = 0x0000;
uint16_t data[] = {0x0FF0, 0x0000, 0x0000, 0x0000, 0x0000,
                   0x0000, 0x0FF0, 0x0000, 0x0000, 0x0000,
                   0x0000, 0x0000, 0x0FF0, 0x0000, 0x0000,
                   0x0000, 0x0000, 0x0000, 0x0FF0, 0x0FF0,
                  };
int dataSize = 20;
void loop()
{
  clearDisplay(&dInter, clearColor);

  Serial.write("Display Written in: ");
  Serial.print(millis() - startMsec);
  Serial.write("ms\n");
  
  clearColor= random(65000);
  startMsec = millis();

}
