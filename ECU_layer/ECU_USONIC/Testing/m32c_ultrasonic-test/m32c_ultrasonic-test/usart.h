/*
 * usart.h
 *
 * Created: 24/01/2019 10:02:47
 *  Author: Mahmoud
 */ 


#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#define uint8_t unsigned int
void usart_init (void );
void usart_send(unsigned char ch);
void usart_send_arr(uint8_t *val);
unsigned char usart_recve(void);




#endif /* USART_H_ */