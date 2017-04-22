/**
  ******************************************************************************
  * @file    main.c
  * @author  Jarosław Wieczorek
  * @version V0.1
  * @date    09-April-2017
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_conf.h"
#include "misc.h"
#include "biblioteka.h"

int main(void)
{

	 SystemInit();

	 InitAllRCC();

	 GPIO_InitTypeDef leds;
	 leds.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	 leds.GPIO_Mode = GPIO_Mode_OUT;
	 leds.GPIO_OType = GPIO_OType_PP;
	 leds.GPIO_PuPd = GPIO_PuPd_UP;
	 leds.GPIO_Speed = GPIO_Speed_100MHz;

	 GPIO_Init(GPIOD, &leds);

	 GPIO_SetBits(GPIOD, leds.GPIO_Pin);


	for(;;);
}
