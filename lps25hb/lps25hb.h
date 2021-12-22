/*
 * lps25hb.h
 *
 *  Created on: 21. 12. 2021
 *      Author: lacoo
 */

#include "main.h"
#include "i2c.h"


#define 	LPS25HB_DEVICE_ADDRESS_0				0xB9
#define 	LPS25HB_DEVICE_ADDRESS_1				0xBB

#define 	LPS25HB1_WHO_AM_I_VALUE					0xBD
#define 	LPS25HB_WHO_AM_I_ADDRES					0x0F

#define 	LPS25HB_ADDRESS_CTRL1					0x20

#define 	LPS25HB_PRESS_OUT_XL_ADDRESS			0x28
#define 	LPS25HB_PRESS_OUT_L_ADDRESS				0x29
#define 	LPS25HB_PRESS_OUT_H_ADDRESS				0x2A





uint8_t lps25hb_init(void);
uint8_t lps25hb_read_byte(uint8_t reg_addr);
void lps25hb_write_byte(uint8_t reg_addr, uint8_t value);
int16_t lps25hb_get_bar();
int16_t lps25hb_get_alt();
