/*
 * hts221.c
 *
 *  Created on: 21. 12. 2021
 *      Author: lacoo
 */

#include "hts221.h"

uint8_t addres_hts221 = HTS221_DEVICE_ADDRESS_0;


uint8_t hts221_read_byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, addres_hts221, 0));
}

void hts221_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, addres_hts221, 0);
}


void hts221_readArray(uint8_t * data, uint8_t reg, uint8_t length)
{
	i2c_master_read(data, length, reg, addres_hts221, 1);
}

uint8_t hts221_init(void)
{
	uint8_t status = 1;

	LL_mDelay(100);

	uint8_t val = hts221_read_byte(HTS221_WHO_AM_I_ADDRES);

	if(val == HTS221_WHO_AM_I_VALUE)
	{
		status = 1;
	}

	uint8_t ctrl1 = 0x86;
	hts221_write_byte(HTS221_ADDRESS_CTRL1, ctrl1);

	return status;
}

int16_t hts221_get_temp()
{
	int16_t T0_out, T1_out, T_out, T0_degC_x8_u16, T1_degC_x8_u16;
	int16_t T0_degC, T1_degC;
	uint8_t buffer[4], tmp;
	int32_t tmp32;

	hts221_readArray(buffer, T0_degC_x8_ADDRESS, 2);
	tmp = hts221_read_byte(T1T0_MSB_ADDRESS);

	T0_degC_x8_u16 = (((uint16_t)(tmp & 0x03)) << 8) | ((uint16_t)buffer[0]);
	T1_degC_x8_u16 = (((uint16_t)(tmp & 0x0C)) << 6) | ((uint16_t)buffer[1]);
	T0_degC = T0_degC_x8_u16>>3;
	T1_degC = T1_degC_x8_u16>>3;

	hts221_readArray(buffer, T0_OUT_L_ADDRES, 4);

	T0_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];
	T1_out = (((uint16_t)buffer[3])<<8) | (uint16_t)buffer[2];

	hts221_readArray(buffer, HTS221_ADDRESS_TEMP_L, 2);

	T_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];

	tmp32 = ((int32_t)(T_out - T0_out)) * ((int32_t)(T1_degC - T0_degC)*10);

	return tmp32 /(T1_out - T0_out) + T0_degC*10;
}

int16_t hts221_get_hum() {
	int16_t H0_T0_out, H1_T0_out, H_T_out;
	int16_t H0_rh, H1_rh;
	uint8_t buffer[2];
	int32_t tmp;

	hts221_readArray(buffer, HTS221_H0_RH_X2_ADDRESS, 2);

	H0_rh = buffer[0]>>1;
	H1_rh = buffer[1]>>1;

	hts221_readArray(buffer, HTS221_H0_T0_OUT_L_ADDRESS, 2);
	H0_T0_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];

	hts221_readArray(buffer, HTS221_H1_T0_OUT_L_ADDRESS, 2);
	H1_T0_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];

	hts221_readArray(buffer, HTS221_H_OUT_L_ADDRESS, 2);
	H_T_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];

	tmp = ((int32_t)(H_T_out - H0_T0_out)) * ((int32_t)(H1_rh - H0_rh)*10);

	int16_t value = (tmp/(H1_T0_out - H0_T0_out) + H0_rh*10);

	if(value>1000) value = 1000;

	return value;




}



