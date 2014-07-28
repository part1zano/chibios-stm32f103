/* 
    USB-HID Gamepad for ChibiOS/RT
    Copyright (C) 2014, +inf Wenzheng Xu.
    
    EMAIL: wx330@nyu.edu
    
    This piece of code is FREE SOFTWARE and is released under the terms
    of the GNU General Public License <http://www.gnu.org/licenses/>
*/

/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "ch.h"
#include "hal.h"
#include "usb_hid.h"


/* CHECK IT!!! */
hid_data hid_in_data={0,0,0};
hid_data hid_out_data={0,0,0};
uint8_t usbInitState=0;
//-----------------------------


void hid_recive(USBDriver *usbp) {
	usbPrepareReceive(usbp, HID_OUT_EP_ADDRESS, (uint8_t *)&hid_out_data, sizeof (hid_out_data));
	chSysLockFromISR();
	usbStartReceiveI(usbp, HID_OUT_EP_ADDRESS);
	chSysUnlockFromISR();
}

void hid_transmit(USBDriver *usbp) {
    usbPrepareTransmit(usbp, HID_IN_EP_ADDRESS, (uint8_t *)&hid_in_data, sizeof (hid_in_data));
	chSysLockFromISR();
	usbStartTransmitI(usbp, HID_IN_EP_ADDRESS);
    palSetPadMode(GPIOD, 14, PAL_MODE_OUTPUT_PUSHPULL);
    palClearPad(GPIOD, 14);
	chSysUnlockFromISR();
}

//static uint8_t report_prev[64];

bool_t hidRequestsHook(USBDriver *usbp){

 const USBDescriptor *dp;
 if ((usbp->setup[0] & (USB_RTYPE_TYPE_MASK | USB_RTYPE_RECIPIENT_MASK)) ==
       (USB_RTYPE_TYPE_STD | USB_RTYPE_RECIPIENT_INTERFACE)) {
       switch (usbp->setup[1]) {
       case USB_REQ_GET_DESCRIPTOR:
           dp = usbp->config->get_descriptor_cb(
           usbp, usbp->setup[3], usbp->setup[2],
           usbFetchWord(&usbp->setup[4]));
            if (dp == NULL)
                return FALSE;
            usbSetupTransfer(usbp, (uint8_t *)dp->ud_string, dp->ud_size, NULL);
            return TRUE;
        default:
            return FALSE;
       }
    }
 
 if ((usbp->setup[0] & (USB_RTYPE_TYPE_MASK | USB_RTYPE_RECIPIENT_MASK)) ==
       (USB_RTYPE_TYPE_CLASS | USB_RTYPE_RECIPIENT_INTERFACE)) {
    switch (usbp->setup[1]) {
    case HID_GET_REPORT_REQUEST:
      //usbSetupTransfer(usbp, (uint8_t *)&linecoding, sizeof(linecoding), NULL);
      palSetPadMode(GPIOD, 12, PAL_MODE_OUTPUT_PUSHPULL);
      palSetPad(GPIOD, 12);
      hid_in_data.x=0;
      hid_in_data.y=0;
      hid_in_data.button=0;
      usbSetupTransfer(usbp,(uint8_t *)&hid_in_data,sizeof(hid_in_data), NULL);
      usbInitState=1;
      return TRUE;
    case HID_GET_IDLE_REQUEST:
      usbSetupTransfer(usbp,NULL,0, NULL);
      return TRUE;
    case HID_GET_PROTOCOL_REQUEST:
      return TRUE;
    case HID_SET_REPORT_REQUEST:
      usbSetupTransfer(usbp,NULL,0, NULL);
      return TRUE;
    case HID_SET_IDLE_REQUEST:
      usbSetupTransfer(usbp,NULL,0, NULL);
      return TRUE;
    case HID_SET_PROTOCOL_REQUEST:
      return TRUE;
    default:
      return FALSE;
    }
  }
  return FALSE;
}



// Callback for THE IN ENDPOINT (INTERRUPT). Device -> HOST
void hidDataTransmitted(USBDriver *usbp, usbep_t ep){
   (void)usbp;
   (void)ep;
   palSetPadMode(GPIOD, 14, PAL_MODE_OUTPUT_PUSHPULL);
   palSetPad(GPIOD, 14);
   //hid_transmit(usbp,&hid_in_data);
}


// Callback for THE OUT ENDPOINT (INTERRUPT)
void hidDataReceived(USBDriver *usbp, usbep_t ep){
    (void)usbp;
    (void)ep;
    palSetPadMode(GPIOD, 15, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPad(GPIOD, 15);
}




