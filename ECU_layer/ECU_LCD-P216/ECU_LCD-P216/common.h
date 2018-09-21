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
typedef tenum_bool 			bool_t;
typedef unsigned char 		msa_u8;
typedef unsigned short int 	msa_u16;
typedef unsigned long int 	msa_u32;
typedef signed char 		msa_s8;
typedef signed short int 	msa_s16;
typedef signed long int 	msa_s32;
#endif /* COMMON_H_ */