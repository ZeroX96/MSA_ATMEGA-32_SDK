/*
 * Adc.c
 *
 * Created: 3/20/2020 5:37:29 AM
 *  Author: Mahmo
 */ 
#include "Adc.h"
//extern uint16_t ReadVal2;
static AdcCallBackFunT AdcCallBack = NULL;

AdcRetT AdcInit(AdcT* Adc){
	if (NULL != Adc)
	{
		//u must change this accourding to your app
# warning "u must change this accourding to your app"
		GpioInitPin(ADC_BASE,ADC_PIN,PIN_IN);
		
		HwWriteReg(ADC_MUX,(Adc->RefSel  << REFS_0_BIT) | \
						   (Adc->ResAdj  << ADLAR_BIT)  | \
		                   (Adc->Channel << MUX_0_BIT));
		
		HwWriteReg(ADC_CSRA,(TRUE << ADEN_BIT) 			| \	
						   (Adc->AutoTrig << ADATE_BIT) | \
						   (Adc->IntMode  << ADIE_BIT)  | \
						   (Adc->PrescSel << ADPS_0_BIT));
						   
		HwWriteReg(ADC_SFIO,(Adc->TrigSrc << ADTS_0_BIT));
		
		if (Adc->IntMode == INTURREPT_ENABLE){
			AdcCallBack = Adc->	CALLBackFunc;
			sei();//enable the global interrupts 
		}
		return ADC_ERR_NO;
	}
	
	return ADC_INV_PARAMS;
}
AdcRetT AdcRead(AdcT* Adc,uint16_t* ReadValue){
	if (NULL != Adc)
	{
		static uint8_t TempRead;
		//static uint16_t test_val=0;
		if (HwIsBitSet(ADC_CSRA,ADIF_BIT)){
			HwSetRegBit(ADC_CSRA,ADIF_BIT);	//clear the flag bit by writing one to it
		}
		
		HwSetRegBit(ADC_CSRA,ADSC_BIT);
		//wait as long as the completion flag is not set to one i.e, the conversion still going
		while( (HwIsBitSet(ADC_CSRA,ADIF_BIT)) == FALSE ){
			//DoNothing
			//PORTB++;
		}
		TempRead= HwReadReg(ADC_L);
		*ReadValue = ((HwReadReg(ADC_H)<<8) | TempRead);
		//test_val = *ReadValue;
		//ReadVal2 = *ReadValue;
		//*ReadValue = ADC;
		return ADC_ERR_NO;
	}
	return ADC_INV_PARAMS;
}
AdcRetT AdcEdit(AdcT* Adc){
	if (NULL != Adc)
	{
		HwClrRegBit(ADC_CSRA,ADEN_BIT);	 
		AdcInit(Adc);
		return ADC_ERR_NO;
	}
	return ADC_INV_PARAMS;
}

AdcRetT AdcDeInit(AdcT* Adc){
	if (NULL != Adc)
	{
		
		HwWriteReg(ADC_MUX,0);
		
		HwWriteReg(ADC_CSRA,0);
		
		HwWriteReg(ADC_SFIO,0);
		
		if (Adc->IntMode == INTURREPT_ENABLE){
			AdcCallBack = NULL;
		}
		return ADC_ERR_NO;
	}
	return ADC_INV_PARAMS;
}

ISR(ADC_vect)
{
	if (NULL != AdcCallBack)
	{
		(*AdcCallBack)();
	}
}
