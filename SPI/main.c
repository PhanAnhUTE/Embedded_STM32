#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

#define SPI_SCK_Pin GPIO_Pin_0
#define SPI_MISO_Pin GPIO_Pin_1
#define SPI_MOSI_Pin GPIO_Pin_2
#define SPI_CS_Pin GPIO_Pin_3
#define SPI_GPIO GPIOA
#define SPI_RCC RCC_APB2Periph_GPIOA


void delay_ms(int timedelay)
{
	TIM_SetCounter(TIM2,0);
		while(TIM_GetCounter(TIM2)<timedelay*10){}
}

void Clock(){
	GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_SET);
	delay_ms(4);
	GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_RESET);
	delay_ms(4);
}



void RCC_Config(){
	RCC_APB2PeriphClockCmd(SPI_RCC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
} 

void TIMER_Config(){

	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;  //clock he thong vd là 72mhz, clock division = 1 => clock timer = 72M/1 = 72M/1
	TIM_InitStruct.TIM_Prescaler = 7200 - 1; // 0.1ms d?m lên 1 l?n
	//VD: 1s = 36.000.000 dao d?ng
	//<=> 1 dao d?ng  = 1/36.000.000 s
	//v?i 1us = (10^-6 x 1)/(1/36.000.000) = 36 dao d?ng(Prescaler) 
	//Prescaler ph?i n?m t? 0 - FFFF <=> 0 - 65535
	TIM_InitStruct.TIM_Period = 0xffff ;   // chu kì d?m, d?m t?i ngu?ng Period s? t? reset v? l?i
	TIM_InitStruct.TIM_CounterMode =  TIM_CounterMode_Up;    // Ch?n mode c?a counter(Up, Down...)
	TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
	TIM_Cmd(TIM2,ENABLE);
	
}



void GPIO_Config(){
	// SCK, Mosi,Cs la output
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = SPI_SCK_Pin| SPI_MOSI_Pin| SPI_CS_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
	
	// Miso la input
	GPIO_InitStructure.GPIO_Pin = SPI_MISO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
}


void SPI_Init(){
	GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_RESET);
	GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET);
	GPIO_WriteBit(SPI_GPIO, SPI_MISO_Pin, Bit_RESET);
	GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET);
}


void SPI_Master_Transmit(uint8_t u8Data){//0b10010000
	uint8_t u8Mask = 0x80;// 0b10000000
	uint8_t tempData;
	GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_RESET);
	delay_ms(1);
	for(int i=0; i<8; i++){
		tempData = u8Data & u8Mask;
		if(tempData){
			GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_SET);
			delay_ms(1);
		} else{
			GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET);
			delay_ms(1);
		}
		u8Data=u8Data<<1;
		Clock();
	}
	GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET);
	delay_ms(1);
}

uint8_t SPI_Slave_Receive(void){
	uint8_t u8Mask = 0x80;
	uint8_t dataReceive =0x00;//0b11000000
	uint8_t temp = 0x00, i=0;
	while(GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin));
	while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin));
	for(i=0; i<8;i++)
    	{ 
		if(GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin)){
		while (GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin)) 
			temp = GPIO_ReadInputDataBit(SPI_GPIO, SPI_MOSI_Pin);
		dataReceive=dataReceive<<1;
		dataReceive=dataReceive|temp;
    		}
	while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin));
	}
	return dataReceive;
}



uint8_t DataTrans[] = {1,3,9,10,15,19,90};//Data
int main(){
	RCC_Config();
	GPIO_Config();
	TIMER_Config();
	SPI_Init();
	while(1)
		{	
			for(int i=0; i<7; i++){
			SPI_Master_Transmit(DataTrans[i]);
			delay_ms(500000);
			}
		}

}




