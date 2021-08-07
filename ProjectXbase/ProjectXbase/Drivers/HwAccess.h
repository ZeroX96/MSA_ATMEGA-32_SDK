/*
 * HwAccess.h
 *
 * Created: 03/25/2020 02:00:55 AM
 * Edited : 04/26/2020 02:35:06 PM
 * Edited : 12/18/2020 08:20:40 AM
 *  Author: Mahmo
 */ 


#ifndef HWACCESS_H_
#define HWACCESS_H_
#include <stdint.h>

#ifndef NULL
#define NULL ((void*)0)
#endif	//NULL

#define TRUE		(1U)
#define FALSE		(0U)
#define STD_HIGH	(1U)
#define STD_LOW		(0U)
//#define HwWritePort(X,V)	(*((volatile uint8_t*)(X)) = V)

static inline void HwWriteReg(uint8_t Reg, uint8_t Val){
	(*(volatile uint8_t*)Reg) = Val;
}

static inline void HwWriteRegOring(uint8_t Reg, uint8_t Val){
	(*(volatile uint8_t*)Reg) |= Val;
}

static inline uint8_t HwReadReg(uint8_t Reg){
	return (*(volatile uint8_t*)Reg);
}
/*ERROR SOLVED HERE
 * Maybe your problem is that there is no prototype available when the function
 * is used the first time and the compiler implicitly creates one that will not be static. 
 * If so the solution is to add a prototype somewhere before it is first used.
*/
static inline void HwSetRegBit(uint8_t Reg, uint8_t Bit){
	(*(volatile uint8_t*)Reg) |= (1<<Bit);
}

static inline void HwClrRegBit(uint8_t Reg, uint8_t Bit){
	(*(volatile uint8_t*)Reg) &= ~(1<<Bit);
}

static inline uint8_t HwReadBit(uint8_t Reg, uint8_t Bit){
	return (*(volatile uint8_t*)Reg) & (1<<Bit);
}

// len from 1..to..n

static inline void HwWriteRegBitGroup(uint8_t Reg, uint8_t GpVal, uint8_t GpLen, uint8_t GpFirstBit){
	uint8_t GpTemp,i;
	for(i = 0;i < GpLen;i++){
		GpTemp = GpVal & (1<<i);
		if(GpTemp != 0){
			HwSetRegBit(Reg, GpFirstBit + i);
		}
		else{
			HwClrRegBit(Reg, GpFirstBit + i);
		}
	}
}

static inline uint8_t HwReadRegBitGroup(uint8_t Reg, uint8_t GpLen, uint8_t GpFirstBit){
	uint8_t Mask=0;
	uint8_t TempRes =  HwReadReg(Reg);
	TempRes = TempRes >> GpFirstBit;
	
	if (GpLen == 1){
		Mask = 0x01;
	}
	else if(GpLen == 2){
		Mask = 0x03;
	}
	else if(GpLen == 3){
		Mask = 0x07;
	}
	else if(GpLen == 4){
		Mask = 0x0F;
	}
	else if(GpLen == 5){
		Mask = 0x1F;
	}
	else if(GpLen == 6){
		Mask = 0x3F;
	}
	else if(GpLen == 7){
		Mask = 0x7F;
	}
	else if(GpLen == 8){
		Mask = 0xFF;
	}
	else{
		//DoNothing
	}
	return ( TempRes & Mask);
}

static inline uint8_t HwIsBitSet(uint8_t Reg, uint8_t Bit){
	if(((*(volatile uint8_t*)Reg) & (1<<Bit)) != 0U){
		return TRUE;
	}else{
		return FALSE;
	}
}

static inline uint8_t HwIsBitClr(uint8_t Reg, uint8_t Bit){
	if(((*(volatile uint8_t*)Reg) & (1<<Bit)) != 0U){
		return FALSE;
	}else{
		return TRUE;
	}
}


#endif /* HWACCESS_H_ */
