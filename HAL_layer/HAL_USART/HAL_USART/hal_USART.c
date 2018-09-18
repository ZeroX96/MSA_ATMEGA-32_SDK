/*
 * hal_USART.c
 *
 * Created: 11/09/2018 20:59:27
 *  Author: Mahmoud
 */ 
#include "hal_USART.h"

static void (*vptr_TXC_cb)(void);
static void (*vptr_RXC_cb)(void);

//DONT_TOUCH_WHAT'S_NEXT**********************************************************************************
//DONT_TOUCH_WHAT'S_NEXT**********************************************************************************
//DONT_TOUCH_WHAT'S_NEXT**********************************************************************************
//DONT_TOUCH_WHAT'S_NEXT**********************************************************************************

#ifdef UART
///*(volatile msa_u8 *)0x40 &= ~(1<<UMSEL);	//activating asynchronous mode
#ifdef NORMAL_ASYNCH
msa_u32 UBRR_VAL =( ((msa_u32)F_CPU/((msa_u32)16*BAUD_VAL)) -1) ; //normal speed equ  >>The URSEL must be zero when writing the UBRRH
#else  //DOUBLE_SPEED
msa_u32 UBRR_VAL =( ((msa_u32)F_CPU/((msa_u32)8*BAUD_VAL)) -1) ;//double speed equ  >>The URSEL must be zero when writing the UBRRH
(*(volatile msa_u8*)0x2b) |=(1<<U2X);	//enabling the double speed mode
#endif //norm or double speed asynch

#endif  //end of uart

#ifdef USRT
(*(volatile msa_u8*)0x40) |=(1<<URSEL)|(1<<UMSEL);	//activating synchronous mode
#ifdef MASTER_SYNCHRONOUS
msa_u32 UBRR_VAL =( ((msa_u32)F_CPU/((msa_u32)2*BAUD_VAL)) -1) ; //synchronous master equ >>The URSEL must be zero when writing the UBRRH
#endif //end of master_synch


#endif //end of usrt



usart_ret_types usart_init(	usart_t*  			usart_obj,operations_mode_t	operations_mode,
							stop_bits_no_t		stop_bits_no,parity_mode_t		parity_mode,
							data_bits_no_t		data_bits_no,notfications_mode_t notifications_mode,
							clock_polarity_t	clock_polarity
						  )
{
	usart_ret_types ret_val=NO_USART_ERRORS;
if (		 (usart_obj != NULL)														&& 
			((operations_mode>=A_SENDER) && (operations_mode<=B_SENDER_N_RECEIVER))		&&
			((stop_bits_no==ONE_STP_BIT) || (stop_bits_no==TWO_STP_BITS))				&&
			((parity_mode >= NO_PARITY) && (parity_mode <=ODD_PARITY))					&&
			((data_bits_no >=FIVE) && (data_bits_no <=NINE))							&&
			((notifications_mode== INTERRUPT_DRIVEN) || (notifications_mode == POLLING_DRIVEN))
	)
	{
		if (notifications_mode == INTERRUPT_DRIVEN)
		{
			cli();
			if (operations_mode == A_SENDER)
			{
				(*(volatile msa_u8*)0x2a) |=(1<<UDRIE);
			} 
			else if (operations_mode == A_RECEIVER)
			{
				(*(volatile msa_u8*)0x2a) |=(1<<RXCIE);
			}
			else
			{
				(*(volatile msa_u8*)0x2a) |=(1<<RXCIE)|(1<<UDRIE);	
			}
		}
		
		usart_obj->obj_data_bits_no=data_bits_no;
		usart_obj->obj_device_state=INITIATED;
		usart_obj->obj_notifications_mode=notifications_mode;
		usart_obj->obj_operations_mode=operations_mode;
		usart_obj->obj_parity_mode=parity_mode;
		usart_obj->obj_stop_bits_no=stop_bits_no;
		
		(*(volatile msa_u8*)0x2a) |=(operations_mode << TXEN)|( (data_bits_no & 0x04)<<UCSZ2);
		UCSRC=(1<<URSEL)|(stop_bits_no<<USBS)|((data_bits_no & 0x03)<<UCSZ0 )|(parity_mode << UPM0)|(clock_polarity<<UCPOL);
		(*(volatile msa_u8*)0x29)  =(msa_u8) UBRR_VAL;
		(*(volatile msa_u8*)0x40)  =(msa_u8) ((UBRR_VAL>>8)&(0x7f));
	} 
	else
	{
		ret_val =INVALID_PARAMS;
	}
	
	if (notifications_mode == INTERRUPT_DRIVEN)
	{sei();}//if interrupt driven the global interrupt must be disabled and then enabled at the end of the initiation
	
	return ret_val;
}
usart_ret_types usart_receive_byte(usart_t * usart_obj,msa_u8* byte)
{
	usart_ret_types ret_val=NO_USART_ERRORS;
	if ( (usart_obj != NULL) && (byte != NULL) )
	{
		if (usart_obj->obj_device_state == INITIATED)
		{
	
			while(!((*(volatile msa_u8*)0x2b) & (1<<RXC)))
			;
			*byte=(*(volatile msa_u8*)0x2c);
			(*(volatile msa_u8*)0x2b)|=(1<<RXC);
		}
		else
		{
			ret_val=NOT_INITIATED;
		}
	}
	else
	{
	ret_val=INVALID_PARAMS;
	}
	return ret_val;
}

usart_ret_types usart_send_byte(usart_t * usart_obj,msa_u8* byte)
{
	usart_ret_types ret_val=NO_USART_ERRORS;
	if ( (usart_obj != NULL) && (byte != NULL) )
	{
		if (usart_obj->obj_device_state == INITIATED)
		{
			
			while ( !( (*(volatile msa_u8*)0x2b) & (1<<UDRE) ) )
			;
				(*(volatile msa_u8*)0x2c)=*byte;
				
			
		} 
		else
		{
			ret_val=NOT_INITIATED;
		} 
	} 
	else
	{
		ret_val=INVALID_PARAMS;
	}
	return ret_val;
	
}

usart_ret_types usart_send_arr(usart_t* usart_obj,uint8_t *arr_add)
{
	usart_ret_types ret_val=NO_USART_ERRORS;
	if ( (usart_obj != NULL) && (arr_add != NULL) )
	{
		if (usart_obj->obj_device_state == INITIATED)
		{
			msa_u8 i=0;
			for (i=0;*(arr_add+i) ;i++)
			{
				while ( !( (*(volatile msa_u8*)0x2b) & (1<<UDRE) ) )
				;
				(*(volatile msa_u8*)0x2c)=*(arr_add+i);
			}
		}
		else
		{
			ret_val=NOT_INITIATED;
		}
	}
	else
	{
		ret_val=INVALID_PARAMS;
	}


	return ret_val;
}

usart_ret_types usart_receive_arr(usart_t * usart_obj,msa_u8* arr_add,msa_u8 arr_size)
{
	usart_ret_types ret_val=NO_USART_ERRORS;
	msa_u8 data_in_cntr=0,temp=0;
	
	if ( (usart_obj != NULL) && (arr_add != NULL) )
	{
		if (usart_obj->obj_device_state == INITIATED)
		{
			while( (temp != 13) && (data_in_cntr < (arr_size) ) )
			{
				usart_receive_byte(usart_obj,&temp);
				arr_add[data_in_cntr++]=temp;
				if(temp == '\0')
				break;
			}
			arr_add[data_in_cntr]=0;
		}
		else
		{
			ret_val=NOT_INITIATED;
		}
	}
	else
	{
		ret_val=INVALID_PARAMS;
	}
	return ret_val;
}


usart_ret_types usart_set_isr_RXC_callback(usart_t * usart_obj,void (*vptr_cb)(void))
{
	usart_ret_types ret_val=NO_USART_ERRORS;
	if ( (usart_obj != NULL) && (vptr_cb != NULL) )
	{
		if (usart_obj->obj_device_state == INITIATED)
		{
			usart_obj->obj_vptr_RXC_cb=vptr_cb;
			vptr_RXC_cb=vptr_cb;
			
		}
		else
		{
			ret_val=NOT_INITIATED;
		}
	}
	else
	{
		ret_val=INVALID_PARAMS;
	}
	PORTA=0xff;
	return ret_val;
	
	
}


usart_ret_types usart_set_isr_TXC_callback(usart_t * usart_obj,void (*vptr_cb)(void))
{
	usart_ret_types ret_val=NO_USART_ERRORS;
	if ( (usart_obj != NULL) && (vptr_cb != NULL) )
	{
		if (usart_obj->obj_device_state == INITIATED)
		{
			usart_obj->obj_vptr_TXC_cb=vptr_cb;
			vptr_TXC_cb=vptr_cb;

		}
		else
		{
			ret_val=NOT_INITIATED;
		}
	}
	else
	{
		ret_val=INVALID_PARAMS;
	}
	PORTA=0xff;
	return ret_val;
	
	
}

ISR(USART_RXC_vect)
{
	if (vptr_RXC_cb)
	{
		vptr_RXC_cb();
	}
		
}

ISR(USART_UDRE_vect)
{
	if (vptr_TXC_cb)
	{
		vptr_TXC_cb();
	}	
}