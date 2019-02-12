/*
 * ecu_CAN_CFG.h
 *
 * Created: 29/01/2019 12:47:45
 *  Author: Mahmoud
 */ 


#ifndef ECU_CAN_CFG_H_
#define ECU_CAN_CFG_H_
//config which INT_PIN of the AT32 MCU will be responsible for the can interrupts
#define ISR_NAME INT0_vect	//edit if used isr 1	//for isr-2 in at32 wont work as it's level_sensing only
#define ISR_NO INTERRUPT0	//edit if used isr 1
//MODES 0,0 AND 1,1
//with data being clocked in on the rising edge,Data is driven out by the MCP2515 on the falling edge of SCK.
//MSB FIRST


#endif /* ECU_CAN_CFG_H_ */