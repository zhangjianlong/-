#include  "delay.h"

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
/* 简单的延时函数 */


 void Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}
