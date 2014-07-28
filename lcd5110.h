/*
    Copyright (C) 2013 Marcin Jokel

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

/**
 * @file    lcd5110.h
 * @brief   Nokia 5110 LCD interface module through SPI code.
 *
 * @addtogroup lcd5110
 * @{
 */

#ifndef _LCD5110_H_
#define _LCD5110_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define LCD5110_X_RES                  84
#define LCD5110_Y_RES                  48

#define LCD5110_FONT_X_SIZE             5
#define LCD5110_FONT_Y_SIZE             8

#define LCD5110_SEND_CMD                0
#define LCD5110_SEND_DATA               1

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#include "hw.h" // it's a dirty job, but someone gotta do it

#if !defined(LCD5110_RES_PIN)
#error "LCD5110_RES_PIN not defined!!!"
#endif

#if !defined(LCD5110_RES_PORT)
#error "LCD5110_RES_PORT not defined!!!"
#endif

#if !defined(LCD5110_DC_PIN)
#error "LCD5110_DC_PIN not defined!!!"
#endif

#if!defined(LCD5110_DC_PORT)
#error "LCD5110_DC_PORT not defined!!!"
#endif

#if!defined(LCD5110_SPID)
#error "LCD5110_SPID undefined!"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void lcd5110InitI(SPIDriver *spip);
  void lcd5110WriteByteI(SPIDriver *spip, uint8_t data, uint8_t cd);
  void lcd5110ContrastI(SPIDriver *spip, uint8_t contrast);
  void lcd5110ClearI(SPIDriver *spip);
  void lcd5110SetPosXYI(SPIDriver *spip, uint8_t x, uint8_t y);
  void lcd5110WriteCharI(SPIDriver *spip, uint8_t ch);
  void lcd5110WriteTextI(SPIDriver *spip, const char * strp);
  void lcd5110RotateTextI(SPIDriver *spip, const uint8_t * strp, uint8_t offset);

#define lcd5110Init() lcd5110InitI(&LCD5110_SPID)
#define lcd5110WriteByte(...) lcd5110WriteByteI(&LCD5110_SPID, __VA_ARGS__)
#define lcd5110Contrast(...) lcd5110ContrastI(&LCD5110_SPID, __VA_ARGS__)
#define lcd5110Clear() lcd5110ClearI(&LCD5110_SPID)
#define lcd5110SetPosXY(...) lcd5110SetPosXYI(&LCD5110_SPID, __VA_ARGS__)
#define lcd51120WriteChar(...) lcd5110WriteCharI(&LCD5110_SPID, __VA_ARGS__)
#define lcd5110WriteText(...) lcd5110WriteTextI(&LCD5110_SPID, __VA_ARGS__)
#define lcd5110RotateText(...) lcd5110RotateTextI(&LCD5110_SPID, __VA_ARGS__)
#ifdef __cplusplus
}
#endif

#endif /* _LCD5110_H_ */

/** @} */
