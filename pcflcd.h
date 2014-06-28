#ifndef _PCFLCD_H_
#define _PCFLCD_H_

#define DEFAULT_ADDRESS 0x27 // a0..a3 unsoldered -- 0x27; soldered: 0x20
#define PCF_I2CD I2CD1 // change this

msg_t pcflcd_init(void);
msg_t pcflcd_write_char(const char chr);

#endif
