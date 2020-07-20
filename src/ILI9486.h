#ifndef ILI9486_H_
#define ILI9486_H_

#include <stdio.h>

#define LCD_INPUT_MODE_PARALLEL_8 0
#define LCD_INPUT_MODE_PARALLEL_16 1

#define DISPLAY_ORIENTATION_PORTRAIT 0
#define DISPLAY_ORIENTATION_LANDSCAPE 1

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 480

#ifdef ARDUINO_ARCH_AVR
//=============================== AVR ===================================
#define BANK_B 1
#define BANK_C 2
#define BANK_D 3

struct DisplayInterface
{
    // one of PARALLEL_8,  PARALLEL_16
    uint8_t inputMode;

    //display orientation one of, DISPLAY_ORIENTATION_PORTRAIT, DISPLAY_ORIENTATION_LANDSCAPE
    uint8_t orientation;

    // pins are tuples. BANK + PIN.
    // BANK is the IO, BANK, i.e. BANK_B.
    // PIN is the pin in that bank, i.e PORTB5.

    // reset pin
    uint8_t RESX_BANK;
    uint8_t RESX_PIN;

    // chip select
    uint8_t CSX_BANK;
    uint8_t CSX_PIN;

    // command select
    uint8_t CX_BANK;
    uint8_t CX_PIN;

    // write signal
    uint8_t WRX_BANK;
    uint8_t WRX_PIN;

    // read signal
    uint8_t RDX_BANK;
    uint8_t RDX_PIN;

    // parallel data input lines
    uint8_t DB0_BANK;
    uint8_t DB0_PIN;

    uint8_t DB1_BANK;
    uint8_t DB1_PIN;

    uint8_t DB2_BANK;
    uint8_t DB2_PIN;

    uint8_t DB3_BANK;
    uint8_t DB3_PIN;

    uint8_t DB4_BANK;
    uint8_t DB4_PIN;

    uint8_t DB5_BANK;
    uint8_t DB5_PIN;

    uint8_t DB6_BANK;
    uint8_t DB6_PIN;

    uint8_t DB7_BANK;
    uint8_t DB7_PIN;

    uint8_t DB8_BANK;
    uint8_t DB8_PIN;

    uint8_t DB9_BANK;
    uint8_t DB9_PIN;

    uint8_t DB10_BANK;
    uint8_t DB10_PIN;

    uint8_t DB11_BANK;
    uint8_t DB11_PIN;

    uint8_t DB12_BANK;
    uint8_t DB12_PIN;

    uint8_t DB13_BANK;
    uint8_t DB13_PIN;

    uint8_t DB14_BANK;
    uint8_t DB14_PIN;

    uint8_t DB15_BANK;
    uint8_t DB15_PIN;

    // ARCH specific optimization
    struct OptimizedPins * optimization;
};
#endif

/**
* sets defaults on the display interface. Probably a good idea to call this.
* @param dInterface - the display interface to be setup.
*/
void initializeDisplayInterface(struct DisplayInterface * dInterface);


/**
* Build display interface applying ARCH specific optimizations to the display interface. updating
* the interface after calling this WILL HAVE NO EFFECT unless you call this AGAIN.
* @param dInterface - the interface to optimize
*/
void buildDisplayInterface(struct DisplayInterface* dInterface);

/**
* destroy a display interface. MUST BE CALLED when you are done with the interface,
* or there will be a memory leak!
* @param dInterface - the display interface to destroy
*/
void destroyDisplayInterface(struct DisplayInterface * dInterface);

void initializeDisplay(struct DisplayInterface* dInterface);

uint8_t * getDisplayInformation(struct DisplayInterface* dInterface, uint8_t* responseBuffer);

uint8_t * getDisplayStatus(struct DisplayInterface* dInterface, uint8_t* responseBuffer);

uint8_t * getDisplayFormat(struct DisplayInterface* dInterface, uint8_t* responseBuffer);

void setSleepMode(struct DisplayInterface* dInterface, boolean sleep);

/**
* sets the orientation of the display
* @param dInterface - the display interface
* @param orientation - the orientation to set.
*/
void setDislayOrientation(struct DisplayInterface* dInterface, uint8_t orientation);

void writeDisplay(struct DisplayInterface* dInterface, uint16_t * data,
                  uint16_t x, uint16_t y, uint16_t width, uint16_t height);

void readDisplay(struct DisplayInterface* dInterface, uint16_t * data,
                  uint16_t x, uint16_t y, uint16_t width, uint16_t height);

void clearDisplay(struct DisplayInterface* dInterface, uint16_t clearColor);

#endif
