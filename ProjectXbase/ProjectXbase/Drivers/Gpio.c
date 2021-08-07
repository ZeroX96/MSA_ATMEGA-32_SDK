/*
 * Gpio.c
 *
 * Created: 3/20/2020 5:38:06 AM
 * Edited : 4/26/2020 2:42:15 PM
 *  Author: Mahmo
 */ 
#include "Gpio.h"

GpioRetT GpioInitPort(GpioBaseT Base,GpioStateT State){
	if ((Base >= BASE_D) && (Base <= BASE_A)){
		HwWriteReg(Base + DDRx_OFFSET, State);
#ifdef INTERNAL_PULL_UP_EN
		if (PORT_IN == State){
			HwWriteReg(Base + PORTx_OFFSET, 0xff);
		}
#endif //INTERNAL_PULL_UP_EN
		return GPIO_ERR_NO;
	} 
	else{
		return GPIO_ERR_INV_PARAMS;
	}
}

GpioRetT GpioInitPin(GpioBaseT Base,GpioPinT Pin,GpioStateT State){
	if ((Base >= BASE_D) && (Base <= BASE_A)){
		if(PIN_IN == State){
			HwClrRegBit(Base+DDRx_OFFSET, Pin);
#ifdef INTERNAL_PULL_UP_EN
			HwSetRegBit(Base+PORTx_OFFSET, Pin);
#endif //INTERNAL_PULL_UP_EN
		}
		else if(PIN_OUT == State){
			HwSetRegBit(Base+DDRx_OFFSET, Pin);
		}
		return GPIO_ERR_NO;
	}
	else{
		return GPIO_ERR_INV_PARAMS;
	}
}

GpioRetT GpioWritePort(GpioBaseT Base,uint8_t Val){
	if ((Base >= BASE_D) && (Base <= BASE_A)){
		HwWriteReg(Base+PORTx_OFFSET, Val);
		return GPIO_ERR_NO;
	}
	else{
		return GPIO_ERR_INV_PARAMS;
	}
}

GpioRetT GpioWriteGroup(GpioBaseT Base,uint8_t Val, uint8_t Len, uint8_t FirstBit){
	if ((Base >= BASE_D) && (Base <= BASE_A)){
		HwWriteRegBitGroup(Base+PORTx_OFFSET, Val, Len, FirstBit);
		return GPIO_ERR_NO;
	}
	else{
		return GPIO_ERR_INV_PARAMS;
	}
}

GpioRetT GpioWritePin(GpioBaseT Base,GpioPinT Pin,GpioStateT State){
	if ((Base >= BASE_D) && (Base <= BASE_A)){
		if(PIN_LOW == State){
			HwClrRegBit(Base+PORTx_OFFSET, Pin);
		}
		else if(PIN_HIGH == State){
			HwSetRegBit(Base+PORTx_OFFSET, Pin);
		}
		else{
			return GPIO_ERR_INV_PARAMS;
		}
		return GPIO_ERR_NO;
	}
	else{
		return GPIO_ERR_INV_PARAMS;
	}
}

uint8_t  GpioReadPort(GpioBaseT Base){
	return (HwReadReg(Base));
}

uint8_t GpioReadGroup(GpioBaseT Base, uint8_t Len, uint8_t FirstBit){
	if ((Base >= BASE_D) && (Base <= BASE_A)){
		return (HwReadRegBitGroup(Base, Len, FirstBit));
	}
	else{
		return GPIO_ERR_INV_PARAMS;
	}
}

uint8_t  GpioReadPin(GpioBaseT Base,GpioPinT Pin){
	if(HwReadBit(Base, Pin)){
		return PIN_HIGH;
	}else{
		return PIN_LOW;
	}
}