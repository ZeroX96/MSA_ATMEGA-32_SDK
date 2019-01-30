/*
 * NOTES.h
 *
 * Created: 29/01/2019 12:49:20
 *  Author: Mahmoud
 */ 

#if 0

sending 
-The CSpin must be held low while any operation is performed.
-works on modes 0,0 & 1,1
-Data is driven out by the MCP2515 on the falling edge of SCK.
-The Reset instruction can be used to re-initialize the internal registers of the MCP2515 and set Configuration mode.


#RESET
-The Reset instruction is a single-byte instruction that requires selecting the device by pulling CSlow, sending the instruction byte and then raising CS
-It is highly recommended that the reset command be sent (or the RESETpin be lowered) as part of the power-on initialization sequence.

#endif