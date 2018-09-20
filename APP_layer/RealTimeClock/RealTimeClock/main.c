/*
 * RealTimeClock.c
 *
 * Created: 05/08/2018 12:08:45
 * Author : Mahmoud
 */ 

#include <avr/io.h>
#include "TMU.h"
#include "seg_disp.h"
//#define _BASE_A SECOND0
//#define _BASE_B SECOND1
//#define _BASE_C MINUTE0
//#define _BASE_D MINUTE1
static ecore_u8 s0,s1,m0,m1;
ecore_u8 sec0,sec1,min0,min1;							//displays handlers
TMU_STR_ELEMENT secondsA,secondsB,miutesC,minutesD,leD;	//displays timers
/*	
void clk_init(ecore_u8 *sec0,ecore_u8 *sec1,ecore_u8 *min0,ecore_u8 *min1,TMU_STR_ELEMENT *secondsA,TMU_STR_ELEMENT *secondsB,TMU_STR_ELEMENT *miutesC,TMU_STR_ELEMENT *minutesD,TMU_STR_ELEMENT *leD);
*/
void disp_outA(void);
void disp_outB(void);
void disp_outC(void);
void disp_outD(void);
void out_led(void);
int main(void)
{
	DDRA=0xff;
	DDRB=0xff;
	DDRC=0xff;
	DDRD=0xff;
seg_errors_t res=NO_ERRORS;
TMU_ErrorTypes resu=NO_ERRORS;
//displays initiation
res = disp_init(_BASE_A ,_COMMON_CATHODE,&sec0);	//bases are as configured in the hw/sim
res = disp_init(_BASE_B,_COMMON_CATHODE,&sec1);
res = disp_init(_BASE_C,_COMMON_CATHODE,&min0);
res = disp_init(_BASE_D,_COMMON_CATHODE,&min1);
//timers initiation
resu = tmu_init(5);
resu = tmu_start_timer(&secondsA,PERIODIC,1200,disp_outA);
resu = tmu_start_timer(&secondsB,PERIODIC,6850,disp_outB);
resu = tmu_start_timer(&miutesC,PERIODIC,60000,disp_outC);
resu = tmu_start_timer(&minutesD,PERIODIC,360000,disp_outD);
resu = tmu_start_timer(&leD,PERIODIC,500,out_led);

resu = disp_reset(&sec0);
resu = disp_reset(&sec1);
resu = disp_reset(&min0);
resu = disp_reset(&min1);

    while (1) 
    {
		resu=tmu_dispatch();

    }
	res+=3;
	resu+=32;
	res+=resu;
}
/*
void clk_init(ecore_u8 *sec0,ecore_u8 *sec1,ecore_u8 *min0,ecore_u8 *min1,TMU_STR_ELEMENT *secondsA,TMU_STR_ELEMENT *secondsB,TMU_STR_ELEMENT *miutesC,TMU_STR_ELEMENT *minutesD,TMU_STR_ELEMENT *leD)
{
	seg_errors_t res=NO_ERRORS;
	TMU_ErrorTypes resu=NO_ERRORS;
	 //displays initiation
	 res = disp_init(_BASE_A ,_COMMON_CATHODE,sec0);	//bases are as configured in the hw/sim
	 res = disp_init(_BASE_B,_COMMON_CATHODE,sec1);
	 res = disp_init(_BASE_C,_COMMON_CATHODE,min0);
	 res = disp_init(_BASE_D,_COMMON_CATHODE,min1);
	 //timers initiation
	resu = tmu_init(10);
	resu = tmu_start_timer(secondsA,PERIODIC,1000,disp_outA);
	resu = tmu_start_timer(secondsB,PERIODIC,10000,disp_outB);
	resu = tmu_start_timer(miutesC,PERIODIC,60000,disp_outC);
	resu = tmu_start_timer(minutesD,PERIODIC,360000,disp_outD);
	resu = tmu_start_timer(leD,PERIODIC,1000,out_led);
}*/

void disp_outA(void)
{
	disp_out(&sec0,s0);
	s0++;
	s0=s0 >= MAX_POSSIBLE_VALS ? 0:s0;
	
}
void disp_outB(void)
{
	disp_out(&sec1,s1);
	s1++;
	s1=s1 >= 6 ? 0:s1;
	
}
void disp_outC(void)
{
	disp_out(&min0,m0);
	m0++;
	m0= m0 >= MAX_POSSIBLE_VALS ? 0:m0;
	
}
void disp_outD(void)
{
	disp_out(&min1,m1);
	m1++;
	m1=m1 >= 3 ? 0:m1;
	
}
void out_led(void)
{
	PORTA ^= (1<<7);
}