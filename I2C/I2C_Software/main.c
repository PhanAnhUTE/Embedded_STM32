#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

#define I2C_SCL 		GPIO_Pin_6
#define I2C_SDA			GPIO_Pin_7

#define I2C_GPIO 	GPIOB


#define WRITE_SDA_0 	GPIO_ResetBits(I2C_GPIO, I2C_SDA)
#define WRITE_SDA_1 	GPIO_SetBits(I2C_GPIO, I2C_SDA)
#define WRITE_SCL_0 	GPIO_ResetBits(I2C_GPIO, I2C_SCL)
#define WRITE_SCL_1 	GPIO_SetBits(I2C_GPIO, I2C_SCL)
#define READ_SDA_VAL 	GPIO_ReadInputDataBit(I2C_GPIO, I2C_SDA)



void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}


void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = I2C_SDA| I2C_SCL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
}

void delay_us(int timedelay)
{
	TIM_SetCounter(TIM2,0);
		while(TIM_GetCounter(TIM2)<timedelay*10){}
}


void I2C_Config(){
	WRITE_SDA_1;
	delay_us(1);
	WRITE_SCL_1;
	delay_us(1);
}





void TIMER_Config(){

	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;  //clock he thong vd là 72mhz, clock division = 1 => clock timer = 72M/1 = 72M/1
	TIM_InitStruct.TIM_Prescaler = 7200 - 1; // 0.1ms dem lên 1 lan
	//VD: 1s = 36.000.000 dao dong
	//<=> 1 dao dong  = 1/36.000.000 s
	//voi 1us = (10^-6 x 1)/(1/36.000.000) = 36 dao dong(Prescaler) 
	//Prescaler phai nam tu 0 - FFFF <=> 0 - 65535
	TIM_InitStruct.TIM_Period = 0xffff ;   // chu kì dem, dem toi nguong Period se tu reset ve lai
	TIM_InitStruct.TIM_CounterMode =  TIM_CounterMode_Up;    // Chan mode cua counter(Up, Down...)
	TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
	TIM_Cmd(TIM2,ENABLE);
	
}




typedef enum{
		NOT_OK = 0,
		OK
} status;


typedef enum{
	N_ACK = 0,
	ACK

} ACK_Bit;

void I2C_Start(){
	
	WRITE_SCL_1;  	
	delay_us(3);	
	WRITE_SDA_1;
	delay_us(3);
	WRITE_SDA_0;	//SDA reset to 0 before SCL.
	delay_us(3);
	WRITE_SCL_0;
	delay_us(3);
}


void I2C_Stop(){
	
	WRITE_SDA_0;
	delay_us(3);
	WRITE_SCL_1; 	//SCL set to 1 before SDA.
	delay_us(3);
	WRITE_SDA_1;
	delay_us(3);
}


status I2C_Write(uint8_t u8Data){	
	uint8_t i;
	status stRet;
	for(int i=0; i< 8; i++){		//Write byte data.
		if (u8Data & 0x80) {
			WRITE_SDA_1;
		} else {
			WRITE_SDA_0;
		}
		delay_us(3);
		WRITE_SCL_1;
		delay_us(5);
		WRITE_SCL_0;
		delay_us(5);
		u8Data <<= 1;
	}
	
		WRITE_SDA_1;					
		delay_us(3);
		WRITE_SCL_1;					//
		delay_us(3);
		
		if (READ_SDA_VAL) {	
			stRet = NOT_OK;				
		} else {
			stRet = OK;					
		}

		delay_us(2);
		WRITE_SCL_0;
		delay_us(5);
		
		return stRet;
	}


uint8_t I2C_Read(ACK_Bit _ACK){	
	uint8_t i;						
	uint8_t u8Ret = 0x00;
	WRITE_SDA_1;
	delay_us(3);	
	for (i = 0; i < 8; ++i) {
		u8Ret <<= 1;
		WRITE_SCL_1;
		delay_us(3);
		if (READ_SDA_VAL) {
			u8Ret |= 0x01;
		}
		delay_us(2);
		WRITE_SCL_0;
		delay_us(5);
	}

	if (_ACK) {	
		WRITE_SDA_0;
	} else {
		WRITE_SDA_1;
	}
	delay_us(3);
	
	WRITE_SCL_1;
	delay_us(5);
	WRITE_SCL_0;
	delay_us(5);

	return u8Ret;
}
	int main()
	{
			while(1)
			{
				
			}
	}
