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

/**
* setup the dispay putting it in to the working state.
* @param dInterface - display interface (MUST CALL buildDisplayInterface() on it first)
*/
void initializeDisplay(struct DisplayInterface* dInterface);

/**
* get basic information about the display. see ILI9486 documentation for exact format
* @param dInterface - the display interface
* @param responseBuffer - a buffer of 3 bytes. to be populated by the display
*/
uint8_t * getDisplayInformation(struct DisplayInterface* dInterface, uint8_t* responseBuffer);

/**
* get the current status of the display. see ILI9486 documentation for exact format
* @param dInterface - the display interface
* @param responseBuffer - a buffer of 4 bytes. to be populated by the display
*/
uint8_t * getDisplayStatus(struct DisplayInterface* dInterface, uint8_t* responseBuffer);

/**
* get the current display format. 16bpp 18bpp
* @param dInterface - the display interface
* @param responseBuffer - a buffer of 1 bytes. to be populated by the display
*/
uint8_t * getDisplayFormat(struct DisplayInterface* dInterface, uint8_t* responseBuffer);

/**
* sleep or wake the display
* @param dInterface - the display interface
* @param sleep - true put the display to sleep. false wake the display up.
*/
void setSleepMode(struct DisplayInterface* dInterface, boolean sleep);

/**
* sets the orientation of the display
* @param dInterface - the display interface
* @param orientation - the orientation to set.
*/
void setDislayOrientation(struct DisplayInterface* dInterface, uint8_t orientation);

/**
* write data to the display
* @param dInterface - the display interface
* @param data - the data to write
* @param x - the x position to write the data at.
* @param y - the y position to write the data at.
* @param width - the width of the data
* @param height - the height of the data
* @param scale - scaling to apply to the data
*/
void writeDisplay(struct DisplayInterface* dInterface, uint16_t * data,
                  uint16_t x, uint16_t y, uint16_t width, uint16_t height, float scale);

/**
* Similar to, writeDisplay(). But takes data in a 1bpp format. 1 == color 0 == altColor
* This allows for greater memory efficency.
* @param dInterface - the display interface
* @param data - the data to write (1bpp)
* @param color - the primary color (1)
* @param altColor - the secondary color (0)
* @param x - the x position to write the data at.
* @param y - the y position to write the data at.
* @param width - the width of the data
* @param height - the height of the data
* @param scale - scaling of the image
*/
void writeDisplay1bpp(struct DisplayInterface* dInterface, uint8_t * data,
                  uint16_t color, uint16_t altColor, uint16_t x, uint16_t y,
                  uint16_t width, uint16_t height, float scale);

/**
* just like writeDisplay() but reads display data in to the provided data buffer.
* @param dInterface - the display interface
* @param data - the data to write
* @param x - the x position to read the data from.
* @param y - the y position to read the data from.
* @param width - the width of the data
* @param height - the height of the data
*/
void readDisplay(struct DisplayInterface* dInterface, uint16_t * data,
                  uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/**
* clear the display with a solid color
* @param dInterface - the display interface
* @param clearColor - the color to clear the display to.
*/
void clearDisplay(struct DisplayInterface* dInterface, uint16_t clearColor);

#endif
