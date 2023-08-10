
#include "gpio.h"


void led_init(){
	 
	 __HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init_struct = {0};
	gpio_init_struct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOE,&gpio_init_struct);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9,GPIO_PIN_SET);
	
}

void key_init(){
	
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	GPIO_InitTypeDef gpio_init_struct = {0};
	gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;
	gpio_init_struct.Pull = GPIO_PULLUP;
	gpio_init_struct.Pin = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOD,&gpio_init_struct);
	
	HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void EXTI15_10_IRQHandler(){
	HAL_Delay(200);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	switch(GPIO_Pin){
		case GPIO_PIN_10		:
			HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_7);
		
		break;
		
		
	}
	
}
