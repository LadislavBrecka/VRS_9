/*
 * hts221.h
 *
 *  Created on: 21. 12. 2021
 *      Author: lacoo
 */

#include "main.h"
#include "i2c.h"


#define 	HTS221_DEVICE_ADDRESS_0				    0xBF

#define 	HTS221_WHO_AM_I_VALUE					0xBC
#define 	HTS221_WHO_AM_I_ADDRES					0x0F

#define 	HTS221_ADDRESS_CTRL1					0x20

#define 	HTS221_ADDRESS_TEMP_L					0x2A
#define 	HTS221_ADDRESS_HUM_L					0x28

#define 	T0_degC_x8_ADDRESS						0x32
#define 	T1_degC_x8_ADDRESS						0x33
#define 	T1T0_MSB_ADDRESS					    0x35
#define 	T0_OUT_L_ADDRES							0x3C

#define 	HTS221_H0_RH_X2_ADDRESS					0x30
#define 	HTS221_H0_T0_OUT_L_ADDRESS				0x36
#define 	HTS221_H1_T0_OUT_L_ADDRESS				0x3A
#define 	HTS221_H_OUT_L_ADDRESS			     	0x28






uint8_t hts221_init(void);
uint8_t hts221_read_byte(uint8_t reg_addr);
void hts221_write_byte(uint8_t reg_addr, uint8_t value);
int16_t hts221_get_temp();
int16_t hts221_get_hum();

