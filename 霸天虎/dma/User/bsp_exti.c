
#include "bsp_exti.h"
#include "./led/bsp_led.h"
#include  "delay.h"
/* ��غ궨�壬ʹ�ô洢�����洢���������ʹ��DMA2 */
DMA_HandleTypeDef DMA_Handle;

#define DMA_STREAM               DMA2_Stream0
#define DMA_CHANNEL              DMA_CHANNEL_0
#define DMA_STREAM_CLOCK()       __DMA2_CLK_ENABLE()

/* ����aSRC_Const_Buffer������ΪDMA��������Դ
  const�ؼ��ֽ�aSRC_Const_Buffer�����������Ϊ�������� */
  
#define BUFFER_SIZE              32

/* ����aSRC_Const_Buffer������ΪDMA��������Դ
  const�ؼ��ֽ�aSRC_Const_Buffer�����������Ϊ�������� */
const uint32_t aSRC_Const_Buffer[BUFFER_SIZE]= {
                                    0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
                                    0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
                                    0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
                                    0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
                                    0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
                                    0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
                                    0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
                                    0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80};
/* ����DMA����Ŀ��洢�� */
uint32_t aDST_Buffer[BUFFER_SIZE];

static void SystemClock_Config(void);
uint8_t Buffercmp(const uint32_t* pBuffer, uint32_t* pBuffer1, uint16_t BufferLength);
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
									
									

void m_m_dma(void){
		/* �����űȽϽ������ */
	uint8_t TransferStatus;
	// �ȴ�dma�������
	while(__HAL_DMA_GET_FLAG(&DMA_Handle,DMA_FLAG_TCIF0_4) == DISABLE){
		
		
	}
	//�Ƚ�Ԫ�����ڴ��������
	TransferStatus = Buffercmp(aSRC_Const_Buffer,aDST_Buffer,BUFFER_SIZE);
		/* �ж�Դ�����봫������ݱȽϽ��*/
	if(TransferStatus==0)  
	{
		/* Դ�����봫������ݲ����ʱRGB��ɫ����ʾ��ɫ */
		LED_RED;
	}
	else
	{ 
		/* Դ�����봫����������ʱRGB��ɫ����ʾ��ɫ */
		LED_BLUE;
	}
	
}




//�����ж�
void key_exit_init(void){
	//gpio ��ʼ��
	GPIO_InitTypeDef gpio_init;	
	//��ʼ��GPIOʱ��
	KEY1_INT_GPIO_CLK_ENABLE();
	KEY2_INT_GPIO_CLK_ENABLE();
	//��ʼ��GPIO
	gpio_init.Pin = KEY1_INT_GPIO_PIN;
	 /* ��������Ϊ����ģʽ */ 
	gpio_init.Mode  = GPIO_MODE_IT_RISING;
	  /* �������Ų�����Ҳ������ */
    gpio_init.Pull = GPIO_NOPULL;
	//��ʼ��GPIO
	HAL_GPIO_Init(KEY1_INT_GPIO_PORT,&gpio_init);
	 /* ���� EXTI �ж�Դ ��key1 ���š������ж����ȼ�*/
	HAL_NVIC_SetPriority(KEY1_INT_EXTI_IRQ,0,0);
	 /* ʹ���ж� */
	HAL_NVIC_EnableIRQ(KEY1_INT_EXTI_IRQ);
	
	
}
void DMA_Config(void)
{
	HAL_StatusTypeDef DMA_status = HAL_ERROR; 
	/* ʹ��DMAʱ�� */
	DMA_STREAM_CLOCK();

	DMA_Handle.Instance = DMA_STREAM;
	/* DMA������ͨ��ѡ�� */
	DMA_Handle.Init .Channel = DMA_CHANNEL;  
	/* �洢�����洢��ģʽ */
	DMA_Handle.Init.Direction = DMA_MEMORY_TO_MEMORY;
	/* ʹ���Զ��������� */
	DMA_Handle.Init.PeriphInc = DMA_PINC_ENABLE;
	/* ʹ���Զ��������� */
	DMA_Handle.Init.MemInc = DMA_MINC_ENABLE;
	/* Դ�������ִ�С(32λ) */
	DMA_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	/* Ŀ������Ҳ���ִ�С(32λ) */
	DMA_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	/* һ�δ���ģʽ���洢�����洢��ģʽ����ʹ��ѭ������ */
	DMA_Handle.Init.Mode = DMA_NORMAL;
	/* DMA���������ȼ�Ϊ�� */
	DMA_Handle.Init.Priority = DMA_PRIORITY_HIGH;
	/* ����FIFOģʽ */
	DMA_Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;     
	DMA_Handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	/* ����ģʽ */
	DMA_Handle.Init.MemBurst = DMA_MBURST_SINGLE;
	/* ����ģʽ */
	DMA_Handle.Init.PeriphBurst = DMA_PBURST_SINGLE;
	/* ���DMA�������������� */
	HAL_DMA_Init(&DMA_Handle);

	DMA_status = HAL_DMA_Start(&DMA_Handle,(uint32_t)aSRC_Const_Buffer,(uint32_t)aDST_Buffer,BUFFER_SIZE);
	/* �ж�DMA״̬ */
	if (DMA_status != HAL_OK)
	{
		/* DMA������ó�����������ѭ����RGB��ɫ����˸ */
		while (1)
		{      
			LED_RED;
			Delay(0xFFFFFF);
			LED_RGBOFF;
			Delay(0xFFFFFF);
		}
	} 
	
	
	

	
}

