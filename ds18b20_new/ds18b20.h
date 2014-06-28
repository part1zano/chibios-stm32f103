#ifndef __DS18B20
#define __DS18B20

#include "ch.h"
#include "hal.h"

#define DS18B20_CONVERT_T_CMD				0x44
#define DS18B20_WRITE_STRATCHPAD_CMD		0x4E
#define DS18B20_READ_STRATCHPAD_CMD			0xBE
#define DS18B20_COPY_STRATCHPAD_CMD			0x48
#define DS18B20_RECALL_E_CMD				0xB8
#define DS18B20_SEARCH_ROM_CMD				0xF0
#define DS18B20_READ_POWER_SUPPLY_CMD		0xB4
#define DS18B20_SKIPROM_CMD					0xCC
#define DS18B20_MATCHROM_CMD				0x55

#define MAX_SENSORS 						5

#define DS18B20_GPIO						GPIOC
#define DS18B20_DATAPIN					    0



void DS18B20_Init(void);
void DS18B20_StartConversion(uint8_t SensorNum);
void DS18B20_GetROM(uint8_t SensorNum, uint8_t *buffer);

uint8_t DS18B20_ScanBus(void);

float DS18B20_GetTemp(uint8_t SensorNum);

#endif
