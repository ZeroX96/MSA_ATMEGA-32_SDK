/*
 * ExternalEeprom.h
 *
 * Created: 6/17/2021 11:52:27 PM
 *  Author: ZeroX
 */ 


#ifndef EXTERNALEEPROM_H_
#define EXTERNALEEPROM_H_

#define EEPROM_PAGE_SIZE 256
#define EEPROM_ADD_AT24C 0xA0

#include "Twi.h"

#define TWI_EEPROM_BASE 40

typedef enum
{
	EEPROM_ERR_NO 			   = (TWI_EEPROM_BASE+1),
	EEPROM_ERR_INV_PARAMS      = (TWI_EEPROM_BASE+2),
	EEPROM_ERR_DEVICE_NOT_INIT = (TWI_EEPROM_BASE+3),
	EEPROM_ERR_WRITE_COLLISION = (TWI_EEPROM_BASE+4),
}EepromRetT;



typedef TwiT EepromT;
/*
TwiBaseT EepromBase;
TODO: TwiStatusT EepromStatus;	//returns the status of the last operation
uint8_t EepromBitRate;				//the value will be set in the TWBR register.
uint8_t EepromAdd;	//the slave add, me, as a master, will talk to.
TwiPrescallerT EepromPrescaller;
*/
EepromRetT EepromInit(EepromT* Eeprom, TwiBaseT EepromBase, uint8_t EepromBitRate, uint8_t EepromAdd, TwiPrescallerT EepromPrescaller);

EepromRetT EepromWriteByte(EepromT* Eeprom, uint16_t* MemAdd, uint8_t* Data );
EepromRetT EepromWriteArrayManual(EepromT* Eeprom, uint16_t* MemAdd, uint8_t* DataArr, uint8_t ArrayLength);
EepromRetT EepromWriteArrayAuto(EepromT* Eeprom, uint16_t* MemAdd, uint8_t* DataArr, uint8_t ArrayLength);

EepromRetT EepromWritePage(EepromT* Eeprom, uint16_t* MemAdd, uint8_t* DataPage);

EepromRetT EepromAckPolling(EepromT* Eeprom);

EepromRetT EepromReadCurrentAdd(EepromT* Eeprom,uint8_t* Data );

EepromRetT EepromReadRandom(EepromT* Eeprom, uint16_t* MemAdd,uint8_t* Data );

EepromRetT EepromReadSequential(EepromT* Eeprom, uint16_t* MemAdd, uint8_t* DataArray, uint8_t Length );




#if 0 //INFO

• Internally Organized 131,072 x 8
• 400 kHz (2.7V) and 1 MHz (5V) Clock Rate
• Write Protect Pin for Hardware and Software Data Protection
• 256-byte Page Write Mode (Partial Page Writes Allowed)
• Random and Sequential Read Modes
• Self-timed Write Cycle (5 ms Typical)
– Endurance: 100,000 Write Cycles/Page
– Data Retention: 40 Years
- The AT24C1024 provides 1,048,576 bits organized as 131,072 words of 8 bits each
Pin Name Function
A1	 Address Input
SDA	 Serial Data
SCL	 Serial Clock Input
WP	 Write Protect
NC	 No Connect

- The 1024K is internally organized as 512 pages of 256 bytes each. Random word addressing requires a 17-bit data word address.
- All addresses and data words are serially transmitted to and from the EEPROM in 8-bit words.
- STANDBY MODE: The AT24C1024 features a low-power standby mode which is enabled: 
	a) upon power-up and b) after the receipt of the stop bit and the completion of any internal operations.
- MEMORY RESET: After an interruption in protocol, power loss or system reset, any two-wire part can be reset by following
	these steps: 1. Clock up to 9 cycles. 2. Look for SDA high in each cycle while SCL is high. 3. Create a start condition.
- The device address word consists of a mandatory one, zero sequence for the first five most significant bits.
- The 1024K uses the one device address bit, A1, to allow up to two devices on the same bus.
- The seventh bit (P0) of the device address is a memory page address bit. This memory page
	address bit is the most significant bit of the data word address that follows. 
- The eighth bit of the device address is the read/write operation select bit. A read operation is initiated if this bit
	is high and a write operation is initiated if this bit is low.
- DATA SECURITY: The AT24C1024 has a hardware data protection scheme that allows the user to write-protect 
	the entire memory when the WP pin is at VCC.
//int main(){
//DDRA = DDRB = DDRD = 0xff;
//EepromT msa;
//uint8_t into_1 = 55, into_2 = 66;
//uint8_t outo_1 = 0, outo_2 = 0;
//EepromInit(&msa, TWI_0_BASE, 8, EEPROM_ADD_AT24C, TWI_PRESCALLER_1);
//while(1){
//EepromWriteByte(&msa, 500,&into_1);
//EepromWriteByte(&msa, 600, &into_2);
//_delay_ms(100);
//EepromReadRandom(&msa, 500, &outo_1);
//EepromReadRandom(&msa, 600, &outo_2);
//PORTA = outo_1;
//PORTB = outo_2;
//_delay_ms(100);
//PORTA = 0;
//PORTB = 0;
//_delay_ms(100);
//into_1 += 2;
//into_2 += 4;
//_delay_ms(100);
//PORTD++;
//}
//return 0;
//}

#endif //INFO

#endif /* EXTERNALEEPROM_H_ */