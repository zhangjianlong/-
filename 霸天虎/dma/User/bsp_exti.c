
#include "bsp_exti.h"
#include "./led/bsp_led.h"
#include  "delay.h"
/* 相关宏定义，使用存储器到存储器传输必须使用DMA2 */
DMA_HandleTypeDef DMA_Handle;

#define DMA_STREAM               DMA2_Stream0
#define DMA_CHANNEL              DMA_CHANNEL_0
#define DMA_STREAM_CLOCK()       __DMA2_CLK_ENABLE()

/* 定义aSRC_Const_Buffer数组作为DMA传输数据源
  const关键字将aSRC_Const_Buffer数组变量定义为常量类型 */
  
#define BUFFER_SIZE              32

/* 定义aSRC_Const_Buffer数组作为DMA传输数据源
  const关键字将aSRC_Const_Buffer数组变量定义为常量类型 */
const uint32_t aSRC_Const_Buffer[BUFFER_SIZE]= {
                                    0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
                                    0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
                                    0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
                                    0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
                                    0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
                                    0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
                                    0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
                                    0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80};
/* 定义DMA传输目标存储器 */
uint32_t aDST_Buffer[BUFFER_SIZE];

static void SystemClock_Config(void);
uint8_t Buffercmp(const uint32_t* pBuffer, uint32_t* pBuffer1, uint16_t BufferLength);
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
									
									

void m_m_dma(void){
		/* 定义存放比较结果变量 */
	uint8_t TransferStatus;
	// 等待dma传输完成
	while(__HAL_DMA_GET_FLAG(&DMA_Handle,DMA_FLAG_TCIF0_4) == DISABLE){
		
		
	}
	//比较元数据于传输后数据
	TransferStatus = Buffercmp(aSRC_Const_Buffer,aDST_Buffer,BUFFER_SIZE);
		/* 判断源数据与传输后数据比较结果*/
	if(TransferStatus==0)  
	{
		/* 源数据与传输后数据不相等时RGB彩色灯显示红色 */
		LED_RED;
	}
	else
	{ 
		/* 源数据与传输后数据相等时RGB彩色灯显示蓝色 */
		LED_BLUE;
	}
	
}




//按键中断
void key_exit_init(void){
	//gpio 初始化
	GPIO_InitTypeDef gpio_init;	
	//初始化GPIO时钟
	KEY1_INT_GPIO_CLK_ENABLE();
	KEY2_INT_GPIO_CLK_ENABLE();
	//初始化GPIO
	gpio_init.Pin = KEY1_INT_GPIO_PIN;
	 /* 设置引脚为输入模式 */ 
	gpio_init.Mode  = GPIO_MODE_IT_RISING;
	  /* 设置引脚不上拉也不下拉 */
    gpio_init.Pull = GPIO_NOPULL;
	//初始化GPIO
	HAL_GPIO_Init(KEY1_INT_GPIO_PORT,&gpio_init);
	 /* 配置 EXTI 中断源 到key1 引脚、配置中断优先级*/
	HAL_NVIC_SetPriority(KEY1_INT_EXTI_IRQ,0,0);
	 /* 使能中断 */
	HAL_NVIC_EnableIRQ(KEY1_INT_EXTI_IRQ);
	
	
}
void DMA_Config(void)
{
	HAL_StatusTypeDef DMA_status = HAL_ERROR; 
	/* 使能DMA时钟 */
	DMA_STREAM_CLOCK();

	DMA_Handle.Instance = DMA_STREAM;
	/* DMA数据流通道选择 */
	DMA_Handle.Init .Channel = DMA_CHANNEL;  
	/* 存储器到存储器模式 */
	DMA_Handle.Init.Direction = DMA_MEMORY_TO_MEMORY;
	/* 使能自动递增功能 */
	DMA_Handle.Init.PeriphInc = DMA_PINC_ENABLE;
	/* 使能自动递增功能 */
	DMA_Handle.Init.MemInc = DMA_MINC_ENABLE;
	/* 源数据是字大小(32位) */
	DMA_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	/* 目标数据也是字大小(32位) */
	DMA_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	/* 一次传输模式，存储器到存储器模式不能使用循环传输 */
	DMA_Handle.Init.Mode = DMA_NORMAL;
	/* DMA数据流优先级为高 */
	DMA_Handle.Init.Priority = DMA_PRIORITY_HIGH;
	/* 禁用FIFO模式 */
	DMA_Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;     
	DMA_Handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	/* 单次模式 */
	DMA_Handle.Init.MemBurst = DMA_MBURST_SINGLE;
	/* 单次模式 */
	DMA_Handle.Init.PeriphBurst = DMA_PBURST_SINGLE;
	/* 完成DMA数据流参数配置 */
	HAL_DMA_Init(&DMA_Handle);

	DMA_status = HAL_DMA_Start(&DMA_Handle,(uint32_t)aSRC_Const_Buffer,(uint32_t)aDST_Buffer,BUFFER_SIZE);
	/* 判断DMA状态 */
	if (DMA_status != HAL_OK)
	{
		/* DMA出错就让程序运行下面循环：RGB彩色灯闪烁 */
		while (1)
		{      
			LED_RED;
			Delay(0xFFFFFF);
			LED_RGBOFF;
			Delay(0xFFFFFF);
		}
	} 
	
	
	

	
}

