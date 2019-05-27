#ifndef UART_H
#define UART_H

#include <stdint.h>

extern void UART_Init(void);
extern void UART_Init_DMA(void);
extern void UART_Init_DMA_Block(void);
extern void UART_Tx(uint8_t *buf, uint16_t buf_len);
extern void UART_Tx_DMA(uint8_t *buf, uint16_t buf_len);
extern void UART_Tx_DMA_Block(uint8_t *buf, uint16_t buf_len);

#endif
