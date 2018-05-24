/*****************************************

          Utility Functions

******************************************/


#include "utilities.h"



#if (EEPROM_UPDATE == 1)
/*
 * Code here should only be executed when you are writing to EEPROM the >first< time.
 * Here we calibrate OSCCAL, write the welcome string, and add fonts to EEPROM.
 * The welcome string may be up to 18 real characters and an '\0'
 */
void updateEEPROM() {
	uint8_t hello[14] = "VGATonic v1.25";

	eeprom_update_byte((uint8_t *)OSCALSPEED, OSCCAL);
	eeprom_update_byte((uint8_t *)LTCSPEED, 0b01011110);
	eeprom_update_block((const void*)&hello, (void *)HELLOMSG, 14);

//	int k = 0;

	for (uint8_t x = 0; x < 95; ++x) {
//		for (int y = 0; y < 2; y++) {
		eeprom_update_byte((uint8_t *)FONTSTORAGE + x, pgm_read_byte(&font4x6[x][0]));
//			++k;
//		}
	}

}
#endif	/* (EEPROM_UPDATE == 1) */


void inline printColorLine() {
	for (uint8_t x = 0; x < 160; ++x) {
		spi_transfer(CURRENT_BACKGROUND_COLOR);
	}
}


void inline printLinesToScreen(uint8_t pos, uint8_t posLast) {
	if (pos == 0) {
		CPLD_HIGH;
		CPLD_LOW;
		printColorLine();
	}

	while (pos < posLast) {
		printLineToScreen("", pos, false);
		pos += 8;
	}
}


void inline printLineToScreen(const char* c, boolean showCursor) {
	printLineToScreen(c, CURRENT_LINE, showCursor);
}


void inline printLineToScreen(const char* c) {
	printLineToScreen(c, CURRENT_LINE, SHOW_CURSOR);
}


void printLineToScreen(const char* c, uint8_t pos, boolean showCursor) {
	// Move command
	//if (pos != 0)
	sendControlSignalToVGATonic(0b10000000 | pos);
	CPLD_LOW;

	uint8_t line_count = 0;
	uint8_t tempColor = CURRENT_BACKGROUND_COLOR;
	const char* start = c;

	while (line_count < 8) {
		uint8_t pixels_printed = 0;

		while (*c && (line_count < 7)) {
			uint8_t pixel = getFontLine(*c++, line_count - 1);

			for (uint8_t j = 0; j < 4; ++j) {
				// AND the pixel with 0b00000011 and subtract that from 3 so we count from 3->0 and loop.  It's due to the pixel decoding in the CPLD
				(pixel >> ( 0x03 - (pixels_printed&0x03)) ) & 0x01 == 1 ? spi_transfer(CURRENT_FOREGROUND_COLOR) : spi_transfer(CURRENT_BACKGROUND_COLOR);
				pixels_printed++;
			}
		}

		while (pixels_printed < 160) {
//			tempColor = CURRENT_BACKGROUND_COLOR;

			/*
			 * This hack saves ~40 bytes from writing out the whole >= and <=.
			 * Thanks, int division for letting this be true 4 times in a row.
			 */
			if ((showCursor == true) && (line_count == 7)) {
				if (CURRENT_CURSOR == (int)(pixels_printed / 4)) {
					tempColor = CURRENT_FOREGROUND_COLOR;
				}
			} else {
				spi_transfer(CURRENT_BACKGROUND_COLOR);
			}

			++pixels_printed;
		}

		++line_count;
		c = start;
	}
}


void inline insCharRange(const char insertChar, uint8_t posStart, uint8_t posEnd) {
	/* Sanity check */
	if (posStart >= posEnd) {
		/*! \warning The start position index must be less than the end position index. */
		return;
	}

	uint8_t currentIndex = posStart;	/*! Hclds the current position index; initalized with \p posStart */

	/* Insert the character(s), filling all positions from posStart to posEnd */
	do {
		insChar(insertChar, currentIndex++);
	} while (currentIndex <= posEnd);
}


void inline insChar(const char insertChar, uint8_t pos) {
	if (strlen(CURRENT_BUFFER) <= pos) {
		strncat(CURRENT_BUFFER, &insertChar, 1);
	} else if (strlen(CURRENT_BUFFER) > pos) {
		 CURRENT_BUFFER[pos] = insertChar;
	}
}


void changeMode(int width, int height, int depth) {
	uint8_t mByte = 0b00000011;		/*! The "mode" byte; default value is: 80x60 */
	uint8_t depthByte = 0b00001100;	/*! The "depth" byte; default value is: black & white */

	/* Calculate the "mode" byte based upon the passed resolution value */
	if (width == 640) {
		mByte = 0b00000000;
	} else if (width == 320) {
		mByte = 0b00000001;
	} else if (width == 160) {
		mByte = 0b00000010;
	}

	/* Calculate the "depth" byte based on the passed depth value */
	if (depth == 8) {
		depthByte = 0b00000000;
	} else if (depth == 4) {
		depthByte = 0b00000100;
	} else if (depth == 2) {
		depthByte = 0b00001000;
	}

	/* Perform a bitwise OR of the "depth" value onto the "mode" value to calculate the proper "mode" byte */
	mByte |= depthByte;

	/* Transfer our newly-transformed "mode" byte over the UART */
	uart_transmit(mByte);

	/* And send out "mode" byte to the CPLD */
	sendControlSignalToVGATonic(mByte);
}


void changeDisplayMode(vga_display_mode_t displayMode) {
	switch (displayMode) {
		case MODE_640_X_480_AT_8BPP
	}
}


void sendControlSignalToVGATonic(uint8_t mByte) {
	CPLD_HIGH;
	CPLD_LOW;
//	delay(0.01);
	spi_transfer(mByte);
//	delay(0.01);
	CPLD_HIGH;
}


uint8_t reverseByte(uint8_t x) {
	x = (((x >> 1) & 0x55) | ((x << 1) & 0xAA));
	x = (((x >> 2) & 0x33) | ((x << 2) & 0xCC));
	x = (((x >> 4) & 0x0F) | ((x << 4) & 0xF0));

	return (x);
}


#if (BITBANG_SPI_FOR_SETUP_LTC == 1)
// Bitbang SPI for LTC603
// DAC = 663, OCT = 15.  Can't change it yet.
void inline setup_ltc(uint8_t inputByte) {
	DDRB |= _BV(PB1);	// as output (D10, CPLD Master)
	DDRB |= _BV(PB0);	// as output (D9, CPLD Select)
	DDRB |= _BV(PB4);	// as output (D13, CLK Select)
	DDRB |= _BV(PB7);	// as output (USISCK)
	DDRB |= _BV(PB5);	// as output (DI, temp for clock)

	CLK_HIGH;
	CLK_LOW;

	bitbang_byte_out(0b11111010);
	bitbang_byte_out(inputByte);

	CLK_HIGH;

	// Turn on regular SPI
	DDRB |= _BV(PB6);	// as output (DO)
	DDRB &= ~_BV(PB5);	// as input (DI)
	PORTB |= _BV(PB5);	// pullup on (DI)
}
#endif	/* (BITBANG_SPI_FOR_SETUP_LTC == 1) */
