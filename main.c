/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
#include "test.h"
#include "shell.h"
#include "chprintf.h"
#include "usbcfg.h"

#define USB_GPIO_PORT GPIOA
#define USBDM_BIT 11
#define USBDP_BIT 12
void usb_lld_disconnect_bus(USBDriver *usbp)
{
	(void) usbp;
	palClearPort(USB_GPIO_PORT, (1<<USBDM_BIT) | (1<<USBDP_BIT));
	palSetPadMode(USB_GPIO_PORT, USBDM_BIT, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode(USB_GPIO_PORT, USBDP_BIT, PAL_MODE_OUTPUT_PUSHPULL);
}

void usb_lld_connect_bus(USBDriver *usbp)
{
	(void) usbp;
	palClearPort(USB_GPIO_PORT, (1<<USBDM_BIT) | (1<<USBDP_BIT));
	palSetPadMode(USB_GPIO_PORT, USBDM_BIT, PAL_MODE_INPUT);
	palSetPadMode(USB_GPIO_PORT, USBDM_BIT, PAL_MODE_INPUT);
}


/*
 * Red LED blinker thread, times are in milliseconds.
 */

uint16_t period = 500;
static THD_WORKING_AREA(waThread1, 128);
static  THD_FUNCTION(Thread1, arg) {

	(void)arg;
	chRegSetThreadName("blinker");
	while (TRUE) {
		palTogglePad(GPIOB, GPIOB_LED);
		chThdSleepMilliseconds(period);
	}
	return 0;
}

static THD_WORKING_AREA(waBtnThread, 128);
static THD_FUNCTION(BtnThread, arg) {
	(void) arg;
	chRegSetThreadName("btn");
	while (TRUE) {
		while (!palReadPad(GPIOB, GPIOB_BTN1)) {
			chThdSleepMilliseconds(1);
		}
		if (period <= 50) {
			period = 1000;
		} else {
			period = period/2;
		}
		chThdSleepMilliseconds(250);
	}
	return 0;
}

#define SHELL_WA_SIZE THD_WORKING_AREA_SIZE(1024)

static void cmd_test(BaseSequentialStream *chp, int argc, char *argv[]) {
	(void) argc;
	(void) argv;
	chprintf(chp, "ChibiOS test suite\r\n");
	TestThread(chp);
}

static void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[]) {
//	size_t n, size;
	(void) argv;
	(void) argc;

//	n = chHeapStatus(&size);
	chprintf(chp, "core free memory : %u bytes\r\n", chCoreStatus());
//	chprintf(chp, "heap fragments   : %u\r\n", n);
//	chprintf(chp, "heap free total  : %u bytes\r\n", size);
}

static void cmd_reboot(BaseSequentialStream *chp, int argc, char *argv[]) {
	(void) argc;
	(void) argv;
	chprintf(chp, "Rebooting...\r\n");
	chThdSleepMilliseconds(100);
	NVIC_SystemReset();
}

static void cmd_btn(BaseSequentialStream *chp, int argc, char *argv[]) {
	(void) argc;
	(void) argv;

	uint8_t i;

	for (i = 0; i < 10; i++) {
		chprintf(chp, "btn is %d\r\n", (uint8_t)palReadPad(GPIOB, GPIOB_BTN1));
		chThdSleepMilliseconds(250);
	}
}

static const ShellCommand shCmds[] = {
	{"test",      cmd_test},
	{"free", cmd_mem},
	{"reboot", cmd_reboot},
	{"btn", cmd_btn},
	{NULL, NULL}
};

SerialUSBDriver SDU1;

static const ShellConfig shCfg = {
	(BaseSequentialStream *)&SDU1,
	shCmds
};

static const I2CConfig i2c_config = {
    OPMODE_I2C,
    100000,
    STD_DUTY_CYCLE
};


/*
 * Application entry point.
 */
int main(void) {
	thread_t *sh = NULL;

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
	halInit();
	chSysInit();
	palSetPadMode(GPIOB, GPIOB_LED, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode(GPIOB, GPIOB_BTN1, PAL_MODE_INPUT_PULLDOWN);

	palSetPadMode(GPIOB, 5, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);
	palSetPadMode(GPIOB, 6, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);

	i2cStart(&I2CD1, &i2c_config);

	shellInit();
	usbDisconnectBus(serusbcfg.usbp);
	chThdSleepMilliseconds(500);
	usbConnectBus(serusbcfg.usbp);
  /*
   * Activates the serial driver 2 using the driver default configuration.
   */
	usbStart(serusbcfg.usbp, &usbcfg);
	sduObjectInit(&SDU1);
	sduStart(&SDU1, &serusbcfg);

  /*
   * Creates the blinker thread.
   */
	chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
	chThdCreateStatic(waBtnThread, sizeof(waBtnThread), NORMALPRIO, BtnThread, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
	while (TRUE) {
		if (!sh) {
			sh = shellCreate(&shCfg, SHELL_WA_SIZE, NORMALPRIO);
		} else if (chThdTerminatedX(sh)) {
			chThdRelease(sh);
			sh = NULL;
		}
		chThdSleepMilliseconds(500);
	}
}
