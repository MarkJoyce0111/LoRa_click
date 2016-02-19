/*******************************************************************************
* Title                 :   LORA RF HAL
* Filename              :   lora_rf_hal.c
* Author                :   MSV
* Origin Date           :   02/12/2015
* Notes                 :   None
*******************************************************************************/
/*************** MODULE REVISION LOG ******************************************
*
*    Date    Software Version    Initials   Description
*  02/12/15    XXXXXXXXXXX         MSV      Module Created.
*
*******************************************************************************/
/**
 * @brief This module contains the hal layer for Mikroelektronika's RF Lora
 *  click board.
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "lora_rf_hal.h"
/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/
#if defined ( __MIKROC_PRO_FOR_ARM__ )       || \
    defined ( __MIKROC_PRO_FOR_PIC32__ )
static void ( *write_uart_p )( unsigned int _data );
static void ( *write_text_p )( unsigned char *_data );
#elif defined ( __MIKROC_PRO_FOR_PIC__ )     || \
      defined ( __MIKROC_PRO_FOR_FT90x__ )
static void ( *write_uart_p )( unsigned char _data );
#endif
/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
//static write_uart_t _hal_uart_write;

#if defined( __MIKROC_PRO_FOR_ARM__ )      || \
       defined( __MIKROC_PRO_FOR_AVR__ )   || \
       defined( __MIKROC_PRO_FOR_PIC__ )   || \
       defined( __MIKROC_PRO_FOR_PIC32__ ) || \
       defined( __MIKROC_PRO_FOR_DSPIC__ ) || \
       defined( __MIKROC_PRO_FOR_8051__ )  || \
       defined( __MIKROC_PRO_FOR_FT90x__ )
extern sfr sbit RF_LORA_RST;
extern sfr sbit RF_LORA_RTS;
extern sfr sbit RF_LORA_CTS;
#endif
/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
void lora_rf_hal_rst_high()
{
    RF_LORA_RST = 1;
}

void lora_rf_hal_rst_low()
{
    RF_LORA_RST = 0;
}

void lora_rf_hal_rts_high()
{
    RF_LORA_RTS = 1;
}

void lora_rf_hal_rts_low()
{
    RF_LORA_RTS = 0;
}

void lora_rf_hal_cts_high()
{
    RF_LORA_CTS = 1;
}

void lora_rf_hal_cts_low()
{
    RF_LORA_CTS = 0;
}

void lora_rf_hal_init()
{
    write_uart_p = UART_Wr_Ptr;
    write_text_p = UART_Write_Text;
}

void lora_rf_hal_write_byte( char *data_in )
{
    write_text_p( data_in );
}

/*************** END OF FUNCTIONS ***************************************************************************/
