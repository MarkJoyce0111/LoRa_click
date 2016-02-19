#include "lora_rf.h"
#include <stddef.h>

sbit RF_LORA_RST at LATC1_bit;
sbit RF_LORA_RST_Direction at TRISC1_bit;

char rsp[ 256 ];

void system_init( void );

void system_init( )
{
    RF_LORA_RST_Direction = 0;
    Delay_ms( 100 );

    UART5_Init( 115200 );
    Delay_ms( 100 );
    UART2_Init( 57600 );
    Delay_ms( 100 );

    AD1PCFG = 0xFFFF;

    U2IP0_bit = 0;
    U2IP1_bit = 1;
    U2IP2_bit = 1;
    U2RXIE_bit = 1;
    EnableInterrupts();

    UART5_Write_Text( "\r\n Initialization Done \r\n" );
}

void get_response( char *resp )
{
    UART5_Write_Text( resp );
    UART5_Write_Text( "\r\n" );
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

void UART2interrupt() iv IVT_UART_2 ilevel 6 ics ICS_AUTO
{
      if( IFS1 & ( 1 << U2RXIF ))
      {
           char tmp = UART2_Read();
           lora_rf_rx_isr( tmp );
           U2RXIF_bit = 0;
      }
}
