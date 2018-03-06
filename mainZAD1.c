#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"



int pwm_value = 0;


void USART3_IRQHandler(void)
{
	// sprawdzenie flagi zwiazanej z odebraniem danych przez USART
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {

		if(USART3->DR == '+')
		{
			if(pwm_value >= 100)
			{
				pwm_value=100;
				TIM4->CCR1 = pwm_value*10;
				TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
				GPIO_SetBits(GPIOD, GPIO_Pin_14);
				while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
					// wyslanie danych
				USART_SendData(USART3, 'M');
					// czekaj az dane zostana wyslane
				while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);

				while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
								// wyslanie danych
					USART_SendData(USART3, 'A');
								// czekaj az dane zostana wyslane
				while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);


				while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
										// wyslanie danych
						USART_SendData(USART3, 'X');
										// czekaj az dane zostana wyslane
				while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);

				USART_SendData(USART3, '\n');
								// czekaj az dane zostana wyslane
				while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
			}
			else
			{
				pwm_value+=10;
				TIM4->CCR1 = pwm_value*10;
				GPIO_ResetBits(GPIOD, GPIO_Pin_14);
			}
			GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
		}
		if(USART3->DR == '-')
		{
				if(pwm_value <= 0)
				{
							TIM4->CCR1 = 0;
							pwm_value=0;
							GPIO_SetBits(GPIOD, GPIO_Pin_14);
							while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
								// wyslanie danych
							USART_SendData(USART3, 'M');
								// czekaj az dane zostana wyslane
							while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);

							while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
											// wyslanie danych
								USART_SendData(USART3, 'I');
											// czekaj az dane zostana wyslane
							while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);


							while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
													// wyslanie danych
									USART_SendData(USART3, 'N');
													// czekaj az dane zostana wyslane
							while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);

							USART_SendData(USART3, '\n');
											// czekaj az dane zostana wyslane
							while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
				}
				else
				{
					pwm_value-=10;
					TIM4->CCR1 = pwm_value*10;
					  GPIO_ResetBits(GPIOD, GPIO_Pin_14);
				}

		}


		USART_ClearFlag(USART3, USART_IT_RXNE);
	}
}



typedef int bool;
enum { false, true };


int mainz(void)
{
	SystemInit();
	// ustawienie trybu pracy priorytetów przerwañ
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);

	// konfiguracja linii Rx i Tx
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	// ustawienie funkcji alternatywnej dla pinów (USART)
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);


	GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitStructure1.GPIO_Pin =  GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure1.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure1);


	//DIODA PWM
	GPIO_InitTypeDef pwmled;
	pwmled.GPIO_Pin = GPIO_Pin_13;
	pwmled.GPIO_Mode = GPIO_Mode_AF;
	pwmled.GPIO_OType = GPIO_OType_PP;
	pwmled.GPIO_PuPd = GPIO_PuPd_UP;
	pwmled.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &pwmled);

	GPIO_SetBits(GPIOD, GPIO_Pin_13);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);

	TIM_TimeBaseInitTypeDef TIM_4;
	TIM_4.TIM_Period = 8400 - 1;
	TIM_4.TIM_Prescaler = 999;
	TIM_4.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_4.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_4);


	//USTAWIENIA PWM
	TIM_OCInitTypeDef TIM_OCInitStructure;
	/* PWM1 Mode configuration: */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	//Konfiguracjê kana³ów timera do pracy w trybie OutputCompare wykonuje siê poprzez 		//ka¿dorazowe wykonanie funkcji:
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	//TIM4->CCR1 = 0;
	TIM_Cmd(TIM4, ENABLE);







	//struktura do konfiguracji kontrolera NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	// wlaczenie przerwania zwi¹zanego z odebraniem danych (pozostale zrodla przerwan zdefiniowane sa w pliku stm32f4xx_usart.h)
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// konfiguracja kontrolera przerwan
	NVIC_Init(&NVIC_InitStructure);
	// wlaczenie przerwan od ukladu USART
	NVIC_EnableIRQ(USART3_IRQn);


	USART_InitTypeDef USART_InitStructure;
	// predkosc transmisji (mozliwe standardowe opcje: 9600, 19200, 38400, 57600, 115200, ...)
	USART_InitStructure.USART_BaudRate = 115200;
	// d³ugoœæ s³owa (USART_WordLength_8b lub USART_WordLength_9b)
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// liczba bitów stopu (USART_StopBits_1, USART_StopBits_0_5, USART_StopBits_2, USART_StopBits_1_5)
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// sprawdzanie parzystoœci (USART_Parity_No, USART_Parity_Even, USART_Parity_Odd)
	USART_InitStructure.USART_Parity = USART_Parity_No;
	// sprzêtowa kontrola przep³ywu (USART_HardwareFlowControl_None, USART_HardwareFlowControl_RTS, USART_HardwareFlowControl_CTS, USART_HardwareFlowControl_RTS_CTS)
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// tryb nadawania/odbierania (USART_Mode_Rx, USART_Mode_Rx )
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	// konfiguracja
	USART_Init(USART3, &USART_InitStructure);

	// wlaczenie ukladu USART
	USART_Cmd(USART3, ENABLE);

	while(true)
	{





	}
}





