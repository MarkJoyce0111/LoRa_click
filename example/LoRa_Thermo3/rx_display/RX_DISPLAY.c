#include <stdlib.h>
#include "lora.h"
#include "resources.h"

// TFT pins
unsigned int TFT_DataPort at GPIOE_ODR;
sbit TFT_RST at GPIOE_ODR.B8;
sbit TFT_RS at GPIOE_ODR.B12;
sbit TFT_CS at GPIOE_ODR.B15;
sbit TFT_RD at GPIOE_ODR.B10;
sbit TFT_WR at GPIOE_ODR.B11;
sbit TFT_BLED at GPIOE_ODR.B9;

// Lora RF pins
sfr sbit LORA_RST at GPIOA_ODR.B4;
sfr sbit LORA_RTS at GPIOD_IDR.B13;
sfr sbit LORA_CTS at GPIOD_ODR.B10;

// Variables
int vdd_lvl;
int tmp_rx;
int global_timer;
char tmp_vdd[10];
char tmp_freq[10];
volatile int global_tick;
volatile bool tt_measure;
volatile bool tt_clear;
volatile bool tt_clear_bottom;
volatile char tmp_raw_temp[10];
volatile response_t rsp;
volatile complete_response_t cmpl_rsp;

// Function prototypes
void display_ints( int x, int y, int _data, unsigned int font_color );
void display_chars( int x, int y, char *text, unsigned int font_color );
void measure_temp ( void );
int read_thermo_3 ( void );
void clear_timer_zone( void );
void clear_bottom( void );

// Function definitions
void system_init( int timer_value )
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
                                &_GPIO_MODULE_USART3_PD89 );

    RXNEIE_USART3_CR1_bit = 1;
    NVIC_IntEnable( IVT_INT_USART3 );

    global_timer = timer_value;
    global_tick = timer_value;

    tt_measure = false;
    tt_clear = false;
    tt_clear_bottom = false;

    Delay_ms( 500 );
}

void init_timer2()
{
  RCC_APB1ENR.TIM2EN = 1;
  TIM2_CR1.CEN = 0;
  TIM2_PSC = 1124;
  TIM2_ARR = 63999;
  NVIC_IntEnable( IVT_INT_TIM2 );
  TIM2_DIER.UIE = 1;
  TIM2_CR1.CEN = 1;
}

void display_init()
{
    TFT_Init_ILI9341_8bit( 320, 240 );
    TFT_BLED = 1;
    TFT_Set_Pen( CL_WHITE, 1 );
    TFT_Set_Brush( 1,CL_WHITE,0,0,0,0 );
    TFT_Set_Font( TFT_defaultFont, CL_BLACK, FO_HORIZONTAL );
    TFT_Fill_Screen( CL_WHITE );
    TFT_Set_Pen( CL_BLACK, 1 );
    TFT_Line( 20,  40, 300,  40 );
    TFT_Line( 20,  60, 300,  60 );
    TFT_Line( 20,  96, 300,  96 );
    TFT_Line( 20, 220, 300, 220 );
    TFT_Set_Font( &HandelGothic_BT21x22_Regular, CL_RED, FO_HORIZONTAL );
    TFT_Write_Text( "Lora RF click", 95, 14 );
    TFT_Set_Font( &Verdana12x13_Regular, CL_BLACK, FO_HORIZONTAL );
    TFT_Write_Text("EasyMx PRO v7 for STM32", 19, 223);
    TFT_Set_Font( &Verdana12x13_Regular, CL_RED, FO_HORIZONTAL );
    TFT_Write_Text( "www.mikroe.com", 200, 223 );
    TFT_Set_Font( &HandelGothic_BT21x22_Regular, CL_RED, FO_HORIZONTAL );
    display_chars( 30, 180, " LAST RECEIVE BEFORE : ", CL_BLACK );
    display_chars( 260, 180, " sec", CL_BLACK );
}

// Clears current timer text
void clear_timer_zone()
{
    TFT_Set_Pen( CL_WHITE, 1 );
    TFT_Rectangle( 228, 178, 259, 194 );
    display_ints( 240, 180, global_timer - global_tick, CL_RED );
}

// Clears "MESSAGE SENT" text
void clear_bottom()
{
    TFT_Rectangle( 10, 198, 310, 218 );
    tt_clear_bottom = false;
}

void display_chars( int x, int y, char *text, unsigned int font_color )
{
    TFT_Set_Font( &Verdana12x13_Regular, font_color, FO_HORIZONTAL );
    TFT_Write_Text( text, x, y );
}

void display_ints( int x, int y, int _data, unsigned int font_color )
{
    char tmp_data[ 10 ];

    sprinti( tmp_data, "%d", _data );
    TFT_Set_Font( &Verdana12x13_Regular, font_color, FO_HORIZONTAL );
    TFT_Write_Text( tmp_data, x, y );
}

void measure_temp()
{
    char           tmp_temp[ 10 ];
    int            current_raw_temp;
    float          real_temp;

    lora_rx( 15000, tmp_temp );
    global_tick = global_timer;
    TFT_Rectangle( 10, 198, 310, 218 );
    display_chars( 80, 200, " TEMPERATURE RECEIVED ", CL_GREEN );

    current_raw_temp = xtoi( tmp_temp );
    real_temp = ( float )(  current_raw_temp * 0.0625f );
    TFT_Set_Font( &HandelGothic_BT21x22_Regular, CL_RED, FO_HORIZONTAL );
        sprintf( tmp_temp, "%2.1f C", real_temp );
      TFT_Rectangle( 130, 118, 220, 148 );
        TFT_Write_Text( tmp_temp, 135, 130 );

    } else if ( tmp_rx == BUSY_ ) {

        TFT_Rectangle( 10, 198, 310, 218 );
        display_chars( 120, 200, " RADIO BUSY ", CL_RED );

    } else if ( tmp_rx == RADIO_ERR ) {

        TFT_Rectangle( 10, 198, 310, 218 );
        display_chars( 110, 200, " RADIO ERROR ", CL_RED );
    }

    tt_measure = false;
}

void btn_7_press()
{
    tt_measure == true;
}

void main()
{
    char tmp_txt[256];

    system_init( 5 );
    Delay_ms ( 50 );
    lora_rf_init( false, false, process_response );
    display_init();
    init_timer2();
    EnableInterrupts();

    // Software version - get and display
    lora_sys_get_software_version( tmp_txt );
    display_chars( 55, 44, tmp_txt, CL_BLUE );

    // VDD level - get and display value
    vdd_lvl = lora_sys_get_vdd();
    sprinti( tmp_vdd, "%d", vdd_lvl );
    display_chars( 35, 64, "VDD level :", CL_BLACK );
    display_chars( 200, 64, tmp_vdd, CL_BLUE );
    display_chars( 270, 64, "mV", CL_BLUE );

    // Radio frequency - get and display value
    lora_radio_freq( GET_OPTION, tmp_freq );
    display_chars( 35, 80, "Radio frequency :", CL_BLACK );
    display_chars( 200, 80, tmp_freq , CL_BLUE );
    display_chars( 270, 80, "Hz", CL_BLUE );


    lora_pause();
    lora_radio_wdog_timeout( SET_OPTION, "0" );

    while( 1 )
    {
        lora_rf_process();

        if ( Button( &GPIOC_IDR, 7, 80, 1 ) )
            btn_7_press();

        if ( tt_measure == true )
            measure_temp();

        if ( tt_clear_bottom == true )
            clear_bottom();
    }
}

// UART interrupt
void LO_RX_ISR() iv IVT_INT_USART3 ics ICS_AUTO
{
     if( RXNE_USART3_SR_bit )
     {
         char tmp = USART3_DR;
         lora_rx_isr( tmp );
     }
}

// Timer interrupt
void Timer2_interrupt() iv IVT_INT_TIM2
{
    TIM2_SR.UIF = 0;
    global_tick--;

    clear_timer_zone();

    if ( global_tick == 0 )
        tt_measure = true;

    if ( global_timer - global_tick == 3 )
        tt_clear_bottom = true;
}