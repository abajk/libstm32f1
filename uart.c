#include "stm32f10x.h"

#include "uart.h"

extern void UART_Init(void){
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;
	
	GPIOA->CRH |= GPIO_CRH_CNF9_1 |GPIO_CRH_MODE9_1;
	
	USART1->CR1 = USART_CR1_UE | USART_CR1_TE;
	USART1->BRR = (SystemCoreClock / 115200);
}

extern void UART_Init_DMA(void){
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN |
									RCC_APB2ENR_AFIOEN |
									RCC_APB2ENR_USART1EN;
	
	GPIOA->CRH |= GPIO_CRH_CNF9_1 |GPIO_CRH_MODE9_1;	
	
	USART1->BRR = (SystemCoreClock / 115200);
	
	USART1->CR1 = USART_CR1_UE | USART_CR1_TE;	
	
	USART1->CR3 |= USART_CR3_DMAT;
	
    // set up DMA1 channel 4 for data transfer
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	// disable channel completely
	DMA1_Channel4->CCR = 0;
	// UART address
	DMA1_Channel4->CPAR = (uint32_t)&USART1->DR;
	// other params
	DMA1_Channel4->CCR = 	DMA_CCR4_DIR |  	// memory->SPI
												DMA_CCR4_MINC | 	// memory increment
												DMA_CCR4_PL_1 |		// priority high
												DMA_CCR4_TCIE;		// transfer complete irq
	
	NVIC_EnableIRQ(DMA1_Channel4_IRQn);
	NVIC_SetPriority(DMA1_Channel4_IRQn,14);	
}

extern void UART_Init_DMA_Block(void){
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN |
									RCC_APB2ENR_AFIOEN |
									RCC_APB2ENR_USART1EN;
	
	GPIOA -> CRH |= GPIO_CRH_CNF9_1 |GPIO_CRH_MODE9_1;	
	
	USART1->BRR = (SystemCoreClock / 115200);
	
	USART1->CR1 = USART_CR1_UE | USART_CR1_TE;	
	
	USART1->CR3 |= USART_CR3_DMAT;
	
		// set up DMA1 channel 4 for data transfer
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	// disable channel completely
	DMA1_Channel4->CCR = 0;
	// UART address
	DMA1_Channel4->CPAR = (uint32_t)&USART1->DR;
	// other params
	DMA1_Channel4->CCR = 	DMA_CCR4_DIR |  	// memory->SPI
												DMA_CCR4_MINC | 	// memory increment
												DMA_CCR4_PL_1;		// priority high
}

extern void UART_Tx(uint8_t *buf, uint16_t buf_len){
	do{
		while(!(USART1->SR & USART_SR_TXE));
		USART1->DR = *buf++;
	} while(--buf_len);
	while(!(USART1->SR & USART_SR_TC));	
}

extern void UART_Tx_DMA(uint8_t *buf, uint16_t buf_len){
	DMA1_Channel4->CMAR = (uint32_t)buf;
	DMA1_Channel4->CNDTR = buf_len;	
	DMA1_Channel4->CCR |= DMA_CCR4_EN;	
}

extern void UART_Tx_DMA_Block(uint8_t *buf, uint16_t buf_len){
	DMA1_Channel4->CMAR = (uint32_t)buf;
	DMA1_Channel4->CNDTR = buf_len;	
	DMA1_Channel4->CCR |= DMA_CCR4_EN;
	while(!(USART1->SR & USART_SR_TC));
	DMA1_Channel4->CCR &= ~DMA_CCR4_EN;	
}

void DMA1_Channel4_IRQHandler(void){
	DMA1->IFCR|=DMA_IFCR_CTCIF4;
	DMA1_Channel4->CCR &= ~DMA_CCR4_EN;
}
