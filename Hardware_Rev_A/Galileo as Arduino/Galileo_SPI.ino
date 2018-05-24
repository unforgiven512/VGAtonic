#include <SPI.h>

#include "Guitars.h"
#include "Doge3.h"



void setup() {
	digitalWrite(SS, HIGH);		// ensure SS stays high for now

	// Put SCK, MOSI, SS pins into output mode
	// also put SCK, MOSI into LOW state, and SS into HIGH state.
	// Then put SPI hardware into Master mode and turn SPI on
	SPI.begin();

	// Slow down the master a bit
	SPI.setClockDivider(SPI_CLOCK_DIV8);
}


void loop() {
	/* Show one picture for 10 seconds */
	showPicture(doge3);
	delay(10000);

	/* Show another picture for 10 seconds */
	showPicture(guitars);
	delay(10000);
}


void showPicture(const uint8_t *arrayRef) {
	// send test string
	delay(1);
	digitalWrite(SS, LOW);

	for (uint16_t row = 0; row <= 480; row++) {
		byte myBytes[640];
		uint32_t rowMultiplier = 640 * row;

		for (uint16_t column = 0; column <= 640; column++) {
			myBytes[column] = arrayRef[rowMultiplier + column];
		}

		SPI.transferBuffer(myBytes, NULL, 640);
	}

	digitalWrite(SS, HIGH);
}
