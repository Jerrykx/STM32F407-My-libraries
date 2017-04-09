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

int main(void)
{

	 SystemInit();

	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |
				 	 	 	RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);



	for(;;);
}
