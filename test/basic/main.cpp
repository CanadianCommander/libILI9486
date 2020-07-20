#include "Arduino.h"
#include <avr/io.h>

extern "C"
{
  #include "ILI9486.h"
}

struct DisplayInterface dInter;

void setup()
{
//  DDRB |= (1 << PORTB5);
//  PORTB |= (1 << PORTB5);
  Serial.begin(115200);

  // setup interface
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

  Serial.write("Init Display...");
  initializeDisplay(&dInter);
  Serial.write("Done");
}

void loop()
{
  uint8_t response[] = {0x00, 0x00, 0x00, 0x00};
  getDisplayStatus(&dInter, response);

  if (response[0] == 0 && response[1] == 97 && response[2] == 0 && response[3] == 0)
  {
    Serial.write("Display status check. PASSED\n");
  }
  else
  {
    Serial.write("Display status check. FAILED\n");
  }

  delay(5000);
}
