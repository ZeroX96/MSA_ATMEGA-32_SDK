/*
 * KeyPad.c
 *
 * Created: 4/25/2020 11:29:49 PM
 *  Author: Mahmo
 */ 
#if 0
#include "KeyPad.h"

static uint8_t KeyPadBuffer[KPM_BUFFER_LENGTH] = {0};	//buffer used to hold the key presses
static uint8_t KeyPadStates[KPM_NUMB_COL][KPM_NUMB_ROW];
static uint8_t KeyPadDecode[KPM_NUMB_COL][KPM_NUMB_ROW]={{'1','2','3','A'},
														 {'4','5','6','B'},
														 {'7','8','9','C'},
														 {'*','0','#','D'}};
static int8_t KeyPadNumbRead;	//counter of the number of reads read into the buffer
//static uint8_t KeyPadPressTime;	//counts the time the press lasted
static int8_t KeyPadBuffInId;	//index of the next read will be inserted to the buffer
static int8_t KeyPadBuffOutId;	//index of the next read will be removed from the buffer
static uint8_t KeyPadTimeOut;

//inter-module function declarations
KeyPadRetT KeyPadStatesFlush(void);
KeyPadRetT KeyPadFlush(KeyPadT*KeyPad);
KeyPadRetT KeyPadStateUpdate(uint8_t ix, uint8_t jx);


KeyPadRetT KeyPadInit(KeyPadT* KeyPad){
	if (KeyPad != NULL){
		uint8_t i =0;
		for (i=0; i < KPM_NUMB_ROW; i++){
			GpioInitPin(KeyPad->RowsBase,ROWS_STARTING_BIT+i,PIN_OUT);
		}
		for (i=0; i < KPM_NUMB_COL; i++){
			GpioInitPin(KeyPad->ColumnsBase,COLUMNS_STARTING_BIT+i,PIN_IN);
		}
		KeyPadNumbRead = 0;
		KeyPadBuffInId = 0;
		KeyPadBuffOutId = 0;
		//KeyPadPressTime = 0;
		KeyPad->State = KPM_INITIATED;
		return KPM_ERR_NO;
	}
	return KPM_ERR_INV_PARAMS;
}

KeyPadRetT KeyPadGetKeyBlocking(KeyPadT*KeyPad,uint8_t* Key,uint16_t TimeOut){
	if( (KeyPad != NULL) && (Key != NULL) ){
		if (TimeOut == KPM_NO_TIMEOUT){
			while (KeyPadNumbRead <= 0){
				//if there is no ready data in the buffer, wait forever
			}
		} 
		else{
			KeyPadTimeOut = TimeOut;
			while(KeyPadTimeOut > 0){
				//wait for the timeout
				if (KeyPadNumbRead > 0){
					KeyPadTimeOut = 0;
					break;
				}
			}
		}
		if (KeyPadNumbRead > 0){
			*Key = KeyPadBuffer[KeyPadBuffOutId];
			KeyPadBuffOutId++;
			if (KeyPadBuffOutId >= KPM_BUFFER_LENGTH){
				KeyPadBuffOutId = 0;
			}
			KeyPadNumbRead--;
		}
		else{
			*Key = KPM_NO_KEY_PRESSED;
		}
	}
	return KPM_ERR_NO;
}


KeyPadRetT KeyPadGetKeyNonBlocking(KeyPadT*KeyPad,uint8_t* Key){
	if( (KeyPad != NULL) && (Key != NULL) ){
		if (KeyPadNumbRead > 0){
			*Key = KeyPadBuffer[KeyPadBuffOutId];
			KeyPadBuffOutId++;
			if (KeyPadBuffOutId >= KPM_BUFFER_LENGTH){
				KeyPadBuffOutId = 0;
			}
			KeyPadNumbRead--;
		}
		else{
			*Key = KPM_NO_KEY_PRESSED;
		}
	}
	return KPM_ERR_NO;
}

//unfortunately this func needs to be redefined/re-written and
//it's only designed for a 4*4 keypad module
//in the periodic isr
KeyPadRetT KeyPadScanKey(KeyPadT*KeyPad){
	uint8_t i;
	uint8_t TempRead=0;
	
	if (KeyPadTimeOut > 0){
		KeyPadTimeOut--;
	}
	
	for (i = 1; i <= 8; i = i<<1){
		GpioWriteGroup(KeyPad->RowsBase,i,KPM_NUMB_ROW,ROWS_STARTING_BIT);
		TempRead = GpioReadGroup(KeyPad->ColumnsBase,KPM_NUMB_COL,COLUMNS_STARTING_BIT);
		if (i == 1){
			switch(TempRead){
				case 1:
					KeyPadStateUpdate(1,1);
					break;
				case 2:
					KeyPadStateUpdate(1,2);
					break;
				case 4:
					KeyPadStateUpdate(1,3);
					break;
				case 8:
					KeyPadStateUpdate(1,4);
					break;
			}
		}
		else if(i == 2){
			switch(TempRead){
				case 1:
				KeyPadStateUpdate(2,1);
				break;
				case 2:
				KeyPadStateUpdate(2,2);
				break;
				case 4:
				KeyPadStateUpdate(2,3);
				break;
				case 8:
				KeyPadStateUpdate(2,4);
				break;
			}
		
		}
		else if(i == 4){
			switch(TempRead){
				case 1:
				KeyPadStateUpdate(3,1);
				break;
				case 2:
				KeyPadStateUpdate(3,2);
				break;
				case 4:
				KeyPadStateUpdate(3,3);
				break;
				case 8:
				KeyPadStateUpdate(3,4);
				break;
			}
	
		}
		else{
			switch(TempRead){
				case 1:
				KeyPadStateUpdate(4,1);
				break;
				case 2:
				KeyPadStateUpdate(4,2);
				break;
				case 4:
				KeyPadStateUpdate(4,3);
				break;
				case 8:
				KeyPadStateUpdate(4,4);
				break;
			}
		}
	}
	return KPM_ERR_NO;
}

KeyPadRetT KeyPadStateUpdate(uint8_t ix, uint8_t jx){
	static uint8_t IxOld = 0,JxOld = 0;
	static uint8_t AutoRepeatDelay = KPM_AUTO_REPEATE_DELAY / KPM_TIMER_ISR_PERIOD;
	if (KeyPadStates[ix][jx] == KPM_KEY_NOT_PRESSED){
		 KeyPadStatesFlush();
		KeyPadStates[ix][jx] = KPM_KEY_DEBOUNCE;
	}
	else if (KeyPadStates[ix][jx] == KPM_KEY_DEBOUNCE){
		KeyPadStatesFlush();
		KeyPadStates[ix][jx] = KPM_KEY_PRESSED;
	}
	else if (KeyPadStates[ix][jx] == KPM_KEY_PRESSED){
		KeyPadStatesFlush();
		KeyPadStates[ix][jx] = KPM_KEY_NOT_PRESSED;
		if ((ix == IxOld) && (jx == JxOld)){
			//wait for the auto-repeat delay to finish
			AutoRepeatDelay--;
			if (AutoRepeatDelay == 0){
				AutoRepeatDelay = KPM_AUTO_REPEATE_DELAY / KPM_TIMER_ISR_PERIOD;
				//store the pressed key only after the auto-repeat delay finished
				KeyPadBuffer[KeyPadBuffInId] = KeyPadDecode[ix][jx];
				KeyPadBuffInId++;
				if (KeyPadBuffInId >= KPM_BUFFER_LENGTH){
					KeyPadBuffInId = 0;
				}
				KeyPadNumbRead++;		
			}
		} 
		else{
			//update the auto-repeat
			AutoRepeatDelay = KPM_AUTO_REPEATE_DELAY / KPM_TIMER_ISR_PERIOD;
			//store the pressed key directly
			KeyPadBuffer[KeyPadBuffInId] = KeyPadDecode[ix][jx];
			KeyPadBuffInId++;
			if (KeyPadBuffInId >= KPM_BUFFER_LENGTH){
				KeyPadBuffInId = 0;
			}
			KeyPadNumbRead++;
		}
	}
	IxOld = ix;
	JxOld = jx;
	return KPM_ERR_NO;
}

KeyPadRetT KeyPadStatesFlush(void){
	uint8_t i,j;
	for (i = 0; i < KPM_NUMB_COL; i++){
		for (j = 0; j < KPM_NUMB_COL; j++){
			KeyPadStates[i][j] = KPM_KEY_NOT_PRESSED;
		}
	}
	return KPM_ERR_NO;
}

KeyPadRetT KeyPadFlush(KeyPadT*KeyPad){
	KeyPadNumbRead = 0;
	KeyPadBuffInId = 0;
	KeyPadBuffOutId = 0;
	//KeyPadPressTime = 0;
	return KPM_ERR_NO;
}
#endif //0