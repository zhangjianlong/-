#ifndef __USART_DMA_H
#define	__USART_DMA_H

#include "stm32f4xx.h"
#include <stdio.h>

//���ڲ�����
#define DEBUG_USART_BAUDRATE                    115200
//���Ŷ���
/*******************************************************/
#define DEBUG_USART                             USART1
#define DEBUG_USART_CLK_ENABLE()                __USART1_CLK_ENABLE();

#define DEBUG_USART_RX_GPIO_PORT                GPIOA
#define DEBUG_USART_RX_GPIO_CLK_ENABLE()        __GPIOA_CLK_ENABLE()
#define DEBUG_USART_RX_PIN                      GPIO_PIN_10
#define DEBUG_USART_RX_AF                       GPIO_AF7_USART1

#define DEBUG_USART_TX_GPIO_PORT                GPIOA
#define DEBUG_USART_TX_GPIO_CLK_ENABLE()        __GPIOA_CLK_ENABLE()
#define DEBUG_USART_TX_PIN                      GPIO_PIN_9
#define DEBUG_USART_TX_AF                       GPIO_AF7_USART1

#define DEBUG_USART_IRQHandler                  USART1_IRQHandler
#define DEBUG_USART_IRQ                 		    USART1_IRQn
/************************************************************/
//DMA
#define SENDBUFF_SIZE                     		10//���͵�������
#define DEBUG_USART_DMA_CLK_ENABLE()      		__DMA2_CLK_ENABLE()	
#define DEBUG_USART_DMA_CHANNEL           		DMA_CHANNEL_4
#define DEBUG_USART_DMA_STREAM            		DMA2_Stream7

void Debug_USART_Config(void);
void USART_DMA_Config(void);
void uart_dma_fun_test(void);
#endif /* __USART1_H */
