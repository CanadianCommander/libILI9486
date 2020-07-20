#include "Arduino.h"
#include "avr/io.h"

#include "ILI9486.h"
#include "hardwareInterface.h"
#include "mcu.h"

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
*/
void buildDisplayInterface(struct DisplayInterface* dInterface)
{
  optimizePins(dInterface, (struct OptimizedPins*)malloc(sizeof(struct OptimizedPins)));
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
void setDislayOrientation(struct DisplayInterface* dInterface, uint8_t orientation)
{
  uint8_t param = (orientation << 5);
  sendCommand(dInterface, COMMAND_SET_MADCTL, &param , COMMAND_SET_MADCTL_PARAMS,
              NULL, COMMAND_SET_MADCTL_RESPONSE_ROWS, COMMAND_SET_MADCTL_DEAD_ROWS);

  dInterface->orientation = orientation;
}

void writeDisplay(struct DisplayInterface* dInterface, uint16_t * data,
                  uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
  // set draw area
  setDrawRegion(dInterface, x, y, width, height);

  // write pixel data
  sendCommand(dInterface, COMMAND_WRITE_DISPLAY, (uint8_t*)data, (width * height) * 2, NULL,
                COMMAND_WRITE_DISPLAY_RESPONSE_ROWS, COMMAND_WRITE_DISPLAY_DEAD_ROWS);
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
