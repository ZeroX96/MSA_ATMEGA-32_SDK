/*
 * seg_disp.c
 *
 *  Created on: 10 Jul 2018
 *      Author: Mahmoud
 */

#include "seg_disp.h"


typedef struct{
	disp_types Disp_type;
	port_bases_t Port_base;
}handler_data;




static handler_data a_str_handler_info[MAX_DISP_NO];
static ecore_u8 handler_counter=0;
static ecore_u8 disp_patterns[MAX_POSSIBLE_VALS]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
	
seg_errors_t disp_init(port_bases_t port_base,disp_types disp_type,ecore_u8 *handler)
{
	seg_errors_t b_ret_val=_NO_ERRORS;
	if(   ((port_base > _MIN_BASE) && (port_base < _MAX_BASE) )&&(handler != NULL))
	{
		if(handler_counter < MAX_DISP_NO)
		{
			if(((a_str_handler_info[handler_counter].Disp_type ==_COMMON_ANODE )||(a_str_handler_info[handler_counter].Disp_type ==_COMMON_CATHODE))&&(a_str_handler_info[handler_counter].Port_base != 0) )
			{
				while(((a_str_handler_info[handler_counter].Disp_type ==_COMMON_ANODE )||(a_str_handler_info[handler_counter].Disp_type ==_COMMON_CATHODE))&&(a_str_handler_info[handler_counter].Port_base != 0))
					handler_counter++;
			}
			*(volatile ecore_u8 *)(port_base+DIRECTION_OFFSET)=0xff;
			a_str_handler_info[handler_counter].Disp_type=disp_type;
			a_str_handler_info[handler_counter].Port_base =port_base;
			*handler=handler_counter;
			handler_counter++;
		}
		else
		{
			b_ret_val=_MAX_NO_REACHED;
		}
	}
	else
	{
		b_ret_val=_INVALID_HANDLER_OR_BASE_PARAMETERS;
	}

	return b_ret_val;
}

seg_errors_t disp_out(ecore_u8 *handler,ecore_u8 disp_val)
{
	seg_errors_t b_ret_val=_NO_ERRORS;

	if ((handler != NULL)&&(*handler <MAX_DISP_NO)&&(disp_val < MAX_POSSIBLE_VALS))
	{
		if(a_str_handler_info[*handler].Port_base != 0)
		{
			if(a_str_handler_info[*handler].Disp_type == _COMMON_CATHODE)
			{
				*(volatile ecore_u8 *)(a_str_handler_info[*handler].Port_base + OUTPUT_OFFSET)=disp_patterns[disp_val];
			}
			else if(a_str_handler_info[*handler].Disp_type == _COMMON_ANODE )
			{
				*(volatile ecore_u8 *)(a_str_handler_info[*handler].Port_base + OUTPUT_OFFSET)=~(disp_patterns[disp_val]);
			}
			else
			{
				b_ret_val=_MODULE_NOT_INITIALIZED;
			}
		}
		else
		{
			b_ret_val=_MODULE_NOT_INITIALIZED;
		}

	}
	else
	{
		b_ret_val=_INVALID_HANDLER_OR_VAL_PARMETERS;
	}
return b_ret_val;
}


seg_errors_t disp_reset(ecore_u8 *handler)//writes zero on the display
{
	seg_errors_t b_ret_val=_NO_ERRORS;
	if((handler != NULL)&&(*handler < MAX_DISP_NO))
	{
		if(a_str_handler_info[*handler].Port_base != 0)
		{
			if(a_str_handler_info[*handler].Disp_type == _COMMON_CATHODE)
			{
				*(volatile ecore_u8 *)(a_str_handler_info[*handler].Port_base+OUTPUT_OFFSET)=disp_patterns[0];
			}
			else if(a_str_handler_info[*handler].Disp_type == _COMMON_ANODE)
			{
				*(volatile ecore_u8*)(a_str_handler_info[*handler].Port_base+OUTPUT_OFFSET)=~(disp_patterns[0]);
			}
			else
			{
				b_ret_val =_MODULE_NOT_INITIALIZED;
			}

		}
		else
		{
			b_ret_val =_MODULE_NOT_INITIALIZED;
		}
	}
	else
	{
		b_ret_val=_INVALID_HANDLER_OR_BASE_PARAMETERS;
	}
	return b_ret_val;
}



seg_errors_t disp_deinit(ecore_u8 *handler)//remove the object from the handler
{
	seg_errors_t b_ret_val=_NO_ERRORS;
	if((handler != NULL)&&(*handler < MAX_DISP_NO))
	{
		*(volatile ecore_u8 *)(a_str_handler_info[*handler].Port_base+DIRECTION_OFFSET)=0x00;
		a_str_handler_info[*handler].Disp_type=_NO_TYPE;
		a_str_handler_info[*handler].Port_base=0x00;
		handler_counter=*handler;
	}
	else
	{
		b_ret_val =_INVALID_HANDLER_OR_BASE_PARAMETERS;
	}


	return b_ret_val;
}
