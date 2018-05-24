/**
 * \file utilities.h
 * Contains definitions, strctures, and enumerations related to the \c utilities.ino file
 *
 * \author		Gerad Munsch <gmunsch@unforgivendevelopment.com>
 * \date		2018-05-05
 * \version		0.1.0
 */

#ifndef _UTILITIES_H__
#define _UTILITIES_H__


/* =====[ DEFINITIONS ]============================================================================================== */

/*! Set the following value to \b 1 if it is desired to update the device \b EEPROM with new data */
#define EEPROM_UPDATE 0

#define BITBANG_SPI_FOR_SETUP_LTC 0	/*!< I don't think we need this... */


/* =====[ TYPE DEFINITIONS, DATA STRUCTURES & ENUMERATIONS ]========================================================= */

/**
 * Type definition enumerating the valid VGA modes (resolution and color depth combinations) available for use.
 */
typedef enum {
	MODE_640_X_480_AT_8BPP	= 0x00,		/*!< Defines a resolution of 640x480 at 8 bits-per-pixel of color */
	MODE_640_X_480_AT_4BPP	= 0x04,		/*!< Defines a resolution of 640x480 at 4 bits-per-pixel of color */
	MODE_640_X_480_AT_2BPP	= 0x08,		/*!< Defines a resolution of 640x480 at 2 bits-per-pixel of color */
	MODE_640_X_480_AT_1BPP	= 0x0C,		/*!< Defines a resolution of 640x480 at 1 bit-per-pixel of color */
	MODE_320_X_240_AT_8BPP	= 0x01,		/*!< Defines a resolution of 320x240 at 8 bits-per-pixel of color */
	MODE_320_X_240_AT_4BPP	= 0x05,		/*!< Defines a resolution of 320x240 at 4 bits-per-pixel of color */
	MODE_320_X_240_AT_2BPP	= 0x09,		/*!< Defines a resolution of 320x240 at 2 bits-per-pixel of color */
	MODE_320_X_240_AT_1BPP	= 0x0D,		/*!< Defines a resolution of 320x240 at 1 bit-per-pixel of color */
	MODE_160_X_120_AT_8BPP	= 0x02,		/*!< Defines a resolution of 160x120 at 8 bits-per-pixel of color */
	MODE_160_X_120_AT_4BPP	= 0x06,		/*!< Defines a resolution of 160x120 at 4 bits-per-pixel of color */
	MODE_160_X_120_AT_2BPP	= 0x0A,		/*!< Defines a resolution of 160x120 at 2 bits-per-pixel of color */
	MODE_160_X_120_AT_1BPP	= 0x0E,		/*!< Defines a resolution of 160x120 at 1 bit-per-pixel of color */
	MODE_80_X_60_AT_8BPP	= 0x03,		/*!< Defines a resolution of 80x60 at 8 bits-per-pixel of color */
	MODE_80_X_60_AT_4BPP	= 0x07,		/*!< Defines a resolution of 80x60 at 4 bits-per-pixel of color */
	MODE_80_X_60_AT_2BPP	= 0x0B,		/*!< Defines a resolution of 80x60 at 2 bits-per-pixel of color */
	MODE_80_X_60_AT_1BPP	= 0x0F		/*!< Defines a resolution of 80x60 at 1 bit-per-pixel of color */
} vga_display_mode_t;



/* =====[ GLOBAL VARIABLES ]========================================================================================= */



/* =====[ FUNCTION PROTOTYPES ]====================================================================================== */

#if (EEPROM_UPDATE == 1)
/**
 * Update the data in the EEPROM of the AVR microcontroller.
 *
 * This code calibrates the \c OSCCAL value, writes the "welcome string", and adds the font(s) to the EEPROM.
 *
 * \note The welcome string may be comprised of \b 18 "real" characters, terminated by an '\0' character.
 *
 * \attention This code should only be executed when writing to the \b EEPROM for the \e first time.
 */
void updateEEPROM();
#endif	/* (EEPROM_UPDATE == 1) */


/**
 * Print a color line to the screen using the default (current) background color.
 */
void inline printColorLine();

/**
 * Print multiple lines to the screen.
 *
 * \todo Clarify documentation.
 *
 * \param[in]	pos		The starting position for the printing of lines
 * \param[in]	posLast	The last position for the printing of lines
 */
void inline printLinesToScreen(uint8_t pos, uint8_t posLast);

/**
 * Print a line to the screen, containing a string of text, with the option to show the cursor.
 *
 * \note The line will be printed starting at the current position.
 *
 * \param[in]	c			The string to print, in the form of a character buffer
 * \param[in]	showCursor	A boolean value indicating whether to show the cursor (or not)
 */
void inline printLineToScreen(const char* c, boolean showCursor);

/**
 * Print a line to the screen, containing a string of text.
 *
 * \note The line will be printed starting at the current position, and the cursor will be shown or hidden based on the
 *       most recently stored value.
 *
 * \param[in]	c	The string to print, in the form of a character buffer
 */
void inline printLineToScreen(const char* c);

/**
 * Print a line to the screen at the specified position, containing a string of text, optionally showing the cursor.
 *
 * \param[in]	c			The string to print, in the form of a character buffer
 * \param[in]	pos			The position at which to print the line
 * \param[in]	showCursor	A boolean value indicating whether to show the cursor (or not)
 */
void printLineToScreen(const char* c, uint8_t pos, boolean showCursor);


/**
 * Insert multiple instances of a character in a range.
 *
 * \attention	It is probably wise to stop at index \b 38 -- otherwise, it may think the line is full, as the length of
 *				a line is \b 40 characters.
 *
 * \param[in]	insertChar	The character to be inserted throughout the range
 * \param[in]	posStart	The position index at which to start inserting the specified character
 * \param[in]	posEnd		The position index at which to stop inserting the specified character
 */
void inline insCharRange(const char insertChar, uint8_t posStart, uint8_t posEnd);

/**
 * Insert characters at a position
 *
 * \param[in]	insertChar	The character value to insert
 * \param[in]	pos			The position within the current character buffer at which point the character should be put
 */
void inline insChar(const char insertChar, uint8_t pos);

/**
 * Change the video mode by issuing a command to the CPLD.
 *
 * \note	This method is apparently unused, but it is retained for reference.
 *
 * \param[in]	width	The desired screen width, in pixels
 * \param[in]	height	The desired screen height, in pixels
 * \param[in]	depth	The desired color depth, in bits-per-pixel
 */
void changeMode(int width, int height, int depth);

/**
 * Send a control signal/command to the VGAtonic firmware on the CPLD.
 *
 * \param[in]	mByte	The control signal to send to the CPLD, in the form of an unsigned, 8-bit integer.
 */
void sendControlSignalToVGATonic(uint8_t mByte);

/**
 * Reverse the order of the bits in a byte.
 *
 * \param[in]	x	An unsigned, 8-bit integer (ie: a byte) to be bit-flipped
 *
 * \return The value returned will be an unsigned, 8-bit integer with the bit-order reversed.
 */
uint8_t reverseByte(uint8_t x);


#if (BITBANG_SPI_FOR_SETUP_LTC == 1)
void inline setup_ltc(uint8_t inputByte);
#endif	/* (BITBANG_SPI_FOR_SETUP_LTC == 1) */

#endif	/* !_UTILITIES_H__ */
