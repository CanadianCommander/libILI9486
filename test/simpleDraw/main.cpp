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
  setDislayOrientation(&dInter, DISPLAY_ORIENTATION_LANDSCAPE);
  Serial.write("Done\n");

  clearDisplay(&dInter, random(65000));

  startMsec = millis();
}

uint16_t clearColor = 0x0000;
uint16_t dataDimensions = 9; // 9x9
float dataScale = 1;
uint16_t data[] = {0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFF00,
                   0x0000, 0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFF00, 0x0000,
                   0x0000, 0x0000, 0x00FF, 0x0000, 0x0000, 0x0000, 0xFF00, 0x0000, 0x0000,
                   0x0000, 0x0000, 0x0000, 0x00FF, 0x0000, 0xFF00, 0x0000, 0x0000, 0x0000,
                   0x0000, 0x0000, 0x0000, 0x0000, 0x00FF, 0x0000, 0x0000, 0x0000, 0x0000,
                   0x0000, 0x0000, 0x0000, 0xFF00, 0x0000, 0x00FF, 0x0000, 0x0000, 0x0000,
                   0x0000, 0x0000, 0xFF00, 0x0000, 0x0000, 0x0000, 0x00FF, 0x0000, 0x0000,
                   0x0000, 0xFF00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00FF, 0x0000,
                   0xFF00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00FF,
                  };

uint8_t data1bppDimensions = 16; // 16x16
uint8_t data1bpp[] = {0b00000000, 0b00000000,
                      0b00111000, 0b00111000,
                      0b00111000, 0b00111000,
                      0b00000000, 0b00000000,
                      0b00000000, 0b00000000,
                      0b00000000, 0b00000000,
                      0b00000000, 0b00000000,
                      0b00000000, 0b00000000,
                      0b11000000, 0b00000011,
                      0b00110000, 0b00001100,
                      0b00001110, 0b01110000,
                      0b00000001, 0b10000000,
                      0b00000000, 0b00000000,
                      0b00000000, 0b00000000,
                      0b00000000, 0b00000000,
                      0b00000000, 0b00000000,
                    };
void loop()
{
  // WIDTH / HEIGHT flipped because we are in landscape
  for (unsigned int y = dataDimensions*dataScale; y < DISPLAY_WIDTH - dataDimensions*dataScale; y +=dataDimensions*dataScale)
  {
    for (unsigned int x = dataDimensions*dataScale; x < DISPLAY_HEIGHT - dataDimensions*dataScale; x +=dataDimensions*dataScale)
    {
      writeDisplay(&dInter, data, x, y, dataDimensions, dataDimensions, dataScale);
    }
  }

  uint16_t clearColor = random(65000);
  clearDisplay(&dInter, clearColor);

  // WIDTH / HEIGHT flipped because we are in landscape
  for (unsigned int y = 0; y < DISPLAY_WIDTH; y +=data1bppDimensions * 2)
  {
    for (unsigned int x = 0; x < DISPLAY_HEIGHT; x +=data1bppDimensions * 2)
    {
      writeDisplay1bpp(&dInter, data1bpp, random(65000), clearColor, x, y, data1bppDimensions, data1bppDimensions);
    }
  }

  dataScale += 1;
  if((int)dataScale % 10 == 0)
  {
    dataScale = 1;
  }

  Serial.write("Test complete in: ");
  Serial.print(millis() - startMsec);
  Serial.write("ms\n");

  clearColor= random(65000);
  startMsec = millis();

}
