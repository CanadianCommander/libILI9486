#include <Arduino.h>
#include <stdio.h>
#include "avr/io.h"

#include "mcu.h"


#ifdef ARDUINO_ARCH_AVR

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


inline void setPixelBus8(struct DisplayInterface* dInterface, uint8_t value)
{
  PORTB = (PORTB & ~dInterface->optimization->PIXEL_BANK_MASK_B) | (dInterface->optimization->PIXEL_BANK_MASK_B & value);
  PORTC = (PORTC & ~dInterface->optimization->PIXEL_BANK_MASK_C) | (dInterface->optimization->PIXEL_BANK_MASK_C & value);
  PORTD = (PORTD & ~dInterface->optimization->PIXEL_BANK_MASK_D) | (dInterface->optimization->PIXEL_BANK_MASK_D & value);
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
* fill out the provided optimizedPins structure. The display interfaces optimized
* attribute is set to the ptr optPins.
* @param dInterface - the display interface
* @param optPins - the optimized pins structure to be filled out;
*/
void optimizePins(struct DisplayInterface* dInterface, struct OptimizedPins * optPins)
{
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

  dInterface->optimization = optPins;
}

// END #ifdef ARDUINO_ARCH_AVR
#endif
