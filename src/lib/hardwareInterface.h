#ifndef LIBILI9486_HARDWAREINTERFACE_H
#define LIBILI9486_HARDWAREINTERFACE_H

#include <stdio.h>
#include "ILI9486.h"
#include "ILI9486Commands.h"

#define DEFAULT_PIXEL_FORMAT 0x55


#define BIT_DEPTH_NATIVE 0
#define BIT_DEPTH_1BPP 1

/**
* reset the display
* @param dInterface - the display interface
*/
void resetDisplay(struct DisplayInterface* dInterface);

/**
* prepare the display for use.
* @param dInterface - display interface
*/
void configureDisplay(struct DisplayInterface* dInterface);

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
                      uint8_t* responseBuffer, uint8_t responseRows, uint8_t deadRows);


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
              uint8_t bitDepth, uint16_t primaryColor, uint16_t secondaryColor);

/**
* sets the drawable region of the display
* @param dInterface - the display interface
* @param x - the top left x position of the region
* @param y - the top left y position of the region
* @param width - the width of the region
* @param height - the height of the region (going down from top left)
*/
void setDrawRegion(struct DisplayInterface* dInterface, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/**
* fills a rectangle with the specified color. note specifiy rectangle with setDrawRegion
* @param dInterface - the display interface
* @param color - the color to fill with
* @param pixelCount - the number of pixels to fill
*/
void fillRectangle(struct DisplayInterface* dInterface, uint16_t color, uint32_t pixelCount);

#endif //LIBILI9486_HARDWAREINTERFACE_H
