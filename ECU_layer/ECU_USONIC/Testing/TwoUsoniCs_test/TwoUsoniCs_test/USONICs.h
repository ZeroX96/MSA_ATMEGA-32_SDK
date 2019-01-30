/*
 * USONICs.h
 *
 * Created: 30/11/2018 04:38:54
 *  Author: Mahmoud
 */ 


#ifndef USONICS_H_
#define USONICS_H_
#include "INCLUDES.h"
#define OVF_VAL 350 //the val. of every overflow occurrence

//static uint8_t overFlow_cntr=0;//enable the ov isr and just inc this global every ov isr and reset it whenever the counter val is read
uint8_t read_ultra_sonic0(void);

void send_usonic0(uint16_t COUNTA);

uint8_t read_ultra_sonic1(void);

void send_usonic1(uint16_t COUNTB);





#endif /* USONICS_H_ */