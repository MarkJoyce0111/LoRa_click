#include "lora.h"

#define LOG(x)   UART1_Write_Text(x)

sfr sbit LORA_RST at GPIOA_ODR.B4;
sfr sbit LORA_RTS at GPIOD_IDR.B13;
sfr sbit LORA_CTS at GPIOD_ODR.B10;

char text[ MAX_TRANSFER_SIZE ] = { 0 };

void lora_cb( char *response );
void sys_init( void );

void lora_cb( char *response )
{
    LOG( "< < CB > >" );
    LOG( "\r\n_________________________________________\r\n\r\n" );
    LOG( response );
    LOG( "\r\n_________________________________________\r\n" );
}

void sys_init( void )
{
    GPIO_Digital_Output( &GPIOA_BASE, _GPIO_PINMASK_4 );
    GPIO_Digital_Input( &GPIOD_BASE, _GPIO_PINMASK_13 );
    GPIO_Digital_Output( &GPIOD_BASE, _GPIO_PINMASK_10 );
    
    UART1_Init_Advanced( 115200, _UART_8_BIT_DATA,
                                 _UART_NOPARITY,
                                 _UART_ONE_STOPBIT,
                                 &_GPIO_MODULE_USART1_PA9_10 );

    UART3_Init_Advanced( 57600, _UART_8_BIT_DATA,
                                _UART_NOPARITY,
                                _UART_ONE_STOPBIT,
                                &_GPIO_MODULE_USART3_PD89);

    Delay_ms( 100 );

    RXNEIE_USART3_CR1_bit = 1;
    NVIC_IntEnable( IVT_INT_USART3 );

    LOG( "\r\n< < INIT DONE > >\r\n" );
}

void main() 
{
    sys_init();
    lora_init( false, false, false, lora_cb );
    
    lora_cmd( "sys get ver", "", text );
    LOG( text );
    
    lora_cmd( "sys sleep", "2000", text );
    LOG( text );
    
    lora_cmd( "sys get ver", "", text );
    LOG( text );
    
    while( 1 )
        lora_process();
}

void LO_RX_ISR() iv IVT_INT_USART3 ics ICS_AUTO
{
     if( RXNE_USART3_SR_bit )
     {
         char tmp = USART3_DR;
         lora_rx_isr( tmp );
     }
}