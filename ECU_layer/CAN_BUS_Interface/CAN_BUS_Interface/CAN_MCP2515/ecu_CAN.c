/*
 * ecu_CAN.c
 *
 * Created: 29/01/2019 12:47:09
 *  Author: Mahmoud
 */ 
#include "ecu_CAN.h"

#if 0	//the spi driver APIs
hal_spiInit(				str_spi_objectInfo_t	*strg_obj,
							spi_driver_base_t		driver_base,
							spi_sck_freq_select_t	freq_select,
							spi_operating_mode_t	mode,
							spi_notifcs_mode_t		notfics_mode,
							spi_transfer_modes_t	transfer_mode,
							spi_data_order_t		data_order);

hal_spiExchangeDATA(		str_spi_objectInfo_t	*strg_obj,
							msa_u8					*ByteOUT,
							msa_u8					*ByteIN);
									
hal_spiExchangeDataPacket(	str_spi_objectInfo_t	*strg_obj,
							msa_u8					*PacketOUT,
							msa_u8					*PacketIN,
							msa_u8					PacketSize);
										
hal_spiDeinit(				str_spi_objectInfo_t	*strg_obj);
hal_setSpiIsrCallback(		str_spi_objectInfo_t	*strg_obj,
							void (*vptr_cb)(void));
#endif

static str_spi_objectInfo_t obj_1;//holds the spi configurations		//for the future..it's not good,if have more than one spi/can driver then what will happen??
									//fixed>>will store this in the cfg_obj//but will edit the code laer :F
can_errors_t ecu_can_init(can_configs_t *can_cfg_obj)
{
	can_errors_t exe_state=NO_CAN_ERRORS;
	if (can_cfg_obj != NULL)
	{
		//check that the configurations are right and in the config_object
		
		//initializing the spi driver
		hal_spiInit(&obj_1,SPI_1_base,FREQ_BY_4,MASTER_EN,SPI_POLLING,MODE_1,MSB_FIRST);	//reconfig according the can controller specifications
		//initializing the AVR interrupt
		exe_state = init_interrrupts();
		exe_state = set_interrupt(ISR_NO,LOW_LEVEL);		
		SET_BIT(DDRB,SS_PIN);//based on avr arch and atmega32 reg_map set the ss pin as an output pin
		CLEAR_BIT(PORTB,SS_PIN);
		//begin the mcp_initialization and interfacing
		can_cfg_obj->initialization_state=DEVICE_INITIATED;
		//resetting the mcu to ensure a startup
		exe_state = ecu_can_reset(can_cfg_obj);
		//initializing the CAN controller
		//setting the bit timing
		msa_u8 init_arr[3]={( (can_cfg_obj->phase2_seg_length_obj) | (can_cfg_obj->wake_up_filter_obj<<WAKFIL) | (can_cfg_obj->start_of_frame_obj<<SOF) ),
							( (can_cfg_obj->btl_mode_obj<<BTLMODE) | (can_cfg_obj->sampling_number_obj<<SAM  ) | (can_cfg_obj->phase1_seg_length_obj<<PHSEG10) | (can_cfg_obj->propagation_seg_length_obj<<PRSEG0) ),
							( (can_cfg_obj->synch_jmp_width_obj<<SJW0) | (can_cfg_obj->baud_rate_prescaler_obj) )
						   };
		exe_state=ecu_can_write(can_cfg_obj,CNF3,init_arr,3);	//first try:- to write to sequential registers by	continuing 
																//to clock in data bytes, as long as CS is held low.

		
		
		SET_BIT(PORTB,SS_PIN);//make the pin high
		//??????? ????	
	} 
	else
	{
		exe_state=INVALID_CAN_PARAMS;
	}
	return exe_state;	
}

can_errors_t ecu_can_reset(can_configs_t *can_cfg_obj)
{
	can_errors_t exe_state=NO_CAN_ERRORS;
	if (can_cfg_obj != NULL)
	{
		if(can_cfg_obj->initialization_state == DEVICE_INITIATED)
		{
			CLEAR_BIT(PORTB,SS_PIN);	//CSlow
			can_instructions_t instruction=RESET;
			hal_spiExchangeDATA(&obj_1,(msa_u8)&instruction,(msa_u8)&instruction);//used the same variable to get the received data as it's not imp any more
			SET_BIT(PORTB,SS_PIN); //needs to return cs to the high state for the next instructions
		}
		else
		{
			exe_state=DEVICE_NOT_INITIATED;
		}
	} 
	else
	{
		exe_state=INVALID_CAN_PARAMS;
	}
	return exe_state;
	}//performs the reset instruction
	
can_errors_t ecu_can_read(can_configs_t *can_cfg_obj,msa_u8 wanted_add,msa_u8 *receiving_buffer,msa_u8 data_size)
{
	can_errors_t exe_state=NO_CAN_ERRORS;
	if( (can_cfg_obj != NULL) && (receiving_buffer != NULL) )
	{
		if(can_cfg_obj->initialization_state == DEVICE_INITIATED)
		{
			CLEAR_BIT(PORTB,SS_PIN); //The Read instruction is started by lowering the CSpin.
			//first the READ instruction itself
			can_instructions_t instruction=READ;
			hal_spiExchangeDATA(&obj_1,(msa_u8)&instruction,(msa_u8)&instruction);//used the same variable to get the received data as it's not imp any more
			//then the 8-bits add #Revisited_to_ensure_the_datasheet_schematic_done
			hal_spiExchangeDATA(&obj_1,&wanted_add,(msa_u8)&instruction);//used the instruction variable to get the received data as it's not imp any more and the output/received data is useless
			//now data_bytes will be got_out_of_the_controller as long as there is a clock which will dep on the data_size variable
			hal_spiExchangeDataPacket(&obj_1,(msa_u8)&wanted_add,receiving_buffer,data_size);//used the wanted_add variable just to meet the function calling  while it's useless since the controller needs only a clock to send it's data out and wont care about the val received
																								//revisit for the size zero
			//The read operation is terminated by raising the CSpin
			SET_BIT(PORTB,SS_PIN);
		} 
		else
		{
			exe_state=DEVICE_NOT_INITIATED;//revise this val later
		}
	}
	else
	{
		exe_state=INVALID_CAN_PARAMS;
	}
	return exe_state;
}//performs the read instruction

can_errors_t ecu_can_readRXbuffer(can_configs_t *can_cfg_obj,rx_buff_add_t rx_buff,msa_u8 *receiving_buffer,msa_u8 data_size)
{
	can_errors_t exe_state=NO_CAN_ERRORS;
	if( (can_cfg_obj != NULL) && (receiving_buffer != NULL) && ( (rx_buff >= RXB0SIDH) && (rx_buff <= RXB1D0) ) )
	{
		if(can_cfg_obj->initialization_state == DEVICE_INITIATED)
		{
			CLEAR_BIT(PORTB,SS_PIN); //The Read_RX_Buffer instruction is started by lowering the CSpin.
			//first the Read_RX_Buffer instruction itself is "OR"ed with the possible rx_buff_add so selects one of the possible four starting points
			can_instructions_t instruction=(Read_RX_Buffer|(rx_buff<<1));	//rx_buff is shifted left to meet it's position in the instruction byte
			hal_spiExchangeDATA(&obj_1,(msa_u8)&instruction,(msa_u8)&instruction);//used the same variable to get the received data as it's not imp any more
			//now data_bytes will be got_out_of_the_controller as long as there is a clock which will dep on the data_size variable
			hal_spiExchangeDataPacket(&obj_1,(msa_u8)&instruction,receiving_buffer,data_size);//used the instruction variable just to meet the function calling  while it's useless since the controller needs only a clock to send it's data out and wont care about the val received
			//revisit for the size zero
			//The Read_RX_Buffer operation is terminated by raising the CSpin
			SET_BIT(PORTB,SS_PIN); //Note:The associated RX flag bit (CANINTF.RXnIF) will be cleared after bringing CShigh.
			//so once the received data is read,the flag will be cleared so it may receive more data from the can bus :D
		}
		else
		{
			exe_state=DEVICE_NOT_INITIATED;//revise this val later
		}
	}
	else
	{
		exe_state=INVALID_CAN_PARAMS;
	}
	return exe_state;
}

can_errors_t ecu_can_write(can_configs_t *can_cfg_obj,msa_u8 targeted_add,msa_u8 *transmitted_buffer,msa_u8 data_size)
{
	
	can_errors_t exe_state=NO_CAN_ERRORS;
	if( (can_cfg_obj != NULL) && (transmitted_buffer != NULL) && (data_size >= 1) )//the data size must be at least one
	{
		if(can_cfg_obj->initialization_state == DEVICE_INITIATED)
		{
			CLEAR_BIT(PORTB,SS_PIN); //The WRITE instruction is started by lowering the CSpin.
			//first the WRITE instruction itself
			can_instructions_t instruction=WRITE;
			hal_spiExchangeDATA(&obj_1,(msa_u8)&instruction,(msa_u8)&instruction);//used the same variable to get the received data as it's not imp any more
			//then the 8-bits add #Revisited_to_ensure_the_datasheet_schematic_done
			hal_spiExchangeDATA(&obj_1,&targeted_add,(msa_u8)&instruction);//used the instruction variable to get the received data as it's not imp any more and the output/received data is useless
			//now data_bytes will be sent_into_the_controller as long as there is a clock which will dep on the data_size variable
			hal_spiExchangeDataPacket(&obj_1,transmitted_buffer,&targeted_add,data_size);//used the wanted_add variable just to meet the function calling  while it's useless since the controller needs only a clock to send it's data out and wont care about the val received
			//revisit for the size zero #Done
			_delay_us(2);//#for_safety_since_the_data_writing_op_will_be_terminated_if_went_high_before_all_the_bits_were_writen
			//The WRITE operation is terminated by raising the CSpin
			SET_BIT(PORTB,SS_PIN);
		}
		else
		{
			exe_state=DEVICE_NOT_INITIATED;//revise this val later
		}
	}
	else
	{
		exe_state=INVALID_CAN_PARAMS;
	}
	return exe_state;	
}

can_errors_t ecu_can_LoadTXBuffer(can_configs_t *can_cfg_obj,tx_buff_add_t tx_buff,msa_u8 *transmitted_buffer,msa_u8 data_size)
{
	
	can_errors_t exe_state=NO_CAN_ERRORS;
	if( (can_cfg_obj != NULL) && (transmitted_buffer != NULL) && (data_size >= 1) && ( (tx_buff >= TXB0SIDH) && (tx_buff <= TXB2D0) ) )//the data size must be at least one
	{
		if(can_cfg_obj->initialization_state == DEVICE_INITIATED)
		{
			CLEAR_BIT(PORTB,SS_PIN); //The LoadTXBuffer instruction is started by lowering the CSpin.
			//first the LoadTXBuffer instruction itself is "OR"ed with tx_buff so the instruction is complete
			can_instructions_t instruction=(Read_RX_Buffer|tx_buff);
			hal_spiExchangeDATA(&obj_1,(msa_u8)&instruction,(msa_u8)&instruction);//used the same variable to get the received data as it's not imp any more
			//now data_bytes will be sent_into_the_controller as long as there is a clock which will dep on the data_size variable
			hal_spiExchangeDataPacket(&obj_1,transmitted_buffer,(msa_u8)&instruction,data_size);//used the instruction variable just to meet the function calling  while it's useless since i dont care about the returned data which will be always 00xff
			//revisit for the size zero #Done
			_delay_us(2);//#for_safety_since_the_data_writing_op_will_be_terminated_if_went_high_before_all_the_bits_were_writen
			//The WRITE operation is terminated by raising the CSpin
			SET_BIT(PORTB,SS_PIN);
		}
		else
		{
			exe_state=DEVICE_NOT_INITIATED;//revise this val later
		}
	}
	else
	{
		exe_state=INVALID_CAN_PARAMS;
	}
	return exe_state;
}

can_errors_t ecu_can_RequestToSend(can_configs_t *can_cfg_obj,rts_buffer_select_t buffer_select)
{
	
	can_errors_t exe_state=NO_CAN_ERRORS;
	if( (can_cfg_obj != NULL) && ( (buffer_select >= RTS_TXB0) && (buffer_select <= RTS_TXB0_TXB1_TXB2) ) )//the data size must be at least one
	{
		if(can_cfg_obj->initialization_state == DEVICE_INITIATED)
		{
			//The MCP2515 is selected by lowering the CSpin.
			CLEAR_BIT(PORTB,SS_PIN);
			//first the Request_To_Send instruction itself is "OR"ed with buffer_select so the instruction is complete
			can_instructions_t instruction=(Request_To_Send|buffer_select);
			//This command will set the TxBnCTRL.TXREQ bit for the respective buffer(s).
			//Any or all of the last three bits can be set in a single command. 
			//If the RTS command is sent with nnn =000, the command will be ignored.
			hal_spiExchangeDATA(&obj_1,(msa_u8)&instruction,(msa_u8)&instruction);//used the same variable to get the received data as it's not imp any more and the received dta is meaningles and useles and it's all about the function call
			_delay_us(2);//#for_safety_since_the_data_writing_op_will_be_terminated_if_went_high_before_all_the_bits_were_writen
			//The request to send operation is terminated by raising the CSpin
			SET_BIT(PORTB,SS_PIN);
		}
		else
		{
			exe_state=DEVICE_NOT_INITIATED;//revise this val later
		}
	}
	else
	{
		exe_state=INVALID_CAN_PARAMS;
	}
	return exe_state;
}

can_errors_t ecu_can_readStatus(can_configs_t *can_cfg_obj,msa_u8 *received_status_byte)
{
	can_errors_t exe_state=NO_CAN_ERRORS;
	if( (can_cfg_obj != NULL) && (received_status_byte != NULL) )
	{
		if(can_cfg_obj->initialization_state == DEVICE_INITIATED)
		{
			CLEAR_BIT(PORTB,SS_PIN); //The Read_Status instruction is started by lowering the CSpin.
			//first the Read_Status instruction itself is sent out
			can_instructions_t instruction=Read_Status;
			hal_spiExchangeDATA(&obj_1,(msa_u8)&instruction,(msa_u8)&instruction);//used the same variable to get the received data as it's not imp any more
			//once the instruction is sent out,the status byte will be got from the controller so will send useless data "the instruction variable" just to get the status byte from the controller
			hal_spiExchangeDATA(&obj_1,(msa_u8)&instruction,received_status_byte);//used the same variable to get the received data as it's not imp any more
			_delay_us(2);//#for_safety_since_the_data_writing_op_will_be_terminated_if_went_high_before_all_the_bits_were_writen
			//The read_Status operation is terminated by raising the CSpin
			SET_BIT(PORTB,SS_PIN);
	
		}
		else
		{
			exe_state=DEVICE_NOT_INITIATED;//revise this val later
		}
	}
	else
	{
		exe_state=INVALID_CAN_PARAMS;
	}
	return exe_state;	
}

can_errors_t ecu_can_RXStatus(can_configs_t *can_cfg_obj,msa_u8 *received_status_byte)
{
	
	can_errors_t exe_state=NO_CAN_ERRORS;
	if( (can_cfg_obj != NULL) && (received_status_byte != NULL) )
	{
		if(can_cfg_obj->initialization_state == DEVICE_INITIATED)
		{
			CLEAR_BIT(PORTB,SS_PIN); //The RX_Status instruction is started by lowering the CSpin.
			//first the Read_Status instruction itself is sent out
			can_instructions_t instruction=RX_Status;
			hal_spiExchangeDATA(&obj_1,(msa_u8)&instruction,(msa_u8)&instruction);//used the same variable to get the received data as it's not imp any more
			//once the instruction is sent out,the status byte will be got from the controller so will send useless data "the instruction variable" just to get the status byte from the controller
			hal_spiExchangeDATA(&obj_1,(msa_u8)&instruction,received_status_byte);//used the same variable to get the received data as it's not imp any more
			_delay_us(2);//#for_safety_since_the_data_writing_op_will_be_terminated_if_went_high_before_all_the_bits_were_writen
			//The RXStatus operation is terminated by raising the CSpin
			SET_BIT(PORTB,SS_PIN);
			
		}
		else
		{
			exe_state=DEVICE_NOT_INITIATED;//revise this val later
		}
	}
	else
	{
		exe_state=INVALID_CAN_PARAMS;
	}
	return exe_state;
}

can_errors_t ecu_can_BitModify(can_configs_t *can_cfg_obj,msa_u8 targeted_add,msa_u8 modification_mask,msa_u8 *transmitted_buffer,msa_u8 data_size)
{
	
	can_errors_t exe_state=NO_CAN_ERRORS;
	if( (can_cfg_obj != NULL) && (transmitted_buffer != NULL) && (data_size >= 1) )//the data size must be at least one
	{
		if(can_cfg_obj->initialization_state == DEVICE_INITIATED)
		{
			CLEAR_BIT(PORTB,SS_PIN); //The BitModify instruction is started by lowering the CSpin.
			//first the BitModify instruction itself is sent out
			can_instructions_t instruction=Bit_Modify;
			hal_spiExchangeDATA(&obj_1,(msa_u8)&instruction,(msa_u8)&instruction);//used the same variable to get the received data as it's not imp any more
			//then the 8-bits add #Revisited_to_ensure_the_datasheet_schematic_done
			hal_spiExchangeDATA(&obj_1,&targeted_add,(msa_u8)&instruction);//used the instruction variable to get the received data as it's not imp any more and the output/received data is useless
			//then the 8-bits MASK
			hal_spiExchangeDATA(&obj_1,&modification_mask,(msa_u8)&instruction);//used the instruction variable to get the received data as it's not imp any more and the output/received data is useless
			//now data_byte will be sent_into_the_controller
			hal_spiExchangeDATA(&obj_1,transmitted_buffer,&targeted_add);//used the wanted_add variable just to meet the function calling  while it's useless since the controller needs only a clock to send it's data out and wont care about the val received
			//revisit for the size zero #Done
			_delay_us(2);//#for_safety_since_the_data_writing_op_will_be_terminated_if_went_high_before_all_the_bits_were_writen
			//The WRITE operation is terminated by raising the CSpin
			SET_BIT(PORTB,SS_PIN);
		}
		else
		{
			exe_state=DEVICE_NOT_INITIATED;//revise this val later
		}
	}
	else
	{
		exe_state=INVALID_CAN_PARAMS;
	}
	return exe_state;
}


can_errors_t ecu_can_IntEnable(can_configs_t *can_cfg_obj,interrupt_source_t int_src)
{
	can_errors_t exe_state=NO_CAN_ERRORS;
	if( (can_cfg_obj != NULL) && ( (int_src >= RX0_FULL_BUFFER) && (int_src >= MESSAGE_ERROR) ) )//the data size must be at least one
	{
		if(can_cfg_obj->initialization_state == DEVICE_INITIATED)
		{
			int_src=(1<<int_src);	//set the prospective int source bit in the byte
			exe_state=ecu_can_BitModify(can_cfg_obj,CANINTE,int_src,&int_src,1);	//used the bit_modify func not to affect the remaining bits in the register 
																					//used the same val "int_src" as the flag because it does what is needed and it's exactly right 
		}
		else
		{
			exe_state=DEVICE_NOT_INITIATED;//revise this val later
		}
	}
	else
	{
		exe_state=INVALID_CAN_PARAMS;
	}
	return exe_state;

}

can_errors_t ecu_can_IntDisable(can_configs_t *can_cfg_obj,interrupt_source_t int_src)
{
	can_errors_t exe_state=NO_CAN_ERRORS;
	if( (can_cfg_obj != NULL) && ( (int_src >= RX0_FULL_BUFFER) && (int_src >= MESSAGE_ERROR) ) )//the data size must be at least one
	{
		if(can_cfg_obj->initialization_state == DEVICE_INITIATED)
		{
			int_src&=~(1<<int_src);	//clear the prospective int source bit in the byte,write ZERO
			exe_state=ecu_can_BitModify(can_cfg_obj,CANINTE,(~(int_src)),&int_src,1);	//used the bit_modify func not to affect the remaining bits in the register
																						//used the same val "int_src" and performed a "NOT" oeration so the mask works well
		}
		else
		{
			exe_state=DEVICE_NOT_INITIATED;//revise this val later
		}
	}
	else
	{
		exe_state=INVALID_CAN_PARAMS;
	}
	return exe_state;

}

can_errors_t ecu_can_IntStatus(can_configs_t *can_cfg_obj,msa_u8 *received_status_byte)
{
	can_errors_t exe_state=NO_CAN_ERRORS;
	if( (can_cfg_obj != NULL) && (received_status_byte != NULL) )//the data size must be at least one
	{
		if(can_cfg_obj->initialization_state == DEVICE_INITIATED)
		{
			exe_state=ecu_can_read(can_cfg_obj,CANINTF,received_status_byte,1);
		}
		else
		{
			exe_state=DEVICE_NOT_INITIATED;//revise this val later
		}
	}
	else
	{
		exe_state=INVALID_CAN_PARAMS;
	}
	return exe_state;
}

//if enabled the mcp2515 interrupt generation this pin will be pulled low till all the isr sources have been serviced

ISR(ISR_NAME)
{
	//next when using an RTOS move the following to a task with a high priority and the isr just releases it's SEMAPHORE
	
	
}


#if 0  //the comming function isn't complete and shouldn't be used as there is an error,will work right for the first time only 
can_errors_t ecu_can_IntByteCFG(can_configs_t *can_cfg_obj,interrupts_struct_t *int_obj)
{
	can_errors_t exe_state=NO_CAN_ERRORS;
	if( (can_cfg_obj != NULL) && (int_obj != NULL) )//the data size must be at least one
	{
		if(can_cfg_obj->initialization_state == DEVICE_INITIATED)
		{
			msa_u8 interrupts_byte = (  //the error is here >>if wanted to write a ZERO in a bit,will be neglicted so the sol is 
										//instead of shifting the result,the result will select an OR"ing with ONE or "AND"ing with ZERO
										//for later on edits 
										(( (int_obj->RX0_FULL_BUFFER_interrupt  == 1 ) ? 1 : 0 ) << RX0_FULL_BUFFER )|
										(( (int_obj->RX1_FULL_BUFFER_interrupt  == 1 ) ? 1 : 0 ) << RX1_FULL_BUFFER )|
										(( (int_obj->TX0_EMPTY_BUFFER_interrupt == 1 ) ? 1 : 0 ) << TX0_EMPTY_BUFFER)|
										(( (int_obj->TX1_EMPTY_BUFFER_interrupt == 1 ) ? 1 : 0 ) << TX1_EMPTY_BUFFER)|
										(( (int_obj->TX2_EMPTY_BUFFER_interrupt == 1 ) ? 1 : 0 ) << TX2_EMPTY_BUFFER)|
										(( (int_obj->ERROR_interrupt            == 1 ) ? 1 : 0 ) << ERROR_INTERRUPT )|
										(( (int_obj->WAKE_UP_interrupt			== 1 ) ? 1 : 0 ) << WAKE_UP         )|
										(( (int_obj->MESSAGE_ERROR_interrupt	== 1 ) ? 1 : 0 ) << MESSAGE_ERROR   )
									  );	//so now the interrupts register can be writen @ one time :D 
									  
			
			
			
			exe_state=ecu_can_BitModify(can_cfg_obj,CANINTE,(~(int_src)),&int_src,1);	//used the bit_modify func not to affect the remaining bits in the register
			//used the same val "int_src" and performed a "NOT" oeration so the mask works well
		}
		else
		{
			exe_state=DEVICE_NOT_INITIATED;//revise this val later
		}
	}
	else
	{
		exe_state=INVALID_CAN_PARAMS;
	}
	return exe_state;

}
#endif


// #ByMSA