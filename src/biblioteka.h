/*
 * biblioteka.h
 *
 *  Created on: 22 kwi 2017
 *      Author: afar
 */

#ifndef BIBLIOTEKA_H_
#define BIBLIOTEKA_H_

void InitAllRCC(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |
			               RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
}

#endif /* BIBLIOTEKA_H_ */
