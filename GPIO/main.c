#include "stm32f10x.h"


#define HIGH 1
#define LOW 0

void RCC_Config(){
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN; //Kich hoat xung clock cap cho GPIOC
	
	}


void GPIOC_Config(void){
				GPIOC->CRH |= GPIO_CRH_MODE13_0; 	//MODE13[1:0] = 11: Output mode, max speed 50 MHz
				GPIOC->CRH |= GPIO_CRH_MODE13_1; 	
				GPIOC->CRH &= ~GPIO_CRH_CNF13_0;	              //CNF13[1:0] = 00: General purpose output push-pull
				GPIOC->CRH &= ~GPIO_CRH_CNF13_1;
	}

void GPIOA_Config(void){
				GPIOA->CRH &= ~GPIO_CRL_MODE0_0; 	//MODE = 00: Output mode, max speed 50 MHz
				GPIOA->CRH &= ~GPIO_CRL_MODE0_1;
				GPIOA->CRH |= GPIO_CRL_CNF0_1;	              //CNF = 10: General purpose output push-pull
				GPIOA->CRH &= ~GPIO_CRL_CNF0_0;
				GPIOA->ODR |= GPIO_ODR_ODR0;
	}	
	
	

void delay(uint32_t delaytime)
	{
			for(int i = 0; i< delaytime;i++){}
		}

void WritePin( uint8_t Pin, uint8_t state)
	{
			if (state == HIGH)
				GPIOC->ODR |= (1<<Pin);
			else
				GPIOC->ODR &= ~(1<<Pin);
		}
	
void toggle(){
		if(GPIOC->ODR & (1<<13)){
				GPIOC->ODR &= ~(1<<13);
			}
		else{
				GPIOC->ODR |= 1<<13;
			}
	}

int main(){
	
		RCC_Config();
		GPIOC_Config();
		GPIOA_Config();
		while(1)
			{
			
					//WritePin(13,HIGH);
					//delay(1000000);
					//WritePin(13,LOW);
					//delay(1000000);
				
				
				if ( (GPIOA->IDR & (1<<0)) == 0){
						while((GPIOA->IDR & (1<<0)) == 0);
							toggle();
					}
			}
	}