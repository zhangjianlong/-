#include "stm32f4xx.h"

int main(){
	//ʱ��ʹ��
	*(unsigned int *)(0x40023800+0x30) |= (1<<5);
	//GPIOF �������
	*(unsigned int *)(0x40021400+0x00) &=  ~((0x03)<<(2*6));
	*(unsigned int *)(0x40021400+0x00) |= (1<<(2*6));
	//pf6����ߵ�ƽ
	*(unsigned int *)(0x40021400+0x14) |= (1<<6);
	//pf6����͵�ƽ
	*(unsigned int *)(0x40021400+0x14) &= ~(1<<6);
	
	
	while(1){
		
	}
}

void SystemInit(void)
{
	
	
}



