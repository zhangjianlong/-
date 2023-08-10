#include "time.h"

#include "stm32l4xx.h"

TIM_HandleTypeDef TIM_Handler;      //��ʱ�����
TIM_HandleTypeDef TIM1_Handler;      //��ʱ�����
TIM_OC_InitTypeDef TIM1_CHnHandler;	    //��ʱ��2���
//������ʱ��
void base_time_init(){
	TIM_Handler.Instance = TIM6;
	//Ԥ��Ƶ
	TIM_Handler.Init.Prescaler = 8000-1;
	//�Զ�װ��ֵ
	TIM_Handler.Init.Period = 10000-1;
	TIM_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&TIM_Handler);
	
	HAL_TIM_Base_Start_IT(&TIM_Handler);			
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM6)
    {
        __HAL_RCC_TIM6_CLK_ENABLE();            
        HAL_NVIC_SetPriority(TIM6_IRQn, 1, 3);  
        HAL_NVIC_EnableIRQ(TIM6_IRQn);         
    }
}
void TIM6_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM_Handler);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == (&TIM_Handler))
    {
      	HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_7);
	}
}

void pwm_init(){
		TIM1_Handler.Instance = TIM1;
		TIM1_Handler.Init.Prescaler = 8000-1;//80k/8000 = 10k;
		TIM1_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;
		TIM1_Handler.Init.Period = 10000-1;
	
		TIM1_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		HAL_TIM_PWM_Init(&TIM1_Handler);
		
		TIM1_CHnHandler.OCMode = TIM_OCMODE_PWM1;
		TIM1_CHnHandler.Pulse = 10000/2;
		TIM1_CHnHandler.OCPolarity = TIM_OCPOLARITY_HIGH;
		
	HAL_TIM_PWM_ConfigChannel(&TIM1_Handler, &TIM1_CHnHandler, TIM_CHANNEL_1); //����TIM2ͨ��1
    HAL_TIM_PWM_Start(&TIM1_Handler, TIM_CHANNEL_1); 	//����PWMͨ��1


    HAL_TIM_PWM_ConfigChannel(&TIM1_Handler, &TIM1_CHnHandler, TIM_CHANNEL_2); //����TIM2ͨ��2
    HAL_TIM_PWM_Start(&TIM1_Handler, TIM_CHANNEL_2); 	
	
}
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM1_CLK_ENABLE();				//ʹ�ܶ�ʱ��2
    __HAL_RCC_GPIOE_CLK_ENABLE();				//����GPIOAʱ��

    GPIO_Initure.Pin = GPIO_PIN_9; //PA0.1
    GPIO_Initure.Mode =GPIO_MODE_OUTPUT_PP;  		//�����������
    GPIO_Initure.Pull = GPIO_PULLUP;        	//����
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;   	//����
//    GPIO_Initure.Alternate = GPIO_AF1_TIM1;		//PA1����ΪTIM2_CH1.CH2
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);
}

void TIM_SetTIM1Compare1(uint32_t compare)
{
    TIM2->CCR1 = compare;
}


