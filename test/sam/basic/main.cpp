#include "Arduino.h"
#include "sam3x8e.h"

extern "C"
{
  #include "ILI9486.h"
}

struct DisplayInterface dInter;

void setup()
{
  Serial.begin(115200);

  // setup interface
  initializeDisplayInterface(&dInter);

  // setup interface
  dInter.RESX_BANK = BANK_C;
  dInter.RESX_PIN = getPinOffset(PIO_PER_P21);

  dInter.CSX_BANK = BANK_D;
  dInter.CSX_PIN = getPinOffset(PIO_PER_P7);

  dInter.CX_BANK = BANK_C;
  dInter.CX_PIN = getPinOffset(PIO_PER_P29);

  dInter.WRX_BANK = BANK_C;
  dInter.WRX_PIN = getPinOffset(PIO_PER_P22);

  dInter.RDX_BANK = BANK_C;
  dInter.RDX_PIN = getPinOffset(PIO_PER_P23);

  dInter.DB0_BANK = BANK_C;
  dInter.DB0_PIN = getPinOffset(PIO_PER_P13);

  dInter.DB1_BANK = BANK_B;
  dInter.DB1_PIN = getPinOffset(PIO_PER_P21);

  dInter.DB2_BANK = BANK_C;
  dInter.DB2_PIN = getPinOffset(PIO_PER_P6);

  dInter.DB3_BANK = BANK_C;
  dInter.DB3_PIN = getPinOffset(PIO_PER_P8);

  dInter.DB4_BANK = BANK_A;
  dInter.DB4_PIN = getPinOffset(PIO_PER_P19);

  dInter.DB5_BANK = BANK_C;
  dInter.DB5_PIN = getPinOffset(PIO_PER_P19);

  dInter.DB6_BANK = BANK_C;
  dInter.DB6_PIN = getPinOffset(PIO_PER_P17);

  dInter.DB7_BANK = BANK_C;
  dInter.DB7_PIN = getPinOffset(PIO_PER_P15);

  // optimize interface
  buildDisplayInterface(&dInter, false);

  Serial.write("Init Display...");
  initializeDisplay(&dInter);
  Serial.write("Done\n");
}

void loop()
{
  uint8_t response[] = {0x00, 0x00, 0x00, 0x00};
  getDisplayStatus(&dInter, response);

  Serial.write("======== STATUS BITS =======\n");
  Serial.print(response[0], BIN);
  Serial.write(",");
  Serial.print(response[1], BIN);
  Serial.write(",");
  Serial.print(response[2], BIN);
  Serial.write(",");
  Serial.print(response[3], BIN);
  Serial.write("\n");

  if (response[0] == 0x80 && response[1] == 0x53 && response[2] == 0x4 && response[3] == 0x0)
  {
    Serial.write("Display status check. PASSED\n");
  }
  else
  {
    Serial.write("Display status check. FAILED\n");
  }

  delay(5000);
}
