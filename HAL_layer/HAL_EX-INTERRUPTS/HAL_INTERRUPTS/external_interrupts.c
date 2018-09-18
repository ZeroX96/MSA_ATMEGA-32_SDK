/*
 * external_interrupts.c
 *
 * Created: 31/07/2018 12:53:11
 *  Author: Mahmoud
 */ 
#include "external_interrupts.h"

#define G_INTERRUPTS_msa 7

#define CONFIGED 1
#define NOT_CONFIGED 0

#ifndef ISC00
#define ISC00 0
#endif
#ifndef ISC01
#define ISC01 1
#endif
#ifndef ISC10
#define ISC10 2
#endif
#ifndef ISC11
#define ISC11 3
#endif
#ifndef ISC2
#define ISC2 6
#endif
static unsigned char isr_status=NOT_CONFIGED;

INT_ERRORS init_interrrupts(void)
{	
	INT_ERRORS ret_val=NO_ERRORS;
	if(isr_status == NOT_CONFIGED)
	{
	*((volatile unsigned char *)0x5f)|=(1<<G_INTERRUPTS_msa);			//Global Interrupt Enable
	isr_status=CONFIGED;
	}
	return ret_val;
}
INT_ERRORS set_interrupt(INT_NAMES int_name,INT_MODES int_mode)
{
	INT_ERRORS ret_val=NO_ERRORS;
	if ( isr_status == CONFIGED )
	{
		if ( ((int_name >= INTERRUPT2)&&(int_name <= INTERRUPT1)) && ((int_mode >= LOW_LEVEL)&&(int_mode <= RISING_EDGE)) )
		{
		
			*((volatile unsigned char *)0x5B)|=(1<<int_name);	//enabling Interrupt X
			if (int_name == INTERRUPT0)
			{
				if ( int_mode == FALLING_OR_RISING_EDGE)
				{
					*((volatile unsigned char *)0x55)|=(1<<ISC00);
				} 
				else if ( int_mode == FALLING_EDGE)
				{
					*((volatile unsigned char *)0x55)|=(1<<ISC01);
				} 
				else if (int_mode == RISING_EDGE)
				{
					*((volatile unsigned char *)0x55)|=(1<<ISC00)|(1<<ISC01);
				} 
				else
				{
					//doNothing cause the low level interrupt just puts nothing
					//or i must enable the pull up res!!!
					*((volatile unsigned char *)0x32)|=(1<<2); //enabling the pull up res for INT0 //###test
				}
			} 
			else if (int_name == INTERRUPT1)
			{
				if ( int_mode == FALLING_OR_RISING_EDGE)
				{
					*((volatile unsigned char *)0x55)|=(1<<ISC10);
				}
				else if ( int_mode == FALLING_EDGE)
				{
					*((volatile unsigned char *)0x55)|=(1<<ISC11);
				}
				else if (int_mode == RISING_EDGE)
				{
					*((volatile unsigned char *)0x55)|=(1<<ISC10)|(1<<ISC11);
				}
				else
				{
					//doNothing cause the low level interrupt just puts nothing
					//or i must enable the pull up res!!!
					*((volatile unsigned char *)0x32)|=(1<<3); //enabling the pull up res for INT1  //###test
				}
			} 
			else        //interrupt2
			{
				if (int_mode == FALLING_EDGE)
				{
					*((volatile unsigned char *)0x54)&=~(1<<ISC2);
				} 
				else if (int_mode == FALLING_EDGE)
				{
					*((volatile unsigned char *)0x54)=(1<<ISC2);
				}
				else
				{
					ret_val=INVALID_MODE_FOR_INT2;
				}
				
			}

			

		} 
		else
		{
			ret_val=INVALID_PARAMS;
		}
	} 
	else
	{
		ret_val=NOT_INITIALIZED;
	}

	return ret_val;
}
INT_ERRORS clr_interrupt(INT_NAMES int_name)
{
	INT_ERRORS ret_val=NO_ERRORS;
	if (isr_status == CONFIGED)
	{
			if ( (int_name >= INTERRUPT2)&&(int_name <= INTERRUPT1) )
			{
						*((volatile unsigned char *)0x5B)&=~(1<<int_name);	//disabling Interrupt X
			}
			else
			{
				ret_val=INVALID_PARAMS;
			}	
	}
	return ret_val;
}
INT_ERRORS deinit_interrrupts(void)
{
	INT_ERRORS ret_val=NO_ERRORS;
	if(isr_status == CONFIGED)
	{
		*((volatile unsigned char *)0x5f)&=~(1<<G_INTERRUPTS_msa);			//Global Interrupt Enable
		isr_status=NOT_CONFIGED;
	}
	return ret_val;
}
