extern "C"
{
  #include "ILI9486.h"
}

struct DisplayInterface dInter;

void setup() {

  // setup interface
  initializeDisplayInterface(&dInter);

  // setup interface
  dInter.RESX_BANK = BANK_A;
  dInter.RESX_PIN = getPinOffset(GPIO_PIN_7);

  dInter.CSX_BANK = BANK_B;
  dInter.CSX_PIN = getPinOffset(GPIO_PIN_0);

  dInter.CX_BANK = BANK_B;
  dInter.CX_PIN = getPinOffset(GPIO_PIN_1);

  dInter.WRX_BANK = BANK_B;
  dInter.WRX_PIN = getPinOffset(GPIO_PIN_2);

  dInter.RDX_BANK = BANK_B;
  dInter.RDX_PIN = getPinOffset(GPIO_PIN_10);

  dInter.DB0_BANK = BANK_B;
  dInter.DB0_PIN = getPinOffset(GPIO_PIN_13);

  dInter.DB1_BANK = BANK_B;
  dInter.DB1_PIN = getPinOffset(GPIO_PIN_12);

  dInter.DB2_BANK = BANK_A;
  dInter.DB2_PIN = getPinOffset(GPIO_PIN_12);

  dInter.DB3_BANK = BANK_A;
  dInter.DB3_PIN = getPinOffset(GPIO_PIN_11);

  dInter.DB4_BANK = BANK_A;
  dInter.DB4_PIN = getPinOffset(GPIO_PIN_9);

  dInter.DB5_BANK = BANK_A;
  dInter.DB5_PIN = getPinOffset(GPIO_PIN_8);

  dInter.DB6_BANK = BANK_B;
  dInter.DB6_PIN = getPinOffset(GPIO_PIN_15);

  dInter.DB7_BANK = BANK_B;
  dInter.DB7_PIN = getPinOffset(GPIO_PIN_14);

  // start display
  buildDisplayInterface(&dInter, false);
  initializeDisplay(&dInter);
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  //#ifdef ARDUINO_ARCH_STM32

  uint8_t response[] = {0x00, 0x00, 0x00, 0x00};
  getDisplayStatus(&dInter, response);

  if (response[0] == 0x80 && response[1] == 0x53 && response[2] == 0x4 && response[3] == 0x0)
  {
    //because serial communication to the PC is not avaiable blink LED to indicate result.
    // LONG == PASS, SHORT == FAIL
    digitalWrite(LED_BUILTIN, HIGH);
    delay(5000);
    digitalWrite(LED_BUILTIN, LOW);
  }
  else
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
  }

  delay(5000);
}
