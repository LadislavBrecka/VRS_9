/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "gpio.h"
#include "lis3mdltr.h"
#include "lsm6ds0.h"
#include "tim.h"
#include "display.h"
#include "hts221.h"
#include "lps25hb.h"
#include <string.h>
#include <math.h>

void SystemClock_Config(void);

#define N_VAL 8
#define N_TXT 6

char dText[N_TXT];
char dValue[N_VAL];
char dResult[N_TXT + N_VAL];

extern uint8_t i = 0;
extern uint8_t state = 1;

int main(void)
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  SystemClock_Config();

  MX_I2C1_Init();
  lsm6ds0_init();
  hts221_init();
  lps25hb_init();

  MX_GPIO_Init();

  setSegments();
  setDigits();
  LL_mDelay(2000);
  resetDigits();
  resetSegments();

  MX_TIM3_Init();

  while (1)
  {

	  	float messurement;
	  	uint8_t prec;

	  	getData(&messurement, &prec);
		displayMessage(messurement, prec);
		LL_mDelay(500);
  }
}

void getData(float *value, uint8_t *prec)
{
	switch(state) {
		case 1:
			strcpy(dText, "TEMP_");
			*prec = 4;
			*value =  hts221_get_temp()/10.0;
			if (*value > 99.9)
				*value = 99.9;
			if (*value < -99.9)
				*value = -99.9;
			break;
		case 2:
			strcpy(dText, "HUM_");
			*prec = 2;
			*value =  hts221_get_hum()/10.0;
			break;
		case 3:
			strcpy(dText, "BAR_");
			*prec = 7;
			*value =  lps25hb_get_bar()/10.0;
			break;
		case 4:
			strcpy(dText, "ALT_");
			*prec = 6;
			float baro = lps25hb_get_bar()/10.0;
			*value =  compute_alt(baro)/10.0;
			break;
	}
}

void displayMessage(float value, uint8_t prec)
{

	if (value < 0)
		gcvt(value, prec+1, dValue);
	else
		gcvt(value, prec, dValue);

	snprintf(dResult, sizeof(dResult), "%s%s", dText, dValue);
	uint8_t dot_pos = removeChar(dResult, '.');
	dot_pos = 4-dot_pos+i;

	char char1,char2,char3,char4;

	char1 = dResult[i];
	char2 = dResult[i+1];
	char3 = dResult[i+2];
	char4 = dResult[i+3];

	if (dResult[i] == '\0') {
	  char1 = ' ';
	  char2 = ' ';
	  char3 = ' ';
	  char4 = ' ';
	} else if (dResult[i+1] == '\0') {
	  char2 = ' ';
	  char3 = ' ';
	  char4 = ' ';
	} else if (dResult[i+2] == '\0') {
	  char3 = ' ';
	  char4 = ' ';
	} else if (dResult[i+3] == '\0') {
	  char4 = ' ';
	}

	displayString(char1, char2, char3, char4, dot_pos);

	if (dResult[i] == '\0')
	  i = 0;
	else
	  i += 1;
}

uint8_t removeChar(char *str, char garbage)
{
	uint8_t ans_pos = 0;
    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) {
        	dst++;
        } else {
        	ans_pos = dst - str;
        }
    }
    *dst = '\0';
    return ans_pos;
}

int16_t compute_alt(float baro)
{
	double alt = ((1 - pow((baro/1013.25), 0.190284))*145366.45) * 0.3048;
	return (int16_t)(alt*10);
}

uint8_t check_button_state(GPIO_TypeDef* PORT, uint8_t PIN)
{
	uint8_t button_state = 0, timeout = 0;

	while(button_state < 20 && timeout < 50)
	{
		if(!(PORT->IDR & (1 << PIN))/*LL_GPIO_IsInputPinSet(PORT, PIN)*/)
		{
			button_state += 1;
		}
		else
		{
			button_state = 0;
		}

		timeout += 1;
		LL_mDelay(1);
	}

	if((button_state >= 20) && (timeout <= 50))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}






/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  Error_Handler();  
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  
  }
  LL_Init1msTick(8000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SYSTICK_EnableIT();
  LL_SetSystemCoreClock(8000000);
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
