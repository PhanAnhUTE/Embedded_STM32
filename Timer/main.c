/*Timer
Là 1 mach digital logic có vai trò dém moi chu ki clock(dem lên hoac xuong)
stm32f103 có 7 timer 

tick vào GPIO,Tim,RCC */

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

void RCC_Config(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
}

void GPIOC_Config(void){
				GPIOC->CRH |= GPIO_CRH_MODE13_0; 	//MODE13[1:0] = 11: Output mode, max speed 50 MHz
				GPIOC->CRH |= GPIO_CRH_MODE13_1; 	
				GPIOC->CRH &= ~GPIO_CRH_CNF13_0;	              //CNF13[1:0] = 00: General purpose output push-pull
				GPIOC->CRH &= ~GPIO_CRH_CNF13_1;
	}

void TIMER_Config(){

	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;  //clock he thong vd là 72mhz, clock division = 1 => clock timer = 72M/1 = 72M/1
	TIM_InitStruct.TIM_Prescaler = 7200 - 1; // 0.1ms dem lên 1 lan
	//VD: 1s = 36.000.000 dao dong
	//<=> 1 dao d?ng  = 1/36.000.000 s
	//voi 1us = (10^-6 x 1)/(1/36.000.000) = 36 dao dong(Prescaler) 
	//Prescaler phai nam tu 0 - FFFF <=> 0 - 65535
	TIM_InitStruct.Tim_Period = 0xffff ;   // chu kì dem, dem toi nguong Period se tu reset ve lai
	TIM_InitStruct.TIM_CounterMode =  TIM_CounterMode_Up;    // Chon mode cua counter(Up, Down...)
	TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
	TIM_Cmd(TIM2,ENABLE);
	
}

void delay_ms(uint8_t timedelay)
{
	TIM_SetCounter(TIM2,0);
		while(TIM_GetCounter(TIM2)<timedelay*10)
}


/*void delay_ms(uint8_t timedelay)
{
	int cnt = 0;
	while(cnt<1000)
	{
		delay_ms(1);
	}
} */

int main()
{
	RCC_Config();
	TIMER_Config();
	GPIOC_Config();

	while(1){
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		delay_ms(10);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		delay_ms(10);
		
	}

}