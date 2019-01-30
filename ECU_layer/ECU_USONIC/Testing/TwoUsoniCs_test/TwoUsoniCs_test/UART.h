/*
 * UART.h
 *
 * Created: 30/11/2018 04:40:44
 *  Author: Mahmoud
 */ 


#ifndef UART_H_
#define UART_H_
#include "INCLUDES.h"
void usart_init (void );
void usart_send(unsigned char ch);
void usart_send_arr(uint8_t *val);
unsigned char usart_recve(void);

#endif /* UART_H_ */