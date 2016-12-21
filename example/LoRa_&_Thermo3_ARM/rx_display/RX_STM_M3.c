#include <stdlib.h>
#include "lora.h"
#include "resources.h"

#define _DEBUG_

#if defined _DEBUG_
#define LOG(x)        UART1_Write_Text(x)
#endif

// TFT pins
unsigned int TFT_DataPort at GPIOE_ODR;
sbit TFT_RST at GPIOE_ODR.B8;
sbit TFT_RS at GPIOE_ODR.B12;
sbit TFT_CS at GPIOE_ODR.B15;
sbit TFT_RD at GPIOE_ODR.B10;
sbit TFT_WR at GPIOE_ODR.B11;
sbit TFT_BLED at GPIOE_ODR.B9;

// Lora RF pins
sfr sbit LORA_RST at GPIOC_ODR.B2;
sfr sbit LORA_RTS at GPIOD_ODR.B13;
sfr sbit LORA_CTS at GPIOD_IDR.B10;

char tmp_txt[ 50 ];
char tmp_int[ 50 ];

// Function prototypes
void system_init( void );
void display_init( void );
void convert_temp( void );
void lora_cbk( char *response );

// Function definitions
void system_init( void )
{
    GPIO_Digital_Output( &GPIOC_BASE, _GPIO_PINMASK_2 );
    GPIO_Digital_Output( &GPIOD_BASE, _GPIO_PINMASK_13 );
    GPIO_Digital_Input( &GPIOD_BASE, _GPIO_PINMASK_10 );
    Delay_ms( 200 );

#if defined _DEBUG_
    UART1_Init_Advanced( 115200, _UART_8_BIT_DATA,
                                 _UART_NOPARITY,
                                 _UART_ONE_STOPBIT,
                                 &_GPIO_MODULE_USART1_PA9_10 );
    Delay_ms( 200 );
#endif

    UART3_Init_Advanced( 57600, _UART_8_BIT_DATA,
                                _UART_NOPARITY,
                                _UART_ONE_STOPBIT,
                                &_GPIO_MODULE_USART3_PD89 );
    Delay_ms( 200 );

    RXNEIE_USART3_CR1_bit = 1;
    NVIC_IntEnable( IVT_INT_USART3 );
    EnableInterrupts();
    Delay_ms( 500 );

#if defined _DEBUG_
    LOG( "< Initialization done >\r\n" );
#endif
}

void display_init()
{
    TFT_Init_ILI9341_8bit( 320, 240 );
    TFT_BLED = 1;
    TFT_Set_Pen( CL_WHITE, 1 );
    TFT_Set_Brush( 1, CL_WHITE, 0, 0, 0, 0 );
    TFT_Set_Font( TFT_defaultFont, CL_BLACK, FO_HORIZONTAL );
    TFT_Fill_Screen( CL_WHITE );
    TFT_Set_Pen( CL_BLACK, 1 );
    TFT_Line(  20,  46, 300,  46 );
    TFT_Line(  20,  70, 300,  70 );
    TFT_Line(  20, 220, 300, 220 );
    TFT_Set_Pen( CL_WHITE, 1 );
    TFT_Set_Font( &HandelGothic_BT21x22_Regular, CL_RED, FO_HORIZONTAL );
    TFT_Write_Text( "Lora click", 108, 14 );
    TFT_Set_Font( &Verdana12x13_Regular, CL_BLACK, FO_HORIZONTAL );
    TFT_Write_Text("EasyMx PRO v7 for STM32", 19, 223);
    TFT_Set_Font( &Verdana12x13_Regular, CL_RED, FO_HORIZONTAL );
    TFT_Write_Text( "www.mikroe.com", 200, 223 );
    TFT_Set_Font( &Tahoma15x16_Bold, CL_BLACK, FO_HORIZONTAL );
    TFT_Write_Text( "Temperature :", 40, 120 );
}

void convert_temp( void )
{
    unsigned int raw_temp;
    float real_temp;

    raw_temp = xtoi( &tmp_txt[ 10 ] );
    real_temp = ( float )( raw_temp * 0.0625f );
    sprintf( tmp_int, "%2.1f C", real_temp );
}

void lora_cbk( char* response )
{

}

void main()
{
    system_init();
    display_init();
    lora_init( false, false, false, lora_cbk );

    lora_cmd( "sys get ver", "", tmp_txt );
    TFT_Set_Font( &Tahoma15x16_Bold, CL_BLUE, FO_HORIZONTAL );
    TFT_Write_Text( tmp_txt, 50, 50 );
    
    lora_cmd( "mac pause", "", tmp_txt );
#if defined _DEBUG_
    LOG( tmp_txt );
    LOG( "\r\n\r\n" );
#endif
        
    lora_cmd( "radio set wdt", "0", tmp_txt );
#if defined _DEBUG_
    LOG( tmp_txt );
    LOG( "\r\n\r\n" );
#endif

    while( 1 )
    {
        if( !lora_rx( 5000, tmp_txt ) )
        {
            TFT_Rectangle( 10, 190, 310, 218 );
            TFT_Set_Font( &Tahoma15x16_Bold, CL_GREEN, FO_HORIZONTAL );
            TFT_Write_Text( " TEMPERATURE RECEIVED ", 80, 190 );
            TFT_Rectangle( 160, 117, 220, 188 );
            convert_temp();
            TFT_Set_Font( &HandelGothic_BT21x22_Regular, CL_GRAY, FO_HORIZONTAL );
            TFT_Write_Text( tmp_int, 160, 117 );
        }
        else {
        
            TFT_Rectangle( 10, 190, 310, 218 );
            TFT_Set_Font( &Tahoma15x16_Bold, CL_RED, FO_HORIZONTAL );
            TFT_Write_Text( " LORA RADIO ERROR ", 90, 190 );
        }
#if defined _DEBUG_
        LOG( "  Response : " );
        LOG( tmp_txt );
        LOG( "\r\n\r\n" );
        Delay_ms( 3000 );
#endif
    }
}

void LO_RX_ISR() iv IVT_INT_USART3 ics ICS_AUTO
{
     if( RXNE_USART3_SR_bit )
     {
         char tmp = USART3_DR;
         lora_rx_isr( tmp );
     }
}