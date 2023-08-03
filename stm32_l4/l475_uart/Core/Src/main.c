
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"

void SystemClock_Config(void);
#define RXBUFFERSIZE             64
extern UART_HandleTypeDef huart1; //UART句柄
typedef struct
{
    uint8_t  rx_len;
    uint8_t  rx_delay;
    uint8_t  rx_buf[RXBUFFERSIZE];
} struct_uart_info;

struct_uart_info g_uart_info;




/**
 * @brief	串口1中断服务程序
 */
void USART1_IRQHandler()
{
    if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) == SET)
    {
        g_uart_info.rx_delay = 20;
        if(g_uart_info.rx_len++ >= RXBUFFERSIZE)
        {
            g_uart_info.rx_len = 0;
        }
    }
	 HAL_UART_IRQHandler(&huart1);
}




void key_init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    GPIO_Initure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_Initure.Mode = GPIO_MODE_INPUT;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_Initure);

//    //开启usart1的接收中断
//    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
//    //使能usart1,中断通道
//    HAL_NVIC_EnableIRQ(USART1_IRQn);
//    //设置中断优先级
//    HAL_NVIC_SetPriority(USART1_IRQn, 3, 3);
}

void led_init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOE_CLK_ENABLE();
    GPIO_Initure.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOE, &GPIO_Initure);

    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_SET);
}

uint8_t  key_scan(void)
{
    static uint8_t key_up = 1;

    if(key_up && (KEY0_VALUE == 0 || KEY1_VALUE == 0 || KEY2_VALUE == 0))
    {
        HAL_Delay(10);
        key_up = 0;
        if(KEY0_VALUE == 0)
        {
            return KEY0_PRES;
        }
        else if(KEY1_VALUE == 0)
        {
            return KEY1_PRES;
        }
        else if(KEY2_VALUE == 0)
        {
            return KEY2_PRES;
        }
    }
    else if(KEY0_VALUE == 1 && KEY1_VALUE == 1 && KEY2_VALUE == 1)
    {
        key_up = 1;
    }
    return 0;
}

void handle_key(void)
{
    switch(key_scan())
    {
    case KEY0_PRES:
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_RESET);
        HAL_UART_Transmit(&huart1, (uint8_t*)"key0", sizeof("key"), 1000);
        break;
    case KEY1_PRES:
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_RESET);
        HAL_UART_Transmit(&huart1, (uint8_t*)"key1", sizeof("key1"), 1000);
        break;
    case KEY2_PRES:
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_RESET);
        HAL_UART_Transmit(&huart1, (uint8_t*)"key2", sizeof("key2"), 1000);
        break;
    }

}
//接收数据处理的逻辑
void tick_handler()
{
    if(g_uart_info.rx_delay)
    {
        g_uart_info.rx_delay--;
    }
}

//接收完一帧uart的数据处理逻辑
void handle_uart(void)
{
    if(g_uart_info.rx_delay)
    {
        return;
    }
    if(g_uart_info.rx_len == 0)
    {
        goto UART_EXIT;
    }
    HAL_UART_AbortReceive_IT(&huart1);
    if(strstr((const char *)g_uart_info.rx_buf, "R_OPEN"))
    {
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);

    }
    else if(strstr((const char *)g_uart_info.rx_buf, "G_OPEN"))
    {
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);

    }
    else if(strstr((const char *)g_uart_info.rx_buf, "B_OPEN"))
    {
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);

    }
    else if(strstr((const char *)g_uart_info.rx_buf, "R_CLOSE"))
    {
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
    }
    else if(strstr((const char *)g_uart_info.rx_buf, "G_CLOSE"))
    {
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
    }
    else if(strstr((const char *)g_uart_info.rx_buf, "B_CLOSE"))
    {
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
    }
    else if(strstr((const char *)g_uart_info.rx_buf, "CLOSE"))
    {
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_SET);
    }
UART_EXIT:
    g_uart_info.rx_len = 0;
    memset(g_uart_info.rx_buf, 0, sizeof(g_uart_info.rx_buf));
   HAL_UART_Receive_IT(&huart1, g_uart_info.rx_buf, sizeof(g_uart_info.rx_buf));
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    key_init();
    led_init();
    HAL_UART_Receive_IT(&huart1, g_uart_info.rx_buf, sizeof(g_uart_info.rx_buf));
    while(1)
    {
        handle_key();
        handle_uart();
    }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
    */
    if(HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 10;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while(1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
