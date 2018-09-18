/*
 * common.h
 *
 * Created: 02/08/2018 17:52:27
 *  Author: Mahmoud
 */ 


#ifndef COMMON_H_
#define COMMON_H_

typedef enum{
	E_FALSE =0,
	E_TRUE,
}tenum_bool;

#define NULL ((void *)0)
typedef tenum_bool bool_t;
typedef unsigned char ecore_u8;
typedef unsigned short int ecore_u16;
typedef unsigned long int ecore_u32;
typedef signed char ecore_s8;
typedef signed short int ecore_s16;
typedef signed long int ecore_s32;
#endif /* COMMON_H_ */