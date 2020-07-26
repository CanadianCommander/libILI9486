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

#ifdef ARDUINO_ARCH_AVR
//=================== ARCH AVR =========================

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

  bool unsafeOptimizations;
  uint8_t PIXEL_BANK_MASK_B;
  uint8_t PIXEL_BANK_MASK_C;
  uint8_t PIXEL_BANK_MASK_D;
  // ORDER OF FIELDS EXTREMELY IMPORTANT
};

#elif ARDUINO_ARCH_SAM
//=================== ARCH SAM =========================
#define OP_PIO_SODR_START 0
#define OP_PIN_MASK_START 84

struct OptimizedPins
{
  // ORDER OF FIELDS EXTREMELY IMPORTANT
  volatile uint32_t* PIN_RESX_PIO_SODR;
  volatile uint32_t* PIN_CSX_PIO_SODR;
  volatile uint32_t* PIN_CX_PIO_SODR;
  volatile uint32_t* PIN_WRX_PIO_SODR;
  volatile uint32_t* PIN_RDX_PIO_SODR;

  volatile uint32_t* PIN_DB0_PIO_SODR;
  volatile uint32_t* PIN_DB1_PIO_SODR;
  volatile uint32_t* PIN_DB2_PIO_SODR;
  volatile uint32_t* PIN_DB3_PIO_SODR;
  volatile uint32_t* PIN_DB4_PIO_SODR;
  volatile uint32_t* PIN_DB5_PIO_SODR;
  volatile uint32_t* PIN_DB6_PIO_SODR;
  volatile uint32_t* PIN_DB7_PIO_SODR;
  volatile uint32_t* PIN_DB8_PIO_SODR;
  volatile uint32_t* PIN_DB9_PIO_SODR;
  volatile uint32_t* PIN_DB10_PIO_SODR;
  volatile uint32_t* PIN_DB11_PIO_SODR;
  volatile uint32_t* PIN_DB12_PIO_SODR;
  volatile uint32_t* PIN_DB13_PIO_SODR;
  volatile uint32_t* PIN_DB14_PIO_SODR;
  volatile uint32_t* PIN_DB15_PIO_SODR;


  uint32_t PIN_RESX_PIN_MASK;
  uint32_t PIN_CSX_PIN_MASK;
  uint32_t PIN_CX_PIN_MASK;
  uint32_t PIN_WRX_PIN_MASK;
  uint32_t PIN_RDX_PIN_MASK;

  uint32_t PIN_DB0_PIN_MASK;
  uint32_t PIN_DB1_PIN_MASK;
  uint32_t PIN_DB2_PIN_MASK;
  uint32_t PIN_DB3_PIN_MASK;
  uint32_t PIN_DB4_PIN_MASK;
  uint32_t PIN_DB5_PIN_MASK;
  uint32_t PIN_DB6_PIN_MASK;
  uint32_t PIN_DB7_PIN_MASK;
  uint32_t PIN_DB8_PIN_MASK;
  uint32_t PIN_DB9_PIN_MASK;
  uint32_t PIN_DB10_PIN_MASK;
  uint32_t PIN_DB11_PIN_MASK;
  uint32_t PIN_DB12_PIN_MASK;
  uint32_t PIN_DB13_PIN_MASK;
  uint32_t PIN_DB14_PIN_MASK;
  uint32_t PIN_DB15_PIN_MASK;
  // ORDER OF FIELDS EXTREMELY IMPORTANT
};
#elif ARDUINO_ARCH_STM32

#define OP_PIO_GPIO_START 0
#define OP_PIN_MASK_START 84
struct OptimizedPins
{
  // ORDER OF FIELDS EXTREMELY IMPORTANT
  volatile uint32_t* PIN_RESX_GPIO;
  volatile uint32_t* PIN_CSX_GPIO;
  volatile uint32_t* PIN_CX_GPIO;
  volatile uint32_t* PIN_WRX_GPIO;
  volatile uint32_t* PIN_RDX_GPIO;

  volatile uint32_t* PIN_DB0_GPIO;
  volatile uint32_t* PIN_DB1_GPIO;
  volatile uint32_t* PIN_DB2_GPIO;
  volatile uint32_t* PIN_DB3_GPIO;
  volatile uint32_t* PIN_DB4_GPIO;
  volatile uint32_t* PIN_DB5_GPIO;
  volatile uint32_t* PIN_DB6_GPIO;
  volatile uint32_t* PIN_DB7_GPIO;
  volatile uint32_t* PIN_DB8_GPIO;
  volatile uint32_t* PIN_DB9_GPIO;
  volatile uint32_t* PIN_DB10_GPIO;
  volatile uint32_t* PIN_DB11_GPIO;
  volatile uint32_t* PIN_DB12_GPIO;
  volatile uint32_t* PIN_DB13_GPIO;
  volatile uint32_t* PIN_DB14_GPIO;
  volatile uint32_t* PIN_DB15_GPIO;

  uint16_t PIN_RESX_PIN_MASK;
  uint16_t PIN_CSX_PIN_MASK;
  uint16_t PIN_CX_PIN_MASK;
  uint16_t PIN_WRX_PIN_MASK;
  uint16_t PIN_RDX_PIN_MASK;

  uint16_t PIN_DB0_PIN_MASK;
  uint16_t PIN_DB1_PIN_MASK;
  uint16_t PIN_DB2_PIN_MASK;
  uint16_t PIN_DB3_PIN_MASK;
  uint16_t PIN_DB4_PIN_MASK;
  uint16_t PIN_DB5_PIN_MASK;
  uint16_t PIN_DB6_PIN_MASK;
  uint16_t PIN_DB7_PIN_MASK;
  uint16_t PIN_DB8_PIN_MASK;
  uint16_t PIN_DB9_PIN_MASK;
  uint16_t PIN_DB10_PIN_MASK;
  uint16_t PIN_DB11_PIN_MASK;
  uint16_t PIN_DB12_PIN_MASK;
  uint16_t PIN_DB13_PIN_MASK;
  uint16_t PIN_DB14_PIN_MASK;
  uint16_t PIN_DB15_PIN_MASK;
  // ORDER OF FIELDS EXTREMELY IMPORTANT
};

#endif

// do whatever one time setup is needed (mcu specific)
void setupMCU();

void setPin(struct DisplayInterface* dInterface, uint8_t pin, uint8_t value);

uint8_t readPin(struct DisplayInterface* dInterface, uint8_t pin);

void setPinDirection(struct DisplayInterface* dInterface, uint8_t pin, uint8_t output);

void setDataBus8(struct DisplayInterface* dInterface, uint8_t value);

/**
* Fill out the ARCH specific optimization structure.
* @param dInterface - the display interface
* @param optPins - the optimized pins structure to be filled out;
* @param unsafe - if true unsafe (breaks compatiability) optimizations are used. These may require the display
                  to be hooked up to specific pins. The exact requirments are MCU specific
*/
void optimizePins(struct DisplayInterface* dInterface, struct OptimizedPins * optPins, bool unsafe);

#endif //LIBILI9486_MCU_H
