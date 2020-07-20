#ifndef LIBILI9486_MCU_H
#define LIBILI9486_MCU_H

#include "ILI9486.h"
#include <stdio.h>

// =========== PINS ==================
#define PIN_RESX 0 // reset
#define PIN_CSX 1 // chip select
#define PIN_CX 2 // command select
#define PIN_WRX 3 // write signal
#define PIN_RDX 4 // read signal

#define PIN_DB0 5
#define PIN_DB1 6
#define PIN_DB2 7
#define PIN_DB3 8
#define PIN_DB4 9
#define PIN_DB5 10
#define PIN_DB6 11
#define PIN_DB7 12

#define PIN_DB8 13
#define PIN_DB9 14
#define PIN_DB10 15
#define PIN_DB11 16
#define PIN_DB12 17
#define PIN_DB13 18
#define PIN_DB14 19
#define PIN_DB15 20
///////////////PINS////////////////////

// ========= OPTIMIZATION STRUCT ===========
#ifdef ARDUINO_ARCH_AVR

#define OP_PORT_START 0
#define OP_DDR_START 42
#define OP_PIN_OFFSET_START 84

struct OptimizedPins {
  // ORDER OF FIELDS EXTREMELY IMPORTANT
  volatile uint8_t* PIN_RESX_PORT;
  volatile uint8_t* PIN_CSX_PORT;
  volatile uint8_t* PIN_CX_PORT;
  volatile uint8_t* PIN_WRX_PORT;
  volatile uint8_t* PIN_RDX_PORT;

  volatile uint8_t* PIN_DB0_PORT;
  volatile uint8_t* PIN_DB1_PORT;
  volatile uint8_t* PIN_DB2_PORT;
  volatile uint8_t* PIN_DB3_PORT;
  volatile uint8_t* PIN_DB4_PORT;
  volatile uint8_t* PIN_DB5_PORT;
  volatile uint8_t* PIN_DB6_PORT;
  volatile uint8_t* PIN_DB7_PORT;
  volatile uint8_t* PIN_DB8_PORT;
  volatile uint8_t* PIN_DB9_PORT;
  volatile uint8_t* PIN_DB10_PORT;
  volatile uint8_t* PIN_DB11_PORT;
  volatile uint8_t* PIN_DB12_PORT;
  volatile uint8_t* PIN_DB13_PORT;
  volatile uint8_t* PIN_DB14_PORT;
  volatile uint8_t* PIN_DB15_PORT;

  volatile uint8_t* PIN_RESX_DDR;
  volatile uint8_t* PIN_CSX_DDR;
  volatile uint8_t* PIN_CX_DDR;
  volatile uint8_t* PIN_WRX_DDR;
  volatile uint8_t* PIN_RDX_DDR;

  volatile uint8_t* PIN_DB0_DDR;
  volatile uint8_t* PIN_DB1_DDR;
  volatile uint8_t* PIN_DB2_DDR;
  volatile uint8_t* PIN_DB3_DDR;
  volatile uint8_t* PIN_DB4_DDR;
  volatile uint8_t* PIN_DB5_DDR;
  volatile uint8_t* PIN_DB6_DDR;
  volatile uint8_t* PIN_DB7_DDR;
  volatile uint8_t* PIN_DB8_DDR;
  volatile uint8_t* PIN_DB9_DDR;
  volatile uint8_t* PIN_DB10_DDR;
  volatile uint8_t* PIN_DB11_DDR;
  volatile uint8_t* PIN_DB12_DDR;
  volatile uint8_t* PIN_DB13_DDR;
  volatile uint8_t* PIN_DB14_DDR;
  volatile uint8_t* PIN_DB15_DDR;

  uint8_t PIN_RESX_PIN_OFFSET;
  uint8_t PIN_CSX_PIN_OFFSET;
  uint8_t PIN_CX_PIN_OFFSET;
  uint8_t PIN_WRX_PIN_OFFSET;
  uint8_t PIN_RDX_PIN_OFFSET;

  uint8_t PIN_DB0_PIN_OFFSET;
  uint8_t PIN_DB1_PIN_OFFSET;
  uint8_t PIN_DB2_PIN_OFFSET;
  uint8_t PIN_DB3_PIN_OFFSET;
  uint8_t PIN_DB4_PIN_OFFSET;
  uint8_t PIN_DB5_PIN_OFFSET;
  uint8_t PIN_DB6_PIN_OFFSET;
  uint8_t PIN_DB7_PIN_OFFSET;
  uint8_t PIN_DB8_PIN_OFFSET;
  uint8_t PIN_DB9_PIN_OFFSET;
  uint8_t PIN_DB10_PIN_OFFSET;
  uint8_t PIN_DB11_PIN_OFFSET;
  uint8_t PIN_DB12_PIN_OFFSET;
  uint8_t PIN_DB13_PIN_OFFSET;
  uint8_t PIN_DB14_PIN_OFFSET;
  uint8_t PIN_DB15_PIN_OFFSET;

  uint8_t PIXEL_BANK_MASK_B;
  uint8_t PIXEL_BANK_MASK_C;
  uint8_t PIXEL_BANK_MASK_D;
  // ORDER OF FIELDS EXTREMELY IMPORTANT
};

#endif

void setPin(struct DisplayInterface* dInterface, uint8_t pin, uint8_t value);

uint8_t readPin(struct DisplayInterface* dInterface, uint8_t pin);

void setPinDirection(struct DisplayInterface* dInterface, uint8_t pin, uint8_t output);

void setPixelBus8(struct DisplayInterface* dInterface, uint8_t value);

/**
* Fill out the ARCH specific optimization structure.
* @param dInterface - the display interface
* @param optPins - the optimized pins structure to be filled out;
*/
void optimizePins(struct DisplayInterface* dInterface, struct OptimizedPins * optPins);

#endif //LIBILI9486_MCU_H
