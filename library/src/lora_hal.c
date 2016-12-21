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
#include "lora_hal.h"
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
#elif defined ( __MIKROC_PRO_FOR_PIC__ )     || \
      defined ( __MIKROC_PRO_FOR_FT90x__ )
static void ( *write_uart_p )( unsigned char _data );
#endif

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/

static int use_rts;
static int use_cts;

#if defined( __MIKROC_PRO_FOR_ARM__ )      || \
    defined( __MIKROC_PRO_FOR_AVR__ )   || \
    defined( __MIKROC_PRO_FOR_PIC__ )   || \
    defined( __MIKROC_PRO_FOR_PIC32__ ) || \
    defined( __MIKROC_PRO_FOR_DSPIC__ ) || \
    defined( __MIKROC_PRO_FOR_8051__ )  || \
    defined( __MIKROC_PRO_FOR_FT90x__ )
extern sfr sbit LORA_RST;
extern sfr sbit LORA_RTS;
extern sfr sbit LORA_CTS;
#endif

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/

void lora_hal_rst
(
        int logic
)
{
#if defined( __MIKROC_PRO_FOR_ARM__ )   || \
    defined( __MIKROC_PRO_FOR_AVR__ )   || \
    defined( __MIKROC_PRO_FOR_PIC__ )   || \
    defined( __MIKROC_PRO_FOR_PIC32__ ) || \
    defined( __MIKROC_PRO_FOR_DSPIC__ ) || \
    defined( __MIKROC_PRO_FOR_8051__ )  || \
    defined( __MIKROC_PRO_FOR_FT90x__ )
    if( logic )
        LORA_RST = 1;
    else
        LORA_RTS = 0;
#endif
}

int lora_hal_rts
(
        void
)
{
    if( use_rts )
    {
#if defined( __MIKROC_PRO_FOR_ARM__ )   || \
    defined( __MIKROC_PRO_FOR_AVR__ )   || \
    defined( __MIKROC_PRO_FOR_PIC__ )   || \
    defined( __MIKROC_PRO_FOR_PIC32__ ) || \
    defined( __MIKROC_PRO_FOR_DSPIC__ ) || \
    defined( __MIKROC_PRO_FOR_8051__ )  || \
    defined( __MIKROC_PRO_FOR_FT90x__ )
        return ( int )LORA_RTS;
#endif
    }
    else
        return 0;
}

void lora_hal_cts
(
        int stop
)
{
    if( use_cts )
    {
#if defined( __MIKROC_PRO_FOR_ARM__ )   || \
    defined( __MIKROC_PRO_FOR_AVR__ )   || \
    defined( __MIKROC_PRO_FOR_PIC__ )   || \
    defined( __MIKROC_PRO_FOR_PIC32__ ) || \
    defined( __MIKROC_PRO_FOR_DSPIC__ ) || \
    defined( __MIKROC_PRO_FOR_8051__ )  || \
    defined( __MIKROC_PRO_FOR_FT90x__ )
        if( stop )
            LORA_CTS = 1;
        else
            LORA_CTS = 0;
#endif
    }
    else {
#if defined( __MIKROC_PRO_FOR_ARM__ )   || \
    defined( __MIKROC_PRO_FOR_AVR__ )   || \
    defined( __MIKROC_PRO_FOR_PIC__ )   || \
    defined( __MIKROC_PRO_FOR_PIC32__ ) || \
    defined( __MIKROC_PRO_FOR_DSPIC__ ) || \
    defined( __MIKROC_PRO_FOR_8051__ )  || \
    defined( __MIKROC_PRO_FOR_FT90x__ )
        LORA_CTS = 0;
#endif
    }
}

void lora_hal_init
(
        int rts_use,
        int cts_use
)
{
#if defined( __MIKROC_PRO_FOR_ARM__ )      || \
    defined( __MIKROC_PRO_FOR_AVR__ )   || \
    defined( __MIKROC_PRO_FOR_PIC__ )   || \
    defined( __MIKROC_PRO_FOR_PIC32__ ) || \
    defined( __MIKROC_PRO_FOR_DSPIC__ ) || \
    defined( __MIKROC_PRO_FOR_8051__ )  || \
    defined( __MIKROC_PRO_FOR_FT90x__ )
    write_uart_p = UART_Wr_Ptr;
#endif

    use_rts = rts_use;
    use_cts = cts_use;
}

void lora_hal_write
(
        char ch
)
{
    write_uart_p( ch );
#ifdef _DEBUG_
    LOG_CH( ch );
#endif
}

/*************** END OF FUNCTIONS ***************************************************************************/