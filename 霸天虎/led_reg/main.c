#include "stm32f4xx.h"

int main(){
	//时钟使能
	*(unsigned int *)(0x40023800+0x30) |= (1<<5);
	//GPIOF 配置输出
	*(unsigned int *)(0x40021400+0x00) &=  ~((0x03)<<(2*6));
	*(unsigned int *)(0x40021400+0x00) |= (1<<(2*6));
	//pf6输出高电平
	*(unsigned int *)(0x40021400+0x14) |= (1<<6);
	//pf6输出低电平
	*(unsigned int *)(0x40021400+0x14) &= ~(1<<6);
	
	
	while(1){
		
	}
}

void SystemInit(void)
{
	
	
}



