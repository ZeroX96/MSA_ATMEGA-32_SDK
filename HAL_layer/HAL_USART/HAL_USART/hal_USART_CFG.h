/*
 * hal_USART_CFG.h
 *
 * Created: 11/09/2018 21:00:12
 *  Author: Mahmoud
 */ 


#ifndef HAL_USART_CFG_H_
#define HAL_USART_CFG_H_

//change these settings as needed.if not changed,these are Z{//default settings-------|
//																					  |
//the operating mode																  |
#define UART  //options for this driver {UART or USRT}								  |	
//																					  |	
//the operating frequency															  |
#define F_CPU 8000000UL														//		  |
//																					  |
//the required baudRate																  |
#define BAUD_VAL 9600				//												  |
//																					  |
#ifdef UART				//															  |
#define NORMAL_ASYNCH //options for uart {normal_speed or double_speed}				  |
//																					  |
//																					  |
//																					  |
//																					  |
#endif //UART																		  |
//																					  |
//																					  |
#ifdef  USRT																		//  |
#define  MASTER_SYNCH //options for usrt {master sychronous or slave synchronous}	  |
//																					  |
//																					  |
#endif //USRT																		  |
//end of the default settings								}//default settings-------|




#endif /* HAL_USART_CFG_H_ */