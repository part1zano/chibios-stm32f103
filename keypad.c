#include "ch.h"
#include "hal.h"
#include "keypad.h"

const uint8_t cols[NCOLS] = {1, 2, 3};     // XXX :: gpio pins
const uint8_t rows[NROWS] = {4, 5, 6, 7};  // XXX :: gpio pins

void prepareKeypad(void) {
	uint8_t i;
	for (i - 0; i < NCOLS; i++) {
		palSetPad(COL_GPIO_PORT, cols[i]);
	}
}

uint8_t getKeyPressed(void) {
	uint8_t i, j;
	for (i = 0; i < NCOLS; i++) {
		palClearPad(ROW_GPIO_PORT, cols[i]);
		for (j = 0; j < NROWS; j++) {
			if (!palReadPad(COL_GPIO_PORT, rows[j])) {
				return (i*j);
			}
		}
		palSetPad(ROW_GPIO_PORT, cols[i]);
	}
	return 0xff;
}


