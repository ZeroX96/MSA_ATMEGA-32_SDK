/*
 * seg_disp.h
 *
 *  Created on: 10 Jul 2018
 *      Author: Mahmoud
 */

#ifndef SEG_DISP_H_
#define SEG_DISP_H_
#include "common.h"

#define MAX_DISP_NO  5
#define INPUT_OFFSET 0
#define DIRECTION_OFFSET 1
#define OUTPUT_OFFSET 2
#define MAX_POSSIBLE_VALS 10


typedef enum {
	_COMMON_ANODE,
	_COMMON_CATHODE,
	_NO_TYPE,
}disp_types;

typedef enum {
	_MIN_BASE=0X2F,
	_BASE_D=0X30,
	_BASE_C=0X33,
	_BASE_B=0X36,
	_BASE_A=0X39,
	_MAX_BASE,
}port_bases_t;

typedef enum {
	_NO_ERRORS=0,							//0
	_INVALID_HANDLER_OR_BASE_PARAMETERS,	//1
	_INVALID_TYPE_PARAMETERS,				//2
	_MAX_NO_REACHED,						//3
	_INVALID_HANDLER_OR_VAL_PARMETERS,		//4
	_MODULE_NOT_INITIALIZED,
}seg_errors_t;


seg_errors_t disp_init(port_bases_t port_base,disp_types disp_type,ecore_u8 *handler);
seg_errors_t disp_out(ecore_u8 *handler,ecore_u8 disp_val);
seg_errors_t disp_reset(ecore_u8 *handler);
seg_errors_t disp_deinit(ecore_u8 *handler);

#endif /* SEG_DISP_H_ */
