/*
 * SevenSegApp.c
 *
 * Created: 4/15/2020 8:45:00 AM
 *  Author: Mahmo
 */ 
#include "SevenSegApp.h"
extern uint8_t SysHours;
extern uint8_t SysMinuts;

static SevenSegT SysDisplay;

void DisplayMuxAppInit(){
	DisplayMuxInit(&SysDisplay,BASE_B,BASE_D,DISP_COMMON_CATHODE);
}

void DisplayMuxAppShow(){
	static uint8_t OutDataVal[4];
	OutDataVal[0] = SysMinuts % 10;
	OutDataVal[1] = SysMinuts / 10;
	
	OutDataVal[2] = SysHours % 10;
	OutDataVal[3] = SysHours / 10;
	DisplayMuxWrite(&SysDisplay,OutDataVal);
}

void DisplayMuxAppReset(){
	DisplayMuxReset(&SysDisplay);
}