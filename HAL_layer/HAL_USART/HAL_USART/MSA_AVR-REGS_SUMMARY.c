#include "common.h"





//USART_REGS


#define MSA_UBRRH	(*(volatile msa_u8)0x40)
#define MSA_UCSRC	(*(volatile msa_u8)0x40)
#define MSA_UDR		(*(volatile msa_u8)0x2c)
#define MSA_UCSRA	(*(volatile msa_u8)0x2b)
#define MSA_UCSRB	(*(volatile msa_u8)0x2a)
#define MSA_UCSRC	(*(volatile msa_u8)0x29)

//end of usart regs