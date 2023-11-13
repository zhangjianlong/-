#include "bsp_usart_dma.h"
#include  "bsp_led.h"
#include "Delay.h"
static uint8_t SendBuff[SENDBUFF_SIZE];
static DMA_HandleTypeDef  DMA_Handle;      //DMA句柄
static UART_HandleTypeDef UartHandle;      //UART句柄




 /**
  * @brief  USART GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
void Debug_USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
      
  DEBUG_USART_RX_GPIO_CLK_ENABLE();
  DEBUG_USART_TX_GPIO_CLK_ENABLE();
  /* 使能 UART 时钟 */
  DEBUG_USART_CLK_ENABLE();
  
   /* 配置Tx引脚为复用功能  */
  GPIO_InitStruct.Pin = DEBUG_USART_TX_PIN;
	  GPIO_InitStruct.Alternate = DEBUG_USART_TX_AF;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* 配置Rx引脚为复用功能 */
  GPIO_InitStruct.Pin = DEBUG_USART_RX_PIN;
  GPIO_InitStruct.Alternate = DEBUG_USART_RX_AF;
  HAL_GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStruct);
  
  
  UartHandle.Instance          = DEBUG_USART;
  UartHandle.Init.BaudRate     = DEBUG_USART_BAUDRATE;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  
  HAL_UART_Init(&UartHandle); 
}



///重定向c库函数printf到串口DEBUG_USART，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口DEBUG_USART */
	HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 1000);	
	
	return (ch);
}

///重定向c库函数scanf到串口DEBUG_USART，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		
	int ch;
	HAL_UART_Receive(&UartHandle, (uint8_t *)&ch, 1, 1000);	
	return (ch);
}

/**
  * @brief  USART1 TX DMA 配置，内存到外设(USART1->DR)
  * @param  无
  * @retval 无
  */
void USART_DMA_Config(void)
{
  DEBUG_USART_DMA_CLK_ENABLE();  
   
  //Tx DMA配置
  DMA_Handle.Instance=DEBUG_USART_DMA_STREAM;                            //数据流选择
  DMA_Handle.Init.Channel=DEBUG_USART_DMA_CHANNEL;                                //通道选择
  DMA_Handle.Init.Direction=DMA_MEMORY_TO_PERIPH;             //存储器到外设
  DMA_Handle.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
  DMA_Handle.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
  DMA_Handle.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
  DMA_Handle.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
  DMA_Handle.Init.Mode=DMA_NORMAL;                            //外设普通模式
  DMA_Handle.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级
  DMA_Handle.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              //禁用FIFO
  DMA_Handle.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
  DMA_Handle.Init.MemBurst=DMA_MBURST_SINGLE;                 //存储器突发单次传输
  DMA_Handle.Init.PeriphBurst=DMA_PBURST_SINGLE;              //外设突发单次传输
  
  HAL_DMA_Init(&DMA_Handle);
  /* Associate the DMA handle */
  __HAL_LINKDMA(&UartHandle, hdmatx, DMA_Handle); 
  
}
//usart_t_m
void usart_t_m_fun_test(void){
	Debug_USART_Config();
	
	
}
//m_t_usart
void uart_dma_fun_test(void){
	
	Debug_USART_Config();
	
	printf("\r\n USART1 DMA TX 测试 \r\n");
   uint16_t i;
  /*填充将要发送的数据*/
  for(i=0;i<SENDBUFF_SIZE;i++)
  {
    SendBuff[i]	 = 'A';
    
  }

  /*为演示DMA持续运行而CPU还能处理其它事情，持续使用DMA发送数据，量非常大，
  *长时间运行可能会导致电脑端串口调试助手会卡死，鼠标乱飞的情况，
  *或把DMA配置中的循环模式改为单次模式*/		
  
  /* USART1 向 DMA发出TX请求 */
   HAL_UART_Transmit_DMA(&UartHandle, (uint8_t *)SendBuff ,SENDBUFF_SIZE);

  /* 此时CPU是空闲的，可以干其他的事情 */  
  //例如同时控制LED
  while(1)
  {
    LED1_TOGGLE
    Delay(0xFFFFF);
  }
	
	
}
