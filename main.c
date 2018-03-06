
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

int ADC_Result;

void  TIM4_IRQHandler(void){
		if(TIM_GetFlagStatus(TIM4, TIM_FLAG_Update)) {

			GPIO_ToggleBits(GPIOD, GPIO_Pin_12);

       	TIM_ClearFlag(TIM4, TIM_FLAG_Update);

}

void main(void) {
	//USTAWIENIA GPIO

    GPIO_InitTypeDef  LEDS;
    /* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
    LEDS.GPIO_Pin =  GPIO_Pin_12 | GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;
    LEDS.GPIO_Mode = GPIO_Mode_OUT;
    LEDS.GPIO_OType = GPIO_OType_PP;
    LEDS.GPIO_Speed = GPIO_Speed_100MHz;
    LEDS.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &LEDS);


    GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15);

    GPIO_SetBits(GPIOD, GPIO_Pin_13);
    GPIO_SetBits(GPIOD, GPIO_Pin_15);

	//USTAWIENIA TIMERA
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	/* Time base structure */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 8399;
	TIM_TimeBaseStructure.TIM_Prescaler = 2999;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM4, ENABLE);

	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel = TIM4_IRQn;						// priorytet gÅ‚Ã³wny
	nvic.NVIC_IRQChannelPreemptionPriority = 0x00;			// numer przerwania
	nvic.NVIC_IRQChannelSubPriority = 0x00;					// subpriorytet
	nvic.NVIC_IRQChannelCmd = ENABLE;						// uruchom dany kanaÅ‚
	NVIC_Init(&nvic);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // zegar dla portu GPIO z którego wykorzystany zostanie pin jako wejœcie ADC (PA1)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); // zegar dla modu³u ADC1

	GPIO_InitTypeDef GPIO_InitStructure2;
	//inicjalizacja wejœcia ADC
	GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure2.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure2);

	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	// niezale¿ny tryb pracy przetworników
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	// zegar g³ówny podzielony przez 2
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	// opcja istotna tylko dla trybu multi ADC
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	// czas przerwy pomiêdzy kolejnymi konwersjami
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitTypeDef ADC_InitStructure;
	//ustawienie rozdzielczoœci przetwornika na maksymaln¹ (12 bitów)
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	//wy³¹czenie trybu skanowania (odczytywaæ bêdziemy jedno wejœcie ADC
	//w trybie skanowania automatycznie wykonywana jest konwersja na wielu 	//wejœciach/kana³ach)
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	//w³¹czenie ci¹g³ego trybu pracy
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	//wy³¹czenie zewnêtrznego wyzwalania
	//konwersja mo¿e byæ wyzwalana timerem, stanem wejœcia itd. (szczegó³y w //dokumentacji)
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	//wartoœæ binarna wyniku bêdzie podawana z wyrównaniem do prawej
	//funkcja do odczytu stanu przetwornika ADC zwraca wartoœæ 16-bitow¹
	//dla przyk³adu, wartoœæ 0xFF wyrównana w prawo to 0x00FF, w lewo 0x0FF0
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	//liczba konwersji równa 1, bo 1 kana³
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	// zapisz wype³nion¹ strukturê do rejestrów przetwornika numer 1
	ADC_Init(ADC1, &ADC_InitStructure);

	//W nastêpnej kolejnoœci nale¿y skonfigurowaæ wybrany kana³ ADC:
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_84Cycles);
	//uruchomienie
	ADC_Cmd(ADC1, ENABLE);

	for (;;) {
	/*	ADC_SoftwareStartConv(ADC1);
		while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
			;
		ADC_Result = ADC_GetConversionValue(ADC1);
		if (ADC_Result < 10) {
			TIM_TimeBaseStructure.TIM_Prescaler = 2999;
			TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

		} else {
			TIM_TimeBaseStructure.TIM_Prescaler /= 2;
		}
	}*/
	}
}}




