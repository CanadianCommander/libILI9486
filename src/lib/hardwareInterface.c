#include <stdio.h>
#include <Arduino.h>

#include "hardwareInterface.h"
#include "mcu.h"

//==================== STATIC FUNCTIONS ===================================
/**
* read the values of the bus pins.
* SET BUS DIRECTION TO INPUT BEFORE CALLING.
* @param dInterface - display interface
*/
static uint16_t readBus(struct DisplayInterface* dInterface)
{
  uint16_t output = 0x0000;

  // pull read pin low
  setPin(dInterface, PIN_RDX, 0);
  uint8_t count = 8;
  if (dInterface->inputMode == LCD_INPUT_MODE_PARALLEL_16)
  {
    count = 16;
  }

  for(uint8_t i = 0; i < count; i ++)
  {
    output |= (readPin(dInterface, PIN_DB0 + i) << i);
  }

  //reset read pin to high
  setPin(dInterface, PIN_RDX, 1);
  return output;
}

/**
* change parallel bus direction
* @param dInterface - display interface
* @param output - if 1 output 0 input.
*/
static void setBusDirection(struct DisplayInterface* dInterface, uint8_t output)
{
  uint8_t count = 8;
  if (dInterface->inputMode == LCD_INPUT_MODE_PARALLEL_16)
  {
    count = 16;
  }

  for(uint8_t i = 0; i < count; i ++)
  {
    setPinDirection(dInterface, PIN_DB0 + i, output);
  }
}

/**
* perform a write strobe. (clock the WRX pin)
* @param dInterface - the display interface
*/
static inline void writeStrobe(struct DisplayInterface* dInterface)
{
    setPin(dInterface, PIN_WRX, 0);
    setPin(dInterface, PIN_WRX, 1);
}

/**
* sets the pin values as required to execute the command
* @param dInterface - the display interface
* @param command - the command to execute
*/
static void setPinsForCommand(struct DisplayInterface* dInterface, uint8_t command)
{
  setBusDirection(dInterface, 1);

  setPin(dInterface, PIN_CX, 0);

  for(int i =0; i < 8; i++)
  {
    setPin(dInterface, PIN_DB0 + i, (command & (1 << i)) >> i);
  }
}

/**
* set the pins as required to write command parameter data to display.
* @param dInterface - the display interface.
* @param param - the parameter to write.
*/
static inline void setPinsForCommandParam(struct DisplayInterface* dInterface, uint8_t param)
{
  setDataBus8(dInterface, param);
}

/**
* configure pins for reading of command response from display
* @param dInterface - display interface
*/
static void setPinsForCommandResponse(struct DisplayInterface* dInterface)
{
  setBusDirection(dInterface, 0);
  setPin(dInterface, PIN_CX, 1);
}

static void setPixelsNatvie(struct DisplayInterface* dInterface, uint8_t * data,
              const uint16_t width, const uint16_t height, float scale)
{
  if (scale != 1.0)
  {
    uint16_t scaledWidth = width*scale;
    uint16_t scaledHeight = height*scale;

    for(uint16_t y = 0; y < scaledHeight; y ++)
    {
      for(uint16_t x = 0; x < scaledWidth; x ++)
      {
        uint16_t color = ((uint16_t*)data)[(int)((((int)(y/scale))*width) + (int)x/scale)];
        setPinsForCommandParam(dInterface, (uint8_t)(color >> 8));
        writeStrobe(dInterface);
        setPinsForCommandParam(dInterface, (uint8_t)color);
        writeStrobe(dInterface);
      }
    }
  }
  else
  {
    for(uint32_t i =0; i < width*height*2; i ++)
    {
      setPinsForCommandParam(dInterface, data[i]);
      writeStrobe(dInterface);
    }
  }
}

static void setPixels1bpp(struct DisplayInterface* dInterface, uint8_t * data,
              uint16_t width, uint16_t height, float scale, uint16_t primaryColor, uint16_t secondaryColor)
{
  uint16_t color = 0x0000;
  uint32_t dataIndex = 0;
  uint8_t byteIndex = 7;
  uint32_t pixelIndex = 0;

  if (scale != 1.0)
  {
    uint16_t scaledWidth = width*scale;
    uint16_t scaledHeight = height*scale;

    for(uint16_t y = 0; y < scaledHeight; y ++)
    {
      for(uint16_t x = 0; x < scaledWidth; x ++)
      {
        int selectedPixel = (int)((((int)(y/scale))*width) + (int)x/scale);
        dataIndex = selectedPixel/8;
        byteIndex = 7 - (selectedPixel % 8);

        color = secondaryColor;
        if (data[dataIndex] & (1 << byteIndex))
        {
            color = primaryColor;
        }
        setPinsForCommandParam(dInterface, (uint8_t)(color >> 8));
        writeStrobe(dInterface);
        setPinsForCommandParam(dInterface, (uint8_t)color);
        writeStrobe(dInterface);
      }
    }
  }
  else
  {
    while(pixelIndex < width*height)
    {
      color = secondaryColor;
      if (data[dataIndex] & (1 << byteIndex))
      {
          color = primaryColor;
      }
      setPinsForCommandParam(dInterface, (uint8_t)(color >> 8));
      writeStrobe(dInterface);
      setPinsForCommandParam(dInterface, (uint8_t)color);
      writeStrobe(dInterface);

      pixelIndex++;
      if ( byteIndex <= 0)
      {
        byteIndex = 7;
        dataIndex ++;
      }
      else
      {
        byteIndex--;
      }
    }
  }
}

/////////////////////////// STATIC FUNCTIONS /////////////////////////////////

/**
* reset the display
* @param dInterface - the display interface
*/
void resetDisplay(struct DisplayInterface* dInterface)
{
  sendCommand(dInterface, COMMAND_SOFT_RESET, NULL, 0, NULL, COMMAND_SOFT_RESET_RESPONSE_ROWS, COMMAND_SOFT_RESET_DEAD_ROWS);
  // delay 5msec as specified in ILI9486L documentation
  delay(5);
}


/**
* prepare the display for use.
* @param dInterface - display interface
*/
void configureDisplay(struct DisplayInterface* dInterface)
{
  // do MCU specific setup
  setupMCU();

  // select chip
  setPinDirection(dInterface, PIN_CSX, 1);
  setPin(dInterface, PIN_CSX, 0);

  // Read and Write high
  setPinDirection(dInterface, PIN_RDX, 1);
  setPinDirection(dInterface, PIN_WRX, 1);
  setPin(dInterface, PIN_RDX, 1);
  setPin(dInterface, PIN_WRX, 1);

  // command select
  setPinDirection(dInterface, PIN_CX, 1);
  setPin(dInterface, PIN_CX, 1);

  // Reset display hardware pin
  setPinDirection(dInterface, PIN_RESX, 1);
  setPin(dInterface, PIN_RESX, 0);
  setPin(dInterface, PIN_RESX, 1);

  // Reset display through software
  resetDisplay(dInterface);

  // wake chip
  setSleepMode(dInterface, false);

  // configure 16bpp color
  uint8_t pixelFormat = DEFAULT_PIXEL_FORMAT;
  sendCommand(dInterface, COMMAND_SET_PIXEL_FORMAT, &pixelFormat, COMMAND_SET_PIXEL_FORMAT_PARAMS, NULL,
              COMMAND_SET_PIXEL_FORMAT_RESPONSE_ROWS, COMMAND_SET_PIXEL_FORMAT_DEAD_ROWS);

  // turn on display
  sendCommand(dInterface, COMMAND_DISPLAY_ON, NULL, 0, NULL, COMMAND_DISPLAY_ON_RESPONSE_ROWS, COMMAND_DISPLAY_ON_DEAD_ROWS);

  // wait for display to "start up". We get some missed pixels if we don't.
  delay(100);
}

/**
 * Sends a command to the display. Depending on the command, the response will be returned.
 * @param dInterface - the display interface
 * @param command - the command code to execute
 * @param params - an array of command specific parameter data
 * @param countParams - the number of params in the params array
 * @param responseBuffer - a buffer big enought to hold the response
 * @param responseRows - the number of response rows to expect
 * @param deadRows - the number of "dead" rows before real response data is returned
 * @return - any data returned by the display in response to the command
 */
uint8_t * sendCommand(struct DisplayInterface* dInterface, uint8_t command,
                      uint8_t* params, uint32_t countParams,
                      uint8_t* responseBuffer, uint8_t responseRows, uint8_t deadRows)
{
  // send command
  setPinsForCommand(dInterface, command);
  writeStrobe(dInterface);

  // prep for param write.
  setBusDirection(dInterface, 1);
  setPin(dInterface, PIN_CX, 1);

  // send parameters
  for (uint32_t i = 0; i < countParams; i ++)
  {
    setPinsForCommandParam(dInterface, params[i]);
    writeStrobe(dInterface);
  }

  if (responseRows)
  {
    // read response
    setPinsForCommandResponse(dInterface);

    for(uint8_t i = 0; i < deadRows; i ++)
    {
      // read through the "invalid" data.
      readBus(dInterface);
    }

    for(uint8_t i = 0; i < responseRows; i ++)
    {
      responseBuffer[i] = (uint8_t)readBus(dInterface);
    }
  }
  return responseBuffer;
}


/**
* set pixels on the display. This is an alternate method to calling sendCommand. This method is faster
* when processing non standard bit depths
* @param dInterface - the display interface
* @param data - the pixel data
* @param width - pixel data width
* @param height - pixel data height
* @prarm scale - scalling to apply to the data
* @param bitDepth - the bit depth of the pixel data
* @param primaryColor - the primary color for use in 1bpp mode
* @param secondaryColor - the secondary color for use in 1bpp mode
*/
void setPixels(struct DisplayInterface* dInterface, uint8_t * data,
              uint16_t width, uint16_t height, float scale,
              uint8_t bitDepth, uint16_t primaryColor, uint16_t secondaryColor)
{
  // send write command
  setPinsForCommand(dInterface, COMMAND_WRITE_DISPLAY);
  writeStrobe(dInterface);

  // prep for param write.
  setBusDirection(dInterface, 1);
  setPin(dInterface, PIN_CX, 1);

  if (bitDepth == BIT_DEPTH_NATIVE)
  {
    setPixelsNatvie(dInterface, data, width, height, scale);
  }
  else if (bitDepth == BIT_DEPTH_1BPP)
  {
    setPixels1bpp(dInterface, data, width, height, scale, primaryColor, secondaryColor);
  }
}

/**
* sets the drawable region of the display
* @param dInterface - the display interface
* @param x - the top left x position of the region
* @param y - the top left y position of the region
* @param width - the width of the region
* @param height - the height of the region (going down from top left)
*/
void setDrawRegion(struct DisplayInterface* dInterface, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
  //set x clip
  uint16_t xEnd = x + width -1;
  uint8_t paramsX[] = {(uint8_t)(x >> 8), (uint8_t)x, (uint8_t)(xEnd >> 8), (uint8_t)xEnd};
  sendCommand(dInterface, COMMAND_SET_COLUMN_ADDRESS, paramsX, COMMAND_SET_COLUMN_ADDRESS_PARAMS, NULL,
              COMMAND_SET_COLUMN_ADDRESS_RESPONSE_ROWS, COMMAND_SET_COLUMN_ADDRESS_DEAD_ROWS);

  //set y clip
  uint16_t yEnd = y + height - 1;
  uint8_t paramsY[] = {(uint8_t)(y >> 8), (uint8_t)y, (uint8_t)(yEnd >> 8), (uint8_t)yEnd};
  sendCommand(dInterface, COMMAND_SET_PAGE_ADDRESS, paramsY, COMMAND_SET_PAGE_ADDRESS_PARAMS, NULL,
              COMMAND_SET_PAGE_ADDRESS_RESPONSE_ROWS, COMMAND_SET_PAGE_ADDRESS_DEAD_ROWS);
}

/**
* fills a rectangle with the specified color. note specifiy rectangle with setDrawRegion
* @param dInterface - the display interface
* @param color - the color to fill with
* @param pixelCount - the number of pixels to fill
*/
void fillRectangle(struct DisplayInterface* dInterface, uint16_t color, uint32_t pixelCount)
{
  // send write command
  setPinsForCommand(dInterface, COMMAND_WRITE_DISPLAY);
  writeStrobe(dInterface);

  // prep for param write.
  setBusDirection(dInterface, 1);
  setPin(dInterface, PIN_CX, 1);

  // send rect color
  for (uint32_t i = 0; i < pixelCount; i ++)
  {
    setPinsForCommandParam(dInterface, (uint8_t)(color >> 8));
    writeStrobe(dInterface);
    setPinsForCommandParam(dInterface, (uint8_t)color);
    writeStrobe(dInterface);
  }
}
