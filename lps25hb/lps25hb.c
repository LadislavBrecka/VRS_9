/*
 * hts221.c
 *
 *  Created on: 21. 12. 2021
 *      Author: lacoo
 */


#include "lps25hb.h"

uint8_t addres_lps25hb = LPS25HB_DEVICE_ADDRESS_0;


uint8_t lps25hb_read_byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, addres_lps25hb, 0));
}

void lps25hb_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, addres_lps25hb, 0);
}


void lps25hb_readArray(uint8_t * data, uint8_t reg, uint8_t length)
{
	i2c_master_read(data, length, reg, addres_lps25hb, 1);
}

uint8_t lps25hb_init(void)
{
	uint8_t status = 1;

	LL_mDelay(100);

	uint8_t val = lps25hb_read_byte(LPS25HB_WHO_AM_I_ADDRES);

	if(val == LPS25HB1_WHO_AM_I_VALUE)
	{
		status = 1;
	}
	else			//if the device is not found on one address, try another one
	{
		addres_lps25hb = LPS25HB_DEVICE_ADDRESS_1;
		val = lps25hb_read_byte(LPS25HB_WHO_AM_I_ADDRES);
		if(val == LPS25HB1_WHO_AM_I_VALUE)
		{
			status = 1;
		}
		else
		{
			status = 0;
			//return status;
		}
	}

	uint8_t ctrl1 = 0xB4;
	lps25hb_write_byte(LPS25HB_ADDRESS_CTRL1, ctrl1);

	return status;
}

int16_t lps25hb_get_bar()
{
	uint32_t press_count = 	(lps25hb_read_byte(LPS25HB_PRESS_OUT_H_ADDRESS) << 16) |
							(lps25hb_read_byte(LPS25HB_PRESS_OUT_L_ADDRESS) << 8)  |
							lps25hb_read_byte(LPS25HB_PRESS_OUT_XL_ADDRESS);

	float bar = (float)press_count / (float)4096.0;

	return (int16_t) (bar*10);
}
