#include "stm32f10x.h"                  // Device header
/*
	Function		PIN
	DAC1_OUT		PA4
	DAC2_OUT 		PA5
*/

#define FLASH_TIME 0xFFFFF
void delay_ms(int delay_time);
float voltaje1;
float voltaje2;
unsigned short dac_1v;
unsigned short dac_2v;
int main(){
	//* TURN PERIPHERIAL *//
	//Turn on DAC
	RCC->APB1ENR|=(0x1<<29);//Bit 29 DACEN: DAC interface clock enable = 0x1: DAC interface clock enabled
	//Turn on GPIOA
	RCC->APB2ENR|=(0x1<<2);//Bit 2 IOPAEN: I/O port A clock enable = 0x1: I/O port A clock enabled
	
	//* CONFIGURE *//
	//Configure PA4 and PA5 as analog inputs (to avoid unitentional parasitic comsumption)
	GPIOA->CRL&=~((0x3<<22)//clean field CNF5[1:0] at 23:22  
		|(0x3<<20)// clean field MODE5[1:0] at 21:20
		|(0x3<<18)// clean field CNF4[1:0] 19:18
		|(0x3<<16));//clean field MODE4[1:0] 17:16
	GPIOA->CRL|=((0x0<<22)//23:22 CNF5[1:0] = 0x0: Analog mode   
		|(0x0<<20)// 21:20 MODE5[1:0] = 0x0: Input mode 
		|(0x0<<18)// 19:18 CNF4[1:0] = 0x0: Analog mode
		|(0x0<<16));// 17:16 MODE4[1:0]= 0x0: Input mode 
	
	DAC->CR=(0x0);//clean CR register (all fields)
	DAC->CR|=((0x1<<17)//Bit 17 BOFF2: DAC channel2 output buffer disable = 0x1
	|(0x1<<16)//Bit 16 EN2: DAC channel2 enable = 0x1
	|(0x1<<1)//Bit 1 BOFF1: DAC channel1 output buffer disable = 0x1
	|(0x1<<0));//Bit 0 EN1: DAC channel1 enable = 0x1
	
	while(1){
		dac_1v=(unsigned short) (voltaje1*(4095.0f/3.0f));
		dac_2v=(unsigned short) (voltaje2*(4095.0f/3.0f));
		DAC->DHR12R1=dac_1v;
		DAC->DHR12R2=dac_2v;
		delay_ms(FLASH_TIME);
	}
}

void delay_ms(int delay_time){
	for (int i=0; i<delay_time; i++);
}
