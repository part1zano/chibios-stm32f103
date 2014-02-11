#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#define NCOLS 3
#define NROWS 4
#define ROW_GPIO_PORT GPIOA
#define COL_GPIO_PORT GPIOA


void prepareKeypad(void);
uint8_t getKeyPressed(void);
#endif // __KEYPAD_H__
