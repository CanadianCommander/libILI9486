#include "Arduino.h"

#include "ILI9486.h"
#include "lib/hardwareInterface.h"
#include "lib/mcu.h"

/**
* sets defaults on the display interface. Probably a good idea to call this.
* @param dInterface - the display interface to be setup.
*/
void initializeDisplayInterface(struct DisplayInterface * dInterface)
{
  memset(dInterface, 0x00, sizeof(struct DisplayInterface));
  dInterface->orientation = DISPLAY_ORIENTATION_PORTRAIT;
}
/**
* Build display interface applying ARCH specific optimizations to the display interface. updating
* the interface after calling this WILL HAVE NO EFFECT unless you call this AGAIN.
* @param dInterface - the interface to optimize
* @param unsafe - if true unsafe optimizations will be applied. These are MCU specifc and may require
                  the display to be connected to certain pins (HUGE PERFORMANCE BOOST FOR AVR).
*/
void buildDisplayInterface(struct DisplayInterface* dInterface, bool unsafe)
{
  optimizePins(dInterface, (struct OptimizedPins*)malloc(sizeof(struct OptimizedPins)), unsafe);
}


/**
* destroy a display interface. MUST BE CALLED when you are done with the interface,
* or there will be a memory leak!
* @param dInterface - the display interface to destroy
*/
void destroyDisplayInterface(struct DisplayInterface * dInterface)
{
  if (dInterface->optimization)
  {
    free(dInterface->optimization);
  }
}

void initializeDisplay(struct DisplayInterface* dInterface)
{
  configureDisplay(dInterface);
}/**
* destroy a display interface. MUST BE CALLED if optimization is used,
* or there will be a memory leak!
* @param dInterface - the display interface to destroy
*/
void destroyDisplayInterface(struct DisplayInterface * dInterface);

uint8_t * getDisplayInformation(struct DisplayInterface* dInterface, uint8_t* responseBuffer)
{
  return sendCommand(dInterface, COMMAND_READ_DISPLAY_INFORMATION,
                     NULL, 0,
                     responseBuffer,
                     COMMAND_READ_DISPLAY_INFORMATION_RESPONSE_ROWS,
                     COMMAND_READ_DISPLAY_INFORMATION_DEAD_ROWS);
}

uint8_t * getDisplayStatus(struct DisplayInterface* dInterface, uint8_t* responseBuffer)
{
  return sendCommand(dInterface, COMMAND_READ_DISPLAY_STATUS,
                     NULL, 0,
                     responseBuffer,
                     COMMAND_READ_DISPLAY_STATUS_RESPONSE_ROWS,
                     COMMAND_READ_DISPLAY_STATUS_DEAD_ROWS);
}

uint8_t * getDisplayFormat(struct DisplayInterface* dInterface, uint8_t* responseBuffer)
{
  return sendCommand(dInterface, COMMAND_READ_DISPLAY_FORMAT,
                     NULL, 0,
                     responseBuffer,
                     COMMAND_READ_DISPLAY_FORMAT_RESPONSE_ROWS,
                     COMMAND_READ_DISPLAY_FORMAT_DEAD_ROWS);
}

void setSleepMode(struct DisplayInterface* dInterface, boolean sleep)
{
  uint8_t command = 0x0;
  if (sleep)
  {
    command = COMMAND_SLEEP;
  }
  else
  {
    command = COMMAND_WAKE;
  }

  sendCommand(dInterface, command, NULL, 0, NULL, 0, 0);
  // wait 5ms as instructed in manual
  delay(5);
}

/**
* sets the orientation of the display
* @param dInterface - the display interface
* @param orientation - the orientation to set.
*/
void setDisplayOrientation(struct DisplayInterface* dInterface, uint8_t orientation)
{
  uint8_t param = (orientation << 5);
  sendCommand(dInterface, COMMAND_SET_MADCTL, &param , COMMAND_SET_MADCTL_PARAMS,
              NULL, COMMAND_SET_MADCTL_RESPONSE_ROWS, COMMAND_SET_MADCTL_DEAD_ROWS);

  dInterface->orientation = orientation;
}

/**
* set the brightness of the display
* @param dInterface - the display interface
* @param brightness - brightness 0x00 (dim) - 0xFF (bright)
*/
void setDisplayBrightness(struct DisplayInterface* dInterface, uint8_t brightness)
{
  uint8_t param = COMMAND_SET_CTRL_BRIGHTNESS_CONTROLL_ON;
  sendCommand(dInterface, COMMAND_SET_CTRL, &param , COMMAND_SET_CTRL_PARAMS,
              NULL, COMMAND_SET_CTRL_RESPONSE_ROWS, COMMAND_SET_CTRL_DEAD_ROWS);

  sendCommand(dInterface, COMMAND_SET_DISPLAY_BRIGHTNESS, &brightness, COMMAND_SET_DISPLAY_BRIGHTNESS_PARAMS,
              NULL, COMMAND_SET_DISPLAY_BRIGHTNESS_RESPONSE_ROWS, COMMAND_SET_DISPLAY_BRIGHTNESS_DEAD_ROWS);
}

void writeDisplay(struct DisplayInterface* dInterface, uint16_t * data,
                  uint16_t x, uint16_t y, uint16_t width, uint16_t height, float scale)
{
  // set draw area
  setDrawRegion(dInterface, x, y, width*scale, height*scale);

  // write pixel data
  setPixels(dInterface, (uint8_t*)data, width, height, scale, BIT_DEPTH_NATIVE, 0, 0);
}

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
                  uint16_t width, uint16_t height, float scale)
{
  // set draw area
  setDrawRegion(dInterface, x, y, width*scale, height*scale);

  // write pixel data
  setPixels(dInterface, (uint8_t*)data, width, height, scale, BIT_DEPTH_1BPP, color, altColor);
}

void readDisplay(struct DisplayInterface* dInterface, uint16_t * data,
                  uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
  // set draw area
  setDrawRegion(dInterface, x, y, width, height);

  // read pixel data
  sendCommand(dInterface, COMMAND_READ_DISPLAY, NULL, 0, (uint8_t*)data,
                (width * height) * 2, COMMAND_READ_DISPLAY_DEAD_ROWS);
}

void clearDisplay(struct DisplayInterface* dInterface, uint16_t clearColor)
{
  uint16_t width = DISPLAY_WIDTH;
  uint16_t height = DISPLAY_HEIGHT;

  if (dInterface->orientation == DISPLAY_ORIENTATION_LANDSCAPE)
  {
    width = DISPLAY_HEIGHT;
    height = DISPLAY_WIDTH;
  }

  // set draw area
  setDrawRegion(dInterface, 0, 0, width, height);

  //fill
  fillRectangle(dInterface, clearColor, (uint32_t)width*(uint32_t)height);
}

/**
* convert a mask style pin to a pin offset. This type of pin definition is found
* on the sam3x8e aka the Arduino Due
* @param pinMask - the mask style pin to convert
* @return a pin offset
*/
uint8_t getPinOffset(uint32_t pinMask)
{
  for (uint8_t i = 0; i < 32; i ++)
  {
    if (!(pinMask >> (i+1)))
    {
      return i;
    }
  }
  return 0;
}
