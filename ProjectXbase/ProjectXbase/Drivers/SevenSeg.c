/*
 * SevenSeg.c
 *
 * Created: 3/20/2020 5:40:14 AM
 *  Author: Mahmo
 */ 
#include "SevenSeg.h"

static uint8_t DispPatterns[MAX_POSSIBLE_VAL] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
	 
 SevenSegRetT DisplayInit(SevenSegT* Display,GpioBaseT DispPort,SevenSegTypeT DispType){
	if (NULL != Display){
		Display->DataBase = DispPort;
		Display->Type = DispType;
		GpioInitPort(DispPort,PORT_OUT);
		return SEVENSEG_ERR_NO;
	}
	return SEVENSEG_ERR_INV_PARAMS;
}
 SevenSegRetT DisplayWrite(SevenSegT* Display,uint8_t OutVal){
	 if( (NULL != Display)&&(OutVal < MAX_POSSIBLE_VAL) ){
		 if (Display->Type == DISP_COMMON_CATHODE){
			 GpioWritePort(Display->DataBase,DispPatterns[OutVal]);
		 } 
		 else{
			 GpioWritePort(Display->DataBase,~(DispPatterns[OutVal]) );
		 }
		 return SEVENSEG_ERR_NO;
	 }
	 return SEVENSEG_ERR_INV_PARAMS;
 }
 SevenSegRetT DisplayReset(SevenSegT* Display){
	 if (NULL != Display){
		 if (Display->Type == DISP_COMMON_CATHODE){
			 GpioWritePort(Display->DataBase,DispPatterns[0]);
		 }
		 else{
			 GpioWritePort(Display->DataBase,~(DispPatterns[0]) );
		 }
		 return SEVENSEG_ERR_NO;
	 }
	 return SEVENSEG_ERR_INV_PARAMS;
 }
 SevenSegRetT DisplayDeInit(SevenSegT* Display){
	 if (NULL != Display){
		 if (Display->Type == DISP_COMMON_CATHODE){
			 GpioWritePort(Display->DataBase,DispPatterns[0]);
		 }
		 else{
			 GpioWritePort(Display->DataBase,~(DispPatterns[0]) );
		 }
		 Display->DataBase = 0x00;
		 return SEVENSEG_ERR_NO;
	 }
	 return SEVENSEG_ERR_INV_PARAMS;
 }
 
//multiplexed displays
SevenSegRetT DisplayMuxInit(SevenSegT* Display,GpioBaseT DataPort,GpioBaseT CtrlPort,SevenSegTypeT DispType)
{
	Display->CtrlBase = CtrlPort;
	Display->DataBase = DataPort;
	Display->Type = DispType;
	DisplayInit(Display,DataPort,DispType);
	for(uint8_t i =SSD_STARTING_CTRL_BIT;i < SSD_STARTING_CTRL_BIT + MUX_NUMBER_OF_DISPS; i++){
		GpioInitPin(CtrlPort,i,PIN_OUT);	
	}
	return SEVENSEG_ERR_NO;
}
SevenSegRetT DisplayMuxWrite(SevenSegT* Display,uint8_t OutDataVal[]){
	
	DisplayWrite(Display,OutDataVal[0]);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+0,PIN_HIGH);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+1,PIN_LOW);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+2,PIN_LOW);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+3,PIN_LOW);
	_delay_ms(5);
	DisplayWrite(Display,OutDataVal[1]);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+0,PIN_LOW);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+1,PIN_HIGH);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+2,PIN_LOW);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+3,PIN_LOW);
	_delay_ms(5);
	DisplayWrite(Display,OutDataVal[2]);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+0,PIN_LOW);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+1,PIN_LOW);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+2,PIN_HIGH);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+3,PIN_LOW);
	_delay_ms(5);
	DisplayWrite(Display,OutDataVal[3]);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+0,PIN_LOW);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+1,PIN_LOW);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+2,PIN_LOW);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+3,PIN_HIGH);
	_delay_ms(5);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+0,PIN_LOW);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+1,PIN_LOW);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+2,PIN_LOW);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+3,PIN_LOW);
	return SEVENSEG_ERR_NO;
}
SevenSegRetT DisplayMuxReset(SevenSegT* Display){
	
	DisplayWrite(Display,0);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+0,PIN_HIGH);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+1,PIN_HIGH);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+2,PIN_HIGH);
	GpioWritePin(Display->CtrlBase,SSD_STARTING_CTRL_BIT+3,PIN_HIGH);
	return SEVENSEG_ERR_NO;
}
SevenSegRetT DisplayMuxDeInit(SevenSegT* Display){
	DisplayMuxReset(Display);
	DisplayDeInit(Display);
	Display->CtrlBase = 0x00;
	return SEVENSEG_ERR_NO;
}