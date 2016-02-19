#include "lora_rf.h"
#include <stddef.h>

sbit RF_LORA_RST at GPIO_PIN1_bit;

char rsp[ MAX_RSP_SIZE + MAX_DATA_SIZE ];

void system_init( void );
void get_response( char *resp );

void system_init()
{
     GPIO_Digital_Output( &GPIO_PORT_00_07, _GPIO_PINMASK_1 );

     UART1_Init( 115200 );
     UART2_Init_Advanced( 57600, _UART_DATA_BITS_8,
                                 _UART_PARITY_NONE,
                                 _UART_STOP_BITS_1 );

     UART2_FCR |= ( 1 << FIFO_EN ) | ( 1 << RCVR_RST );
     UART2_INT_ENABLE |= ( 1 << RX_AVL_EN );

     IRQ_CTRL &= ~( 1 << GLOBAL_INTERRUPT_MASK );
}

void get_response( char *resp )
{
    UART1_Write_Text( resp );
    UART1_Write_Text( "\r\n" );
}

void main()
{
    system_init();
    lora_rf_init();
    lora_rf_callback( get_response );

    lora_rf_pause( rsp );
    lora_rf_set_wdog( 0 );
    lora_rf_tx( "ABCDEF0123456789" );

    while( 1 )
    {
        lora_rf_process();
    }
}

void UART_RX_ISR() iv IVT_UART2_IRQ ics ICS_AUTO
{
    if( UART2_LSR & ( 1 << DATA_RDY ) )
    {
        unsigned char tmp_data;
        tmp_data = UART2_RBR;
        lora_rf_rx_isr( tmp_data );
    }
}
