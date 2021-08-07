/*
 * Keypad.c
 *
 * Created: 7/13/2021 6:09:47 AM
 *  Author: ZeroX
 */ 
#include "Keypad.h"

static void KeyPadStateUpdate(uint8_t ix, uint8_t jx);
static void KeyPadStatesFlush(void);

static uint8_t InternalPressedKeyBuffer[MAX_BUFFER_LENGTH]={7};
static uint8_t PressedKeyBufferIn = 0;
static uint8_t PressedKeyBufferOut= 0;
static uint8_t InternalMatrixCodes[KEYPAD_NUMB_ROWS][KEYPAD_NUMB_COLS] = {'1','2','3','*',
	                                                                      '4','5','6','/',
																          '7','8','9','-',
																          '=','0','%','+'};
																		
static KeypadStateT InternalMatrixStates[KEYPAD_NUMB_ROWS][KEYPAD_NUMB_COLS] = {KEY_RELEASED};
static TimerT SysTimer;
static KeypadT SysKeypad;
//initialization
void KeypadInit(KeypadT* Keypad){
	//initialize the rows as out
	//initialize the cols as input
	//GpioInitPort(Keypad->RowsPort,0x0f);
	for (uint8_t i = 0; i < KEYPAD_NUMB_ROWS; i++){
		GpioInitPin(Keypad->RowsPort, Keypad->RowsPosition + i, PIN_OUT);
    }
    for (uint8_t i = 0; i < KEYPAD_NUMB_COLS; i++){
	    GpioInitPin(Keypad->ColsPort, Keypad->ColsPosition + i, PIN_IN);
    }
    //store the keypad info into the internal variable
    SysKeypad = *Keypad;
	//initialize the hw-timer for 20 ms intervals
	  /*SREG_I_bit = 1;
	    OCR0 = 155;
	    TCCR0 = 0x28;
	    TCCR0 |= 0x05;
	    OCIE0_bit = 1;*/
	SysTimer.Base = TIMER_0;
	SysTimer.Clk  = CLK_PRESCALER_8;
	SysTimer.OcCallBkFunc = KeypadUpdate;
	SysTimer.OcrVal = 100;
	SysTimer.CompMatchMode  = OC_CLEAR;
	SysTimer.Int = INT_OC;
	SysTimer.Mode = CTC_OP_MODE;
	SysTimer.TcntVal = 0x00;
	TimerInit(&SysTimer);
}
//task or isr -- periodic task
void KeypadUpdate(void){
	//check it's period come
	//take new sample for the keypad
	//change the states of the keypad
	for (int i = 1; i <= 8; i = i<<1){
		GpioWriteGroup(SysKeypad.RowsPort,i,KEYPAD_NUMB_ROWS,SysKeypad.RowsPosition);
		//uint8_t TempRead = GpioReadPort(SysKeypad.ColsPort);
        //TempRead = (TempRead & 0xf0) >> SysKeypad.ColsPosition;
        uint8_t TempRead = GpioReadGroup(SysKeypad.ColsPort,KEYPAD_NUMB_COLS,SysKeypad.ColsPosition); //& 0x0f;
        //PORTA = TempRead;//testing
		//think the error is here
		if (i == 1){
			switch(TempRead){
				case 1:
                PORTD ^= 1;
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
	
}
//function
uint8_t KeypadGetKey(KeypadT* Keypad){
    if (Keypad == NULL){
        return MODULE_NOT_INITIALIZED;
    }
    if (Keypad->InitState == MODULE_INITIALIZED){
	        //return key state or key pressed
            uint8_t temp = InternalPressedKeyBuffer[PressedKeyBufferOut];
            PressedKeyBufferOut++;
            if (PressedKeyBufferOut == MAX_BUFFER_LENGTH){
                PressedKeyBufferOut = 0;
            return temp;
        }
        return MODULE_NOT_INITIALIZED;
    }
}    

void KeyPadStateUpdate(uint8_t ix, uint8_t jx){
    static KeypadStateT OldState = KEY_PRE_RELEASED;
    switch (InternalMatrixStates[ix][jx])
    {
        case KEY_RELEASED:
        /* Your code here */
        KeyPadStatesFlush();
        InternalMatrixStates[ix][jx] = KEY_PRE_PRESSED;
        OldState = KEY_RELEASED;
        break;
        case KEY_PRE_PRESSED:
        /* Your code here */
        KeyPadStatesFlush();
        InternalMatrixStates[ix][jx] = KEY_PRESSED;
        OldState = KEY_PRE_PRESSED;    
        break;
        case KEY_PRESSED:
        /* Your code here */
        KeyPadStatesFlush();
        InternalMatrixStates[ix][jx] = KEY_PRESSED;
        if (OldState != KEY_PRESSED){
            InternalPressedKeyBuffer[PressedKeyBufferIn] = InternalMatrixCodes[ix][jx];
            PORTD = InternalMatrixCodes[ix][jx];
            PressedKeyBufferIn++;
            if (PressedKeyBufferIn == MAX_BUFFER_LENGTH){
                PressedKeyBufferIn = 0;
            }
        }
        OldState = KEY_PRESSED;
        break;
        default:
        /* Your code here */
        break;
    }
    
}
//is flush needed?
void KeyPadStatesFlush(void){
    uint8_t i,j;
    for (i = 0; i < KEYPAD_NUMB_ROWS; i++){
        for (j = 0; j < KEYPAD_NUMB_COLS; j++){
            InternalMatrixStates[i][j] = KEY_RELEASED;
        }
    }
}


// void KeyPadStateUpdate(uint8_t ix, uint8_t jx){
//         static uint8_t PrePressesdNumOfSampling = 0;
//         static uint8_t PressesdNumOfSampling    = 0;
//         static uint8_t PreReleasedNumOfSampling = 0;
//         static uint8_t ReleasedNumOfSampling    = 0;
//     if ((InternalMatrixStates[ix][jx] == KEY_RELEASED) && (ReleasedNumOfSampling < KEYPAD_SAMPLES_NUMB)){
//         KeyPadStatesFlush();
//         InternalMatrixStates[ix][jx] = KEY_RELEASED;
//         ReleasedNumOfSampling++;
//     }
//     else if ((InternalMatrixStates[ix][jx] == KEY_RELEASED) && (ReleasedNumOfSampling >= KEYPAD_SAMPLES_NUMB)){
//         KeyPadStatesFlush();
//         InternalMatrixStates[ix][jx] = KEY_PRE_PRESSED;
//         ReleasedNumOfSampling = 0;
//     }
//     else if ((InternalMatrixStates[ix][jx] == KEY_PRE_PRESSED) && (PrePressesdNumOfSampling < KEYPAD_SAMPLES_NUMB)){
//         KeyPadStatesFlush();
//         InternalMatrixStates[ix][jx] = KEY_PRE_PRESSED;
//         PrePressesdNumOfSampling++;
//     }
//     else if ((InternalMatrixStates[ix][jx] == KEY_PRE_PRESSED) && (PrePressesdNumOfSampling >= KEYPAD_SAMPLES_NUMB)){
//         KeyPadStatesFlush();
//         InternalMatrixStates[ix][jx] = KEY_PRESSED;
//         PrePressesdNumOfSampling = 0;
//     }
//     else if ((InternalMatrixStates[ix][jx] == KEY_PRESSED) && (PressesdNumOfSampling < KEYPAD_SAMPLES_NUMB)){
//         KeyPadStatesFlush();
//         InternalMatrixStates[ix][jx] = KEY_PRESSED;
//         PressesdNumOfSampling++;
//     }
//     else if ((InternalMatrixStates[ix][jx] == KEY_PRESSED) && (PressesdNumOfSampling >= KEYPAD_SAMPLES_NUMB)){
//         KeyPadStatesFlush();
//         InternalMatrixStates[ix][jx] = KEY_PRE_RELEASED;
//         PressesdNumOfSampling = 0;
//         //u may offer a option here to take the value at pressed/pre/released time so lets do it this way for now
//         InternalPressedKeyBuffer[PressedKeyBufferIn] = InternalMatrixCodes[ix][jx];
//         PressedKeyBufferIn++;
//         if (PressedKeyBufferIn == MAX_BUFFER_LENGTH){
//             PressedKeyBufferIn = 0;
//         }
//     }
//     else if ((InternalMatrixStates[ix][jx] == KEY_PRE_RELEASED) && (PreReleasedNumOfSampling < KEYPAD_SAMPLES_NUMB)){
//         KeyPadStatesFlush();
//         InternalMatrixStates[ix][jx] = KEY_PRE_RELEASED;
//         PreReleasedNumOfSampling++;
//     }
//     else if ((InternalMatrixStates[ix][jx] == KEY_PRE_RELEASED) && (PreReleasedNumOfSampling >= KEYPAD_SAMPLES_NUMB)){
//         KeyPadStatesFlush();
//         InternalMatrixStates[ix][jx] = KEY_RELEASED;
//         PreReleasedNumOfSampling = 0;
//     }
// }