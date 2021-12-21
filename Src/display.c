/*
 * display.c
 *
 *  Created on: Nov 23, 2020
 *      Author: Stancoj
 */

#include "display.h"

display_data_ dDisplayData = {0};
uint64_t disp_time = 0, disp_time_saved = 0;

const unsigned char seven_seg_digits_decode_abcdefg[75]=
{
/*  0     1     2     3     4     5     6     7     8     9     :     ;     */
    0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B, 0x00, 0x00,
/*  <     =     >     ?     @     A     B     C     D     E     F     G     */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47, 0x5E,
/*  H     I     J     K     L     M     N     O     P     Q     R     S     */
    0x37, 0x06, 0x3C, 0x57, 0x0E, 0x55, 0x15, 0x1D, 0x67, 0x73, 0x05, 0x5B,
/*  T     U     V     W     X     Y     Z     [     \     ]     ^     _     */
    0x0F, 0x3E, 0x1C, 0x5C, 0x13, 0x3B, 0x6D, 0x00, 0x00, 0x00, 0x00, 0x08,
/*  `     a     b     c     d     e     f     g     h     i     j     k     */
    0x00, 0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47, 0x5E, 0x37, 0x06, 0x3C, 0x57,
/*  l     m     n     o     p     q     r     s     t     u     v     w     */
    0x0E, 0x55, 0x15, 0x1D, 0x67, 0x73, 0x05, 0x5B, 0x0F, 0x3E, 0x1C, 0x5C,
/*  x     y     z      */
    0x13, 0x3B, 0x6D
};


/* Invalid letters are mapped to all segments off (0x00). */
unsigned char decode_7seg(unsigned char chr)
{
    if (chr > (unsigned char)'z')
        return 0x00;
    if (chr == '-')
    	return 0x01;
    if (chr == '.')
        return 0x80;
    if (chr == ' ')
            return 0x00;
    return seven_seg_digits_decode_abcdefg[chr - '0'];
}

void setAsciiCharacter(unsigned char chr, uint8_t dot)
{
	unsigned char hexChar = decode_7seg(chr);

	if (hexChar & (~0xFE)) {
		LL_GPIO_ResetOutputPin(SEGMENTG_PORT, SEGMENTG_PIN);
	}

	if(hexChar & ((~0xFE) << 1)){
		LL_GPIO_ResetOutputPin(SEGMENTF_PORT, SEGMENTF_PIN);
	}

	if(hexChar & ((~0xFE) << 2)){
		LL_GPIO_ResetOutputPin(SEGMENTE_PORT, SEGMENTE_PIN);
	}

	if(hexChar & ((~0xFE) << 3)){
		LL_GPIO_ResetOutputPin(SEGMENTD_PORT, SEGMENTD_PIN);
	}

	if(hexChar & ((~0xFE) << 4)){
		LL_GPIO_ResetOutputPin(SEGMENTC_PORT, SEGMENTC_PIN);
	}

	if(hexChar & ((~0xFE) << 5)){
		LL_GPIO_ResetOutputPin(SEGMENTB_PORT, SEGMENTB_PIN);
	}

	if(hexChar & ((~0xFE) << 6)){
		LL_GPIO_ResetOutputPin(SEGMENTA_PORT, SEGMENTA_PIN);
	}

	if(hexChar & ((~0xFE) << 7)){
		LL_GPIO_ResetOutputPin(SEGMENTDP_PORT, SEGMENTDP_PIN);
	}

	if(dot){
		LL_GPIO_ResetOutputPin(SEGMENTDP_PORT, SEGMENTDP_PIN);
	}
}

/**
 * Display data in dDisplayData.
 * Sets every digit to display its value and decimal point.
 */
void updateDisplayAlphabet(void)
{
	for(uint8_t i = 0; i < 4; i++)
	{
		setDigit(i);
		if(dDisplayData.resolution == i)
		{
			setAsciiCharacter(dDisplayData.digit[i], 1);

		} else {
			setAsciiCharacter(dDisplayData.digit[i], 0);
		}


		disp_time_saved = disp_time;
		while((disp_time_saved + 2) > disp_time){};

		resetDigits();
		resetSegments();
	}

}

/**
 * Pre-process number before it is displayed. Extract digits of the number
 */
void displayString(uint8_t ch1, uint8_t ch2, uint8_t ch3, uint8_t ch4, uint8_t del)
{
	dDisplayData.digit[0] = ch4;
	dDisplayData.digit[1] = ch3;
	dDisplayData.digit[2] = ch2;
	dDisplayData.digit[3] = ch1;

	dDisplayData.digit_num = 4-del;
	dDisplayData.negative = 0;
	dDisplayData.resolution = del;
}


/*Reset (turn-off) all the segments of display*/
void resetSegments(void)
{
	SEGMENT_A_OFF;
	SEGMENT_B_OFF;
	SEGMENT_C_OFF;
	SEGMENT_D_OFF;
	SEGMENT_E_OFF;
	SEGMENT_F_OFF;
	SEGMENT_G_OFF;
	SEGMENT_DP_OFF;
}

/*Set (turn-on) all the segments of display*/
void setSegments(void)
{
	SEGMENT_A_ON;
	SEGMENT_B_ON;
	SEGMENT_C_ON;
	SEGMENT_D_ON;
	SEGMENT_E_ON;
	SEGMENT_F_ON;
	SEGMENT_G_ON;
	SEGMENT_DP_ON;
}

/* Reset (turn-off) all digits*/
void resetDigits(void)
{
	DIGIT_1_OFF;
	DIGIT_2_OFF;
	DIGIT_3_OFF;
	DIGIT_4_OFF;
}

/* Reset (turn-on) all digits*/
void setDigits(void)
{
	DIGIT_1_ON;
	DIGIT_2_ON;
	DIGIT_3_ON;
	DIGIT_4_ON;
}


/*
 * Turns required digit ON
 */
void setDigit(uint8_t pos)
{
	switch(pos)
	{
		case 0:
			DIGIT_4_ON;
			break;
		case 1:
			DIGIT_3_ON;
			break;
		case 2:
			DIGIT_2_ON;
			break;
		case 3:
			DIGIT_1_ON;
			break;
	}
}






//Update displayed data and keep display ON
void TIM3_IRQHandler(void)
{
	if(LL_TIM_IsActiveFlag_UPDATE(TIM3))
	{
//		updateDisplayNumber();
		updateDisplayAlphabet();
	}

	LL_TIM_ClearFlag_UPDATE(TIM3);
}

