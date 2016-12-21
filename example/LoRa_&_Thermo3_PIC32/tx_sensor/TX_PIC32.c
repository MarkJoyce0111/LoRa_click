#include <stdlib.h>
#include "lora.h"
#include "resources.h"

// Thermo 3 I2C address
#define TMP102_I2C_ADDR                         0x48

// TFT pins
char TFT_DataPort                   at LATE;
sbit TFT_RST                        at LATD7_bit;
sbit TFT_BLED                       at LATD2_bit;
sbit TFT_RS                         at LATD9_bit;
sbit TFT_CS                         at LATD10_bit;
sbit TFT_RD                         at LATD5_bit;
sbit TFT_WR                         at LATD4_bit;
char TFT_DataPort_Direction         at TRISE;
sbit TFT_RST_Direction              at TRISD7_bit;
sbit TFT_BLED_Direction             at TRISD2_bit;
sbit TFT_RS_Direction               at TRISD9_bit;
sbit TFT_CS_Direction               at TRISD10_bit;
sbit TFT_RD_Direction               at TRISD5_bit;
sbit TFT_WR_Direction               at TRISD4_bit;

// Lora RF pins
sfr sbit LORA_RST at LATC1_bit;
sfr sbit LORA_RTS at LATC2_bit;
sfr sbit LORA_CTS at RE8_bit;

// Vars
char tmp_txt[ 50 ] = "48656c6C6F";
char tmp_int[ 50 ];

// Function prototypes
void system_init( void );
void display_init( void );
void measure_temp( void );
void lora_cbk( char *response );

// Function definitions
void system_init( void )
{
    TRISC1_bit = 0;
    TRISC2_bit = 0;
    TRISE8_bit = 1;
    TRISD0_bit = 1;
    Delay_ms( 200 );

    //I2C1_Init_Advanced( 400000, &_GPIO_MODULE_I2C1_PB67 );
    //Delay_ms( 200 );

#if defined _DEBUG_
    UART5_Init( 115200 );
    Delay_ms( 200 );
#endif

    UART2_Init( 57600 );
    Delay_ms( 200 );

    AD1PCFG = 0xFFFF;

    U2IP0_bit = 1;
    U2IP1_bit = 1;
    U2IP2_bit = 1;
    U2RXIE_bit = 1;
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

    //I2C1_Start();
    //I2C1_Write( TMP102_I2C_ADDR, tmp_data, 1, END_MODE_RESTART );
    //I2C1_Read( TMP102_I2C_ADDR, tmp_data, 2, END_MODE_STOP );
    raw_temp = ( ( tmp_data[ 0 ] << 8 ) | tmp_data[ 1 ] ) >> 4;

    /// UBACI U PROMENLJIVU I POSALJI SA LORA_TX

    /*if( raw_temp & ( 1 << 11 ) )
        raw_temp |= 0xF800;

    IntToHex( raw_temp, tmp_txt );
    Ltrim( tmp_txt );

    real_temp = ( float )( raw_temp * 0.0625 );
    sprintf( tmp_int, "%2.1f C", real_temp );*/
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

        if( Button( &PORTD, 0, 100, 1 ) )
        {
            measure_temp();
            TFT_Rectangle( 130, 118, 220, 188 );
            TFT_Set_Font( &HandelGothic_BT21x22_Regular, CL_GRAY, FO_HORIZONTAL );
            TFT_Write_Text( tmp_int, 160, 117 );

            if( !lora_tx( "48656c6C6F" ) )    // SLANJE _ PARAMETAR JE TEMPERATURA ( HEX FORMAT ONLY )
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

void UART2interrupt() iv IVT_UART_2 ilevel 7 ics ICS_AUTO
{
      if( IFS1 & ( 1 << U2RXIF ))
      {
           char tmp = UART2_Read();
           lora_rx_isr( tmp );
           U2RXIF_bit = 0;
#ifdef _DEBUG_
           LOG_CH( tmp );
#endif
      }
}