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
-The Reset instruction is a single-byte instruction that requires selecting the device by pulling CS low, sending the instruction byte and then raising CS
-It is highly recommended that the reset command be sent (or the RESETpin be lowered) as part of the power-on initialization sequence.

#READ
-The Read instruction is started by lowering the CSpin.
-The Read instruction is then sent to the MCP2515 followed by the 8-bit address Next, the data stored in the register at the selected address will be shifted out on the SO pin
-The Read operation is terminated by raising the CSpin

#Read RX Buffer 
	-The Read RX Buffer instruction provides a means to quickly address a receive buffer for reading.
	-This instruction reduces the SPI overhead by one byte,the address byte.
	-The command byte actually has four possible values that determine the address pointer location.
	-This instruction further reduces the SPI overhead by automatically clearing the associated receive flag (CANINTF.RXnIF) when CSis raised at the end of the command.

#WRITE
-The Write instruction is started by lowering the CSpin.
-The Write instruction is then sent to the MCP2515 followed by the address and at least one byte of data.	
-It is possible to write to sequential registers by continuing to clock in data bytes, as long as CSis held low.	
-Data will actually be written to the register on the rising edge of the SCK line for the D0 bit. 
-If the CS line is brought high before eight bits are loaded, the write will be aborted for that data byte and previous bytes in the command will have been written.

#LOAD TX BUFFER 
-The Load TX Buffer instruction eliminates the eight-bit address required by a normal write command. 
-The eight-bit instruction sets the address pointer to one of six addresses to quickly write to a transmit buffer that points to the “ID” or “data” address of any of the three transmit buffers.

#REQUEST-TO-SEND
-The RTS command can be used to initiate message transmission for one or more of the transmit buffers.
-The MCP2515 is selected by lowering the CSpin. The RTS command byte is then sent.
-the last 3 bits of this command indicate which transmit buffer(s) are enabled to send.
-This command will set the TxBnCTRL.TXREQ bit for the respective buffer(s).
-Any or all of the last three bits can be set in a single command. 
-If the RTS command is sent with nnn =000, the command will be ignored.

#READ-STATUS
-The MCP2515 is selected by lowering the CSpin and the read status command byte is sent to the MCP2515. 
-Once the command byte is sent, the MCP2515 will return eight bits of data that contain the status.
-If additional clocks are sent after the first eight bits are transmitted, the MCP2515 will continue to output the status bits as long as the CSpin is held low and clocks are provided on SCK.
-Each status bit returned in this command may also be read by using the standard read command with the appropriate register address.

#RX STATUS
-The RX Status instruction is used to quickly determine which filter matched the message and message type (standard, extended, remote).
-After  the command byte is sent, the controller will return 8 bits of data that contain the status data.
-If more clocks are sent after the 8 bits are transmitted, the controller will continue to output the same status bits as long as the CSpin stays low and clocks are provided.

#BIT-MODIFY
-The Bit Modify instruction provides a means for setting or clearing individual bits in specific status and control registers.
-This command is not available for all registers. See  Section 11.0 “Register Map”to determine which registers allow the use of this command.
-The part is selected by lowering the CSpin and the Bit Modify command byte is then sent to the MCP2515.
-The command is followed by the address of the register, the mask byte and finally the data byte.
->The mask byte determines which bits in the register will be allowed to change. A ‘1’ in the mask byte will allow a bit in the register to change, while a ‘0’ will not.
->The data byte determines what value the modified bits in the register will be changed to. A ‘1’ in the data byte will set the bit and a ‘0’ will clear the bit, provided that the mask for that bit is set to a ‘1’
#endif