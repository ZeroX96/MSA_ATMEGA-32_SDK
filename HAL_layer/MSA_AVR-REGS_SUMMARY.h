




//USART_REGS from me
#define msa_ucsra (*(volatile msa_u8)0x20)
//from tiva
#define SYSCTL_RCGC2_R          (*((volatile uint32_t *)0x400FE108))
//ENDOF USART_REGS