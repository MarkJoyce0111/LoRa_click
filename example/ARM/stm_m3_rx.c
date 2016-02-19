/* LoRa RF Example
********************/
#include "lora_rf.h"


/* Variables
**************/
sbit RF_LORA_RST at GPIOC_ODR.B2;
char rsp[ MAX_RSP_SIZE + MAX_DATA_SIZE ];


/* Function Prototypes
************************/
void system_init( void );
void get_response( char *resp );
void btn_7_press( void );


/* Functions
**************/
void system_init( )
{
    GPIO_Digital_Output( &GPIOC_ODR, _GPIO_PINMASK_2 );
    Delay_ms( 100 );

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

    UART1_Write_Text( "\r\n Initialization Done \r\n" );
}

void get_response( char *resp )
{
    UART1_Write_Text( resp );
    UART1_Write_Text( "\r\n" );
}

void btn_7_press()
{
    char tmp[ 50 ] = { 0 };

    lora_rf_rx( 0, tmp );
}

void main()
{
    system_init();
    lora_rf_init();
    lora_rf_callback( get_response );

    lora_rf_fw_ver( rsp );
    lora_rf_pause( rsp );
    lora_rf_set_wdog( 0 );
    lora_rf_tx( "ABCDEF0123456789" );

    UART1_Write_Text( "\r\n Process Started \r\n" );
    while( 1 )
    {
        lora_rf_process();
    }
}

void LO_RX_ISR() iv IVT_INT_USART3 ics ICS_AUTO
{
     if( RXNE_USART3_SR_bit )
     {
         char tmp = USART3_DR;
         lora_rf_rx_isr( tmp );
     }
}