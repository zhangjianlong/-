#include "stm32f4xx.h"

int main(){
	*(unsigned int *)(0x40021400+0x14) |= (1<<6);
	*(unsigned int *)(0x40021400+0x14) |= (1<<6);
	*(unsigned int *)(0x40021400+0x14) &= ~(1<<6);
	
	
	while(1){
		
	}
}

void SystemInit(void)
{
	
	
}



