#include <stdlib.h>
#include "lora.h"
#include "resources.h"

#define _DEBUG_

#if defined _DEBUG_
#define LOG(x)                                  UART1_Write_Text(x)
#endif

// Thermo 3 I2C address
#define TMP102_I2C_ADDR                         0x48

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

// Vars
char tmp_txt[ 50 ];
char tmp_int[ 50 ];

// Function prototypes
void system_init( void );
void display_init( void );
void measure_temp( void );
void lora_cbk( char *response );

// Function definitions
void system_init( void )
{
    GPIO_Digital_Output( &GPIOC_BASE, _GPIO_PINMASK_2 );
    GPIO_Digital_Output( &GPIOD_BASE, _GPIO_PINMASK_13 );
    GPIO_Digital_Input( &GPIOD_BASE, _GPIO_PINMASK_10 );
    GPIO_Digital_Input( &GPIOC_BASE, _GPIO_PINMASK_7 );
    Delay_ms( 200 );
    
    I2C1_Init_Advanced( 400000, &_GPIO_MODULE_I2C1_PB67 );
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
    TFT_Write_Text( "LoRa click", 108, 14 );
    TFT_Set_Font( &Verdana12x13_Regular, CL_BLACK, FO_HORIZONTAL );
    TFT_Write_Text("EasyMx PRO v7 for STM32", 19, 223);
    TFT_Set_Font( &Verdana12x13_Regular, CL_RED, FO_HORIZONTAL );
    TFT_Write_Text( "www.mikroe.com", 200, 223 );
    TFT_Set_Font( &Tahoma15x16_Bold, CL_BLACK, FO_HORIZONTAL );
    TFT_Write_Text( "Temperature :", 40, 120 );
}

void measure_temp()
{
    int raw_temp;
    float real_temp;
    uint8_t tmp_data[ 2 ] = { 0x00 };

    I2C1_Start();
    I2C1_Write( TMP102_I2C_ADDR, tmp_data, 1, END_MODE_RESTART );
    I2C1_Read( TMP102_I2C_ADDR, tmp_data, 2, END_MODE_STOP );
    raw_temp = ( ( tmp_data[ 0 ] << 8 ) | tmp_data[ 1 ] ) >> 4;

    if( raw_temp & ( 1 << 11 ) )
        raw_temp |= 0xF800;

    IntToHex( raw_temp, tmp_txt );
    Ltrim( tmp_txt );
    
    real_temp = ( float )( raw_temp * 0.0625 );
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
        lora_process();
        
        if( Button( &GPIOC_IDR, 7, 100, 1 ) )
        {
            measure_temp();
            TFT_Rectangle( 130, 118, 220, 188 );
            TFT_Set_Font( &HandelGothic_BT21x22_Regular, CL_GRAY, FO_HORIZONTAL );
            TFT_Write_Text( tmp_int, 160, 117 );
            
            if( !lora_tx( tmp_txt ) )
            {
                TFT_Rectangle( 10, 190, 310, 218 );
                TFT_Set_Font( &Tahoma15x16_Bold, CL_GREEN, FO_HORIZONTAL );
                TFT_Write_Text( " TEMPERATURE SENT ", 90, 190 );
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
}

void LO_RX_ISR() iv IVT_INT_USART3 ics ICS_AUTO
{
     if( RXNE_USART3_SR_bit )
     {
         char tmp = USART3_DR;
         lora_rx_isr( tmp );
     }
}
