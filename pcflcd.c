#include "ch.h"
#include "hal.h"
#include "pcflcd.h"

msg_t pcflcd_init() {
	i2cAcquireBus(&PCF_I2CD);
	uint8_t txbuff[4] = {0x03<<4, 0x03<<4, 0x03<<4, 0x02<<4};
	uint16_t delays[4] = {4500, 4100, 4100, 150};
	uint8_t i;
	for (i = 0; i < 4; i++) {
		msg_t status = i2cMasterTransmitTimeout(&PCF_I2CD, DEFAULT_ADDRESS, &txbuff[i], 1, NULL, 0, MS2ST(1000));
		if (status != MSG_OK) {
			i2cReleaseBus(&PCF_I2CD);
			return status*i;
		}
		chThdSleepMicroseconds(delays[i]);
	}
	i2cReleaseBus(&PCF_I2CD);
	return MSG_OK;
}

msg_t pcflcd_write_char(const char chr) {
	i2cAcquireBus(&PCF_I2CD);
	uint8_t highnib = ((uint8_t)chr)&0xf0;
	uint8_t lownib = (((uint8_t)chr) << 4)&0xf0;

	uint8_t txbuff[2] = {(highnib)|0x1, (lownib)|0x1};
	uint8_t i;
	
	for (i = 0; i < 2; i++) {
		msg_t status = i2cMasterTransmitTimeout(&PCF_I2CD, DEFAULT_ADDRESS, &txbuff[i], 1, NULL, 0, MS2ST(1000));
		if (status != MSG_OK) {
			i2cReleaseBus(&PCF_I2CD);
			return status*i;
		}
	}

	i2cReleaseBus(&PCF_I2CD);
	return MSG_OK;
}
