#include <Arduino.h>
#include <stdio.h>

#include "mcu.h"


#ifdef ARDUINO_ARCH_AVR
//=================== ARCH AVR =========================
#include "avr/io.h"

volatile uint8_t * getPORT(uint8_t bank)
{
  switch(bank)
  {
    case BANK_B:
      return &PORTB;
    case BANK_C:
      return &PORTC;
    case BANK_D:
      return &PORTD;
    default:
      return NULL;
  }
}

volatile uint8_t * getDDRx(uint8_t bank)
{
  switch(bank)
  {
    case BANK_B:
      return &DDRB;
    case BANK_C:
      return &DDRC;
    case BANK_D:
      return &DDRD;
    default:
      return NULL;
  }
}

volatile uint8_t * getPINx(uint8_t bank)
{
  switch(bank)
  {
    case BANK_B:
      return &PINB;
    case BANK_C:
      return &PINC;
    case BANK_D:
      return &PIND;
    default:
      return NULL;
  }
}

// do whatever one time setup is needed (mcu specific)
void setupMCU()
{
  // no setup need for AVR
}

#define AVR_CASE_SET_PIN(PIN_NAME) case PIN_##PIN_NAME: \
  *getPORT(dInterface->PIN_NAME##_BANK) = (*getPORT(dInterface->PIN_NAME##_BANK) & ~(1 << dInterface->PIN_NAME##_PIN)) | (value << dInterface->PIN_NAME##_PIN); \
  break;

// this functions interface must be generic enought to work with multiple different architectures!
inline void setPin(struct DisplayInterface* dInterface, uint8_t pin, uint8_t value)
{
  volatile uint8_t* reg = (uint8_t*) *(((uint16_t*)dInterface->optimization) + OP_PORT_START + pin);
  uint8_t pinOffset = *(((uint8_t*)dInterface->optimization) + OP_PIN_OFFSET_START + pin);
  *reg = (*reg & ~(1 << pinOffset)) | (value << pinOffset);
}


inline void setDataBus8(struct DisplayInterface* dInterface, uint8_t value)
{
  if (dInterface->optimization->unsafeOptimizations)
  {
    PORTB = (PORTB & ~dInterface->optimization->PIXEL_BANK_MASK_B) | (dInterface->optimization->PIXEL_BANK_MASK_B & value);
    PORTD = (PORTD & ~dInterface->optimization->PIXEL_BANK_MASK_D) | (dInterface->optimization->PIXEL_BANK_MASK_D & value);
  }
  else
  {
    setPin(dInterface, PIN_DB0, (value >> 0) & 0x1);
    setPin(dInterface, PIN_DB1, (value >> 1) & 0x1);
    setPin(dInterface, PIN_DB2, (value >> 2) & 0x1);
    setPin(dInterface, PIN_DB3, (value >> 3) & 0x1);
    setPin(dInterface, PIN_DB4, (value >> 4) & 0x1);
    setPin(dInterface, PIN_DB5, (value >> 5) & 0x1);
    setPin(dInterface, PIN_DB6, (value >> 6) & 0x1);
    setPin(dInterface, PIN_DB7, (value >> 7) & 0x1);
  }
}

#define AVR_CASE_READ_PIN(PIN_NAME) case PIN_##PIN_NAME: \
  return 0x01 & (*getPINx(dInterface->PIN_NAME##_BANK) >> dInterface->PIN_NAME##_PIN);

// read pin value
uint8_t readPin(struct DisplayInterface* dInterface, uint8_t pin)
{
  switch(pin)
  {
    AVR_CASE_READ_PIN(RESX);
    AVR_CASE_READ_PIN(CSX);
    AVR_CASE_READ_PIN(CX);
    AVR_CASE_READ_PIN(WRX);
    AVR_CASE_READ_PIN(RDX);
    AVR_CASE_READ_PIN(DB0);
    AVR_CASE_READ_PIN(DB1);
    AVR_CASE_READ_PIN(DB2);
    AVR_CASE_READ_PIN(DB3);
    AVR_CASE_READ_PIN(DB4);
    AVR_CASE_READ_PIN(DB5);
    AVR_CASE_READ_PIN(DB6);
    AVR_CASE_READ_PIN(DB7);
    AVR_CASE_READ_PIN(DB8);
    AVR_CASE_READ_PIN(DB9);
    AVR_CASE_READ_PIN(DB10);
    AVR_CASE_READ_PIN(DB11);
    AVR_CASE_READ_PIN(DB12);
    AVR_CASE_READ_PIN(DB13);
    AVR_CASE_READ_PIN(DB14);
    AVR_CASE_READ_PIN(DB15);
  }
  return 0;
}

#define AVR_CASE_SET_PIN_DIRECTION(PIN_NAME) case PIN_##PIN_NAME: \
  *getDDRx(dInterface->PIN_NAME##_BANK) = (*getDDRx(dInterface->PIN_NAME##_BANK) & ~(1 << dInterface->PIN_NAME##_PIN)) | (output << dInterface->PIN_NAME##_PIN); \
  break;

/**
* set the direction of a pin
* @param dInterface - the display interface
* @param pin - the pin to set direction for
* @param output - if 1 output 0 input
*/
void setPinDirection(struct DisplayInterface* dInterface, uint8_t pin, uint8_t output)
{
  volatile uint8_t* reg = (uint8_t*) *(((uint16_t*)dInterface->optimization) + (OP_DDR_START/2) + pin);
  uint8_t pinOffset = *(((uint8_t*)dInterface->optimization) + OP_PIN_OFFSET_START + pin);
  *reg = (*reg & ~(1 << pinOffset)) | (output << pinOffset);
}

#define AVR_SET_OPTIMIZED_PORTX(PIN_NAME) optPins->PIN_##PIN_NAME##_PORT = getPORT(dInterface->PIN_NAME##_BANK)
#define AVR_SET_OPTIMIZED_DDRX(PIN_NAME) optPins->PIN_##PIN_NAME##_DDR = getDDRx(dInterface->PIN_NAME##_BANK)
#define AVR_SET_OPTIMIZED_PIN_OFFSET(PIN_NAME) optPins->PIN_##PIN_NAME##_PIN_OFFSET = dInterface->PIN_NAME##_PIN


/**
* Fill out the ARCH specific optimization structure.
* @param dInterface - the display interface
* @param optPins - the optimized pins structure to be filled out;
* @param unsafe - if true unsafe (breaks compatiability) optimizations are used. These may require the display
                  to be hooked up to specific pins. The exact requirments are MCU specific
*/
void optimizePins(struct DisplayInterface* dInterface, struct OptimizedPins * optPins, bool unsafe)
{
  memset(optPins, 0x00, sizeof(struct OptimizedPins));

  AVR_SET_OPTIMIZED_PORTX(RESX);
  AVR_SET_OPTIMIZED_PORTX(CSX);
  AVR_SET_OPTIMIZED_PORTX(CX);
  AVR_SET_OPTIMIZED_PORTX(WRX);
  AVR_SET_OPTIMIZED_PORTX(RDX);
  AVR_SET_OPTIMIZED_PORTX(DB0);
  AVR_SET_OPTIMIZED_PORTX(DB1);
  AVR_SET_OPTIMIZED_PORTX(DB2);
  AVR_SET_OPTIMIZED_PORTX(DB3);
  AVR_SET_OPTIMIZED_PORTX(DB4);
  AVR_SET_OPTIMIZED_PORTX(DB5);
  AVR_SET_OPTIMIZED_PORTX(DB6);
  AVR_SET_OPTIMIZED_PORTX(DB7);
  AVR_SET_OPTIMIZED_PORTX(DB8);
  AVR_SET_OPTIMIZED_PORTX(DB9);
  AVR_SET_OPTIMIZED_PORTX(DB10);
  AVR_SET_OPTIMIZED_PORTX(DB11);
  AVR_SET_OPTIMIZED_PORTX(DB12);
  AVR_SET_OPTIMIZED_PORTX(DB13);
  AVR_SET_OPTIMIZED_PORTX(DB14);
  AVR_SET_OPTIMIZED_PORTX(DB15);

  AVR_SET_OPTIMIZED_DDRX(RESX);
  AVR_SET_OPTIMIZED_DDRX(CSX);
  AVR_SET_OPTIMIZED_DDRX(CX);
  AVR_SET_OPTIMIZED_DDRX(WRX);
  AVR_SET_OPTIMIZED_DDRX(RDX);
  AVR_SET_OPTIMIZED_DDRX(DB0);
  AVR_SET_OPTIMIZED_DDRX(DB1);
  AVR_SET_OPTIMIZED_DDRX(DB2);
  AVR_SET_OPTIMIZED_DDRX(DB3);
  AVR_SET_OPTIMIZED_DDRX(DB4);
  AVR_SET_OPTIMIZED_DDRX(DB5);
  AVR_SET_OPTIMIZED_DDRX(DB6);
  AVR_SET_OPTIMIZED_DDRX(DB7);
  AVR_SET_OPTIMIZED_DDRX(DB8);
  AVR_SET_OPTIMIZED_DDRX(DB9);
  AVR_SET_OPTIMIZED_DDRX(DB10);
  AVR_SET_OPTIMIZED_DDRX(DB11);
  AVR_SET_OPTIMIZED_DDRX(DB12);
  AVR_SET_OPTIMIZED_DDRX(DB13);
  AVR_SET_OPTIMIZED_DDRX(DB14);
  AVR_SET_OPTIMIZED_DDRX(DB15);

  AVR_SET_OPTIMIZED_PIN_OFFSET(RESX);
  AVR_SET_OPTIMIZED_PIN_OFFSET(CSX);
  AVR_SET_OPTIMIZED_PIN_OFFSET(CX);
  AVR_SET_OPTIMIZED_PIN_OFFSET(WRX);
  AVR_SET_OPTIMIZED_PIN_OFFSET(RDX);
  AVR_SET_OPTIMIZED_PIN_OFFSET(DB0);
  AVR_SET_OPTIMIZED_PIN_OFFSET(DB1);
  AVR_SET_OPTIMIZED_PIN_OFFSET(DB2);
  AVR_SET_OPTIMIZED_PIN_OFFSET(DB3);
  AVR_SET_OPTIMIZED_PIN_OFFSET(DB4);
  AVR_SET_OPTIMIZED_PIN_OFFSET(DB5);
  AVR_SET_OPTIMIZED_PIN_OFFSET(DB6);
  AVR_SET_OPTIMIZED_PIN_OFFSET(DB7);
  AVR_SET_OPTIMIZED_PIN_OFFSET(DB8);
  AVR_SET_OPTIMIZED_PIN_OFFSET(DB9);
  AVR_SET_OPTIMIZED_PIN_OFFSET(DB10);
  AVR_SET_OPTIMIZED_PIN_OFFSET(DB11);
  AVR_SET_OPTIMIZED_PIN_OFFSET(DB12);
  AVR_SET_OPTIMIZED_PIN_OFFSET(DB13);
  AVR_SET_OPTIMIZED_PIN_OFFSET(DB14);
  AVR_SET_OPTIMIZED_PIN_OFFSET(DB15);

  if (unsafe)
  {
    optPins->unsafeOptimizations = true;
    memset(&optPins->PIXEL_BANK_MASK_B, 0x00, 1);
    memset(&optPins->PIXEL_BANK_MASK_C, 0x00, 1);
    memset(&optPins->PIXEL_BANK_MASK_D, 0x00, 1);
    for(uint8_t i =0; i < 16; i ++)
    {
      if((uint8_t*) *(((uint16_t*)optPins) + OP_PORT_START + PIN_DB0 + i) == &PORTB)
      {
        optPins->PIXEL_BANK_MASK_B |= (1 << i);
      }
      else if((uint8_t*) *(((uint16_t*)optPins) + OP_PORT_START + PIN_DB0 + i) == &PORTC)
      {
        optPins->PIXEL_BANK_MASK_C |= (1 << i);
      }
      else if((uint8_t*) *(((uint16_t*)optPins) + OP_PORT_START + PIN_DB0 + i) == &PORTD)
      {
        optPins->PIXEL_BANK_MASK_D |= (1 << i);
      }
    }
  }

  dInterface->optimization = optPins;
}

// END #ifdef ARDUINO_ARCH_AVR
#elif ARDUINO_ARCH_SAM
//=================== ARCH SAM =========================
#include "sam3x8e.h"

// Register offsets from REG_PIO_PER
#define REG_PIO_PER 0
#define REG_PIO_PDR 1
#define REG_PIO_PSR 2
#define REG_PIO_OER 4
#define REG_PIO_ODR 5
#define REG_PIO_OSR 6
//...
#define REG_PIO_SODR 12
#define REG_PIO_CODR 13
//...
#define REG_PIO_PDSR 15
//...
#define REG_PIO_PUDR 24
#define REG_PIO_PUER 25
//...
#define REG_PIO_OWER 40
#define REG_PIO_OWDR 41

// get a pointer to a register form the specified bank
static uint32_t * lookupRegister(uint8_t bank, uint8_t reg)
{
  switch(bank)
  {
    case BANK_A:
      return (uint32_t *) (&REG_PIOA_PER + reg);

    case BANK_B:
      return (uint32_t *) (&REG_PIOB_PER + reg);

    case BANK_C:
      return (uint32_t *) (&REG_PIOC_PER + reg);

    case BANK_D:
      return (uint32_t *) (&REG_PIOD_PER + reg);

    default:
      return NULL;
  }
}

static void setPullup(uint8_t bank, uint8_t pinOffset, bool enabled)
{
  uint32_t * reg;
  if(enabled)
  {
    reg = lookupRegister(bank,REG_PIO_PUER);
  }
  else
  {
    reg = lookupRegister(bank,REG_PIO_PUDR);
  }
  *reg |= (1 << pinOffset);
}


// do whatever one time setup is needed (mcu specific)
void setupMCU()
{
  //enable all IO banks found on the Arduino Due
  REG_PMC_PCER0 |= (1 << ID_PIOA) | (1 << ID_PIOB) | (1 << ID_PIOC) | (1 << ID_PIOD);
}

// set pin value
inline void setPin(struct DisplayInterface* dInterface, uint8_t pin, uint8_t value)
{
  *(((uint32_t*) *((uint32_t*)dInterface->optimization + pin)) + (value ^ 0x1)) |= *(((uint32_t*)dInterface->optimization) + OP_PIN_MASK_START/4 + pin);
}

inline void setDataBus8(struct DisplayInterface* dInterface, uint8_t value)
{
  setPin(dInterface, PIN_DB0, (value >> 0) & 0x1);
  setPin(dInterface, PIN_DB1, (value >> 1) & 0x1);
  setPin(dInterface, PIN_DB2, (value >> 2) & 0x1);
  setPin(dInterface, PIN_DB3, (value >> 3) & 0x1);
  setPin(dInterface, PIN_DB4, (value >> 4) & 0x1);
  setPin(dInterface, PIN_DB5, (value >> 5) & 0x1);
  setPin(dInterface, PIN_DB6, (value >> 6) & 0x1);
  setPin(dInterface, PIN_DB7, (value >> 7) & 0x1);
}

#define SAM_READ_PIN_CASE(PIN_NAME) case PIN_##PIN_NAME: \
  return (*lookupRegister(dInterface->PIN_NAME##_BANK, REG_PIO_PDSR) & (1 << dInterface->PIN_NAME##_PIN)) >> dInterface->PIN_NAME##_PIN;

// read pin value
inline uint8_t readPin(struct DisplayInterface* dInterface, uint8_t pin)
{
  switch(pin)
  {
    SAM_READ_PIN_CASE(RESX);
    SAM_READ_PIN_CASE(CSX);
    SAM_READ_PIN_CASE(CX);
    SAM_READ_PIN_CASE(WRX);
    SAM_READ_PIN_CASE(RDX);
    SAM_READ_PIN_CASE(DB0);
    SAM_READ_PIN_CASE(DB1);
    SAM_READ_PIN_CASE(DB2);
    SAM_READ_PIN_CASE(DB3);
    SAM_READ_PIN_CASE(DB4);
    SAM_READ_PIN_CASE(DB5);
    SAM_READ_PIN_CASE(DB6);
    SAM_READ_PIN_CASE(DB7);
    SAM_READ_PIN_CASE(DB8);
    SAM_READ_PIN_CASE(DB9);
    SAM_READ_PIN_CASE(DB10);
    SAM_READ_PIN_CASE(DB11);
    SAM_READ_PIN_CASE(DB12);
    SAM_READ_PIN_CASE(DB13);
    SAM_READ_PIN_CASE(DB14);
    SAM_READ_PIN_CASE(DB15);
  }
  return 0;
}

#define SAM_SET_PIN_DIRECTION_CASE(PIN_NAME) case PIN_##PIN_NAME: \
  _setPinDirection(dInterface->PIN_NAME##_BANK, dInterface->PIN_NAME##_PIN, output); \
  break;

static inline void _setPinDirection(uint8_t bank, uint8_t pinOffset, uint8_t output)
{
  if(output)
  {
    *lookupRegister(bank, REG_PIO_OER) |= (1 << pinOffset);
    *lookupRegister(bank, REG_PIO_OWER) |= (1 << pinOffset);
    setPullup(bank,pinOffset,false);
  }
  else
  {
    *lookupRegister(bank, REG_PIO_ODR) |= (1 << pinOffset);
    *lookupRegister(bank, REG_PIO_OWDR) |= (1 << pinOffset);
    setPullup(bank,pinOffset,true);
  }
}

/**
* set the direction of a pin
* @param dInterface - the display interface
* @param pin - the pin to set direction for
* @param output - if 1 output 0 input
*/
inline void setPinDirection(struct DisplayInterface* dInterface, uint8_t pin, uint8_t output)
{
  switch(pin)
  {
    SAM_SET_PIN_DIRECTION_CASE(RESX);
    SAM_SET_PIN_DIRECTION_CASE(CSX);
    SAM_SET_PIN_DIRECTION_CASE(CX);
    SAM_SET_PIN_DIRECTION_CASE(WRX);
    SAM_SET_PIN_DIRECTION_CASE(RDX);
    SAM_SET_PIN_DIRECTION_CASE(DB0);
    SAM_SET_PIN_DIRECTION_CASE(DB1);
    SAM_SET_PIN_DIRECTION_CASE(DB2);
    SAM_SET_PIN_DIRECTION_CASE(DB3);
    SAM_SET_PIN_DIRECTION_CASE(DB4);
    SAM_SET_PIN_DIRECTION_CASE(DB5);
    SAM_SET_PIN_DIRECTION_CASE(DB6);
    SAM_SET_PIN_DIRECTION_CASE(DB7);
    SAM_SET_PIN_DIRECTION_CASE(DB8);
    SAM_SET_PIN_DIRECTION_CASE(DB9);
    SAM_SET_PIN_DIRECTION_CASE(DB10);
    SAM_SET_PIN_DIRECTION_CASE(DB11);
    SAM_SET_PIN_DIRECTION_CASE(DB12);
    SAM_SET_PIN_DIRECTION_CASE(DB13);
    SAM_SET_PIN_DIRECTION_CASE(DB14);
    SAM_SET_PIN_DIRECTION_CASE(DB15);
  }
}

#define SAM_ENABLE_PIN(PIN_NAME) *lookupRegister(dInterface->PIN_NAME##_BANK, REG_PIO_PER) = (1 << dInterface->PIN_NAME##_PIN)
#define SAM_SET_OPTIMIZED_SODR_REG(PIN_NAME) optPins->PIN_##PIN_NAME##_PIO_SODR = lookupRegister(dInterface->PIN_NAME##_BANK, REG_PIO_SODR)
#define SAM_SET_OPTIMIZED_PIN_MASK(PIN_NAME) optPins->PIN_##PIN_NAME##_PIN_MASK = (1 << dInterface->PIN_NAME##_PIN)
/**
* Fill out the ARCH specific optimization structure.
* @param dInterface - the display interface
* @param optPins - the optimized pins structure to be filled out;
* @param unsafe - if true unsafe (breaks compatiability) optimizations are used. These may require the display
                  to be hooked up to specific pins. The exact requirments are MCU specific
*/
void optimizePins(struct DisplayInterface* dInterface, struct OptimizedPins * optPins, bool unsafe)
{
  SAM_ENABLE_PIN(RESX);
  SAM_ENABLE_PIN(CSX);
  SAM_ENABLE_PIN(CX);
  SAM_ENABLE_PIN(WRX);
  SAM_ENABLE_PIN(RDX);
  SAM_ENABLE_PIN(DB0);
  SAM_ENABLE_PIN(DB1);
  SAM_ENABLE_PIN(DB2);
  SAM_ENABLE_PIN(DB3);
  SAM_ENABLE_PIN(DB4);
  SAM_ENABLE_PIN(DB5);
  SAM_ENABLE_PIN(DB6);
  SAM_ENABLE_PIN(DB7);
  SAM_ENABLE_PIN(DB8);
  SAM_ENABLE_PIN(DB9);
  SAM_ENABLE_PIN(DB10);
  SAM_ENABLE_PIN(DB11);
  SAM_ENABLE_PIN(DB12);
  SAM_ENABLE_PIN(DB13);
  SAM_ENABLE_PIN(DB14);
  SAM_ENABLE_PIN(DB15);

  SAM_SET_OPTIMIZED_SODR_REG(RESX);
  SAM_SET_OPTIMIZED_SODR_REG(CSX);
  SAM_SET_OPTIMIZED_SODR_REG(CX);
  SAM_SET_OPTIMIZED_SODR_REG(WRX);
  SAM_SET_OPTIMIZED_SODR_REG(RDX);
  SAM_SET_OPTIMIZED_SODR_REG(DB0);
  SAM_SET_OPTIMIZED_SODR_REG(DB1);
  SAM_SET_OPTIMIZED_SODR_REG(DB2);
  SAM_SET_OPTIMIZED_SODR_REG(DB3);
  SAM_SET_OPTIMIZED_SODR_REG(DB4);
  SAM_SET_OPTIMIZED_SODR_REG(DB5);
  SAM_SET_OPTIMIZED_SODR_REG(DB6);
  SAM_SET_OPTIMIZED_SODR_REG(DB7);
  SAM_SET_OPTIMIZED_SODR_REG(DB8);
  SAM_SET_OPTIMIZED_SODR_REG(DB9);
  SAM_SET_OPTIMIZED_SODR_REG(DB10);
  SAM_SET_OPTIMIZED_SODR_REG(DB11);
  SAM_SET_OPTIMIZED_SODR_REG(DB12);
  SAM_SET_OPTIMIZED_SODR_REG(DB13);
  SAM_SET_OPTIMIZED_SODR_REG(DB14);
  SAM_SET_OPTIMIZED_SODR_REG(DB15);

  SAM_SET_OPTIMIZED_PIN_MASK(RESX);
  SAM_SET_OPTIMIZED_PIN_MASK(CSX);
  SAM_SET_OPTIMIZED_PIN_MASK(CX);
  SAM_SET_OPTIMIZED_PIN_MASK(WRX);
  SAM_SET_OPTIMIZED_PIN_MASK(RDX);
  SAM_SET_OPTIMIZED_PIN_MASK(DB0);
  SAM_SET_OPTIMIZED_PIN_MASK(DB1);
  SAM_SET_OPTIMIZED_PIN_MASK(DB2);
  SAM_SET_OPTIMIZED_PIN_MASK(DB3);
  SAM_SET_OPTIMIZED_PIN_MASK(DB4);
  SAM_SET_OPTIMIZED_PIN_MASK(DB5);
  SAM_SET_OPTIMIZED_PIN_MASK(DB6);
  SAM_SET_OPTIMIZED_PIN_MASK(DB7);
  SAM_SET_OPTIMIZED_PIN_MASK(DB8);
  SAM_SET_OPTIMIZED_PIN_MASK(DB9);
  SAM_SET_OPTIMIZED_PIN_MASK(DB10);
  SAM_SET_OPTIMIZED_PIN_MASK(DB11);
  SAM_SET_OPTIMIZED_PIN_MASK(DB12);
  SAM_SET_OPTIMIZED_PIN_MASK(DB13);
  SAM_SET_OPTIMIZED_PIN_MASK(DB14);
  SAM_SET_OPTIMIZED_PIN_MASK(DB15);

  dInterface->optimization = optPins;
}

#endif
