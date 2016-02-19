/*******************************************************************************
* Title                 :   LORA RF CLICK ENGINE
* Filename              :   lora_rf_engine.c
* Author                :   MSV
* Origin Date           :   10/10/2015
* Notes                 :   None
*******************************************************************************/
/*************** MODULE REVISION LOG ******************************************
*
*    Date    Software Version    Initials   Description
*  24/11/15         2.0            MSV      Module Created.
*
*******************************************************************************/
/**
 * @brief Main library of the module hardware layer.
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "lora_rf_hw.h"
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

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* Command Strings */
static const char operation[3][5] =
{
    "", "set", "get"
};
static const char cmd_lvl[4][6] =
{
    "", "sys", "mac", "radio"
};
static const char cmd_ctl[15][13] =
{
    "", "sleep", "reset", "eraseFW", "factoryRESET", "join", "save",
    "forceENABLE", "pause", "resume", "rx", "tx", "cw", "get", "set"
};
static const char cmd_opt[50][15] =
{
    "", "pindig", "nvm", "ver", "vdd", "hweui", "nwkskey", "appskey",
    "appkey", "bat", "linkchk", "ch freq", "ch dcycle", "ch drrange",
    "ch status", "devaddr", "deveui", "appeui", "dr", "pwridx", "adr",
    "retx", "ar", "rx2", "band", "rxdelay1", "rxdelay2", "dcycleps",
    "mrgn", "gwnb", "status", "ch", "sync", "bt", "mod", "freq", "pwr",
    "sf", "afcbw", "rxbw", "bitrate", "fdev", "prlen", "crc", "iqi",
    "cr", "wdt", "bw", "snr"
};

/* Buffers */
static volatile int             _rx_buffer_len;
static volatile char            _tx_buffer[ MAX_CMD_SIZE + MAX_DATA_SIZE ];
static volatile char            _rx_buffer[ MAX_RSP_SIZE + MAX_DATA_SIZE ];

/* Timer Flags and Counter */
static volatile bool            _timer_f;
static volatile bool            _timeout_f;
static volatile bool            _timer_use_f;
static volatile uint32_t        _ticker;
static volatile uint32_t        _timer_max;

/* Process Flags */
static volatile bool            _rsp_rdy_f;
static volatile bool            _rsp_cue_f;
static volatile bool            _cmd_rdy_f;
static volatile bool            _cmd_cue_f;
static volatile bool            _rsp_f;
/******************************************************************************
* Function Prototypes
*******************************************************************************/
/* Callback Function Pointer */
static void ( *_callback_resp )( char *response );

/* String functions for constants */
static void _strcpy_const( char* destination,
                           const char* source );

static void _strcat_const( char* destination,
                           const char* source );
/******************************************************************************
* Function Definitions - Private
*******************************************************************************/
static void _strcpy_const( char* dst, const char* src )
{
    while ( ( *( dst++ ) = *( src++ ) ) != '\0' );
}

static void _strcat_const( char* dst, const char* src )
{
    _strcpy_const( &dst[ strlen( dst ) ], src );
}
/******************************************************************************
* Function Definitions - Public
*******************************************************************************/
void lora_rf_hw_init()
{
    lora_rf_hal_init();
    lora_rf_hal_rst_high();
    Delay_ms( 100 );
    lora_rf_hal_rst_low();
    Delay_ms( 100 );
    lora_rf_hal_rst_high();
    
    memset( _tx_buffer, 0, MAX_CMD_SIZE + MAX_DATA_SIZE );
    memset( _rx_buffer, 0, MAX_RSP_SIZE + MAX_DATA_SIZE );
    
    _timer_f = false;
    _timeout_f = false;
    _timer_use_f = true;
    _ticker = 0;
    _timer_max = TIMER_EXPIRED;

    _rsp_rdy_f = false;
    _rsp_cue_f = false;
    _cmd_rdy_f = false;
    _cmd_cue_f = false;
    
    while( !_rsp_rdy_f )
        lora_rf_process();
}

void lora_rf_set_callback( void ( *response_p )( char *response ) )
{
    _callback_resp = response_p;
}

void lora_rf_process( void )
{
    if ( _cmd_rdy_f && !_timer_f )
    {
        lora_rf_hal_write_byte( _tx_buffer );
        _rsp_rdy_f = false;
        _cmd_rdy_f = false;
        _timer_f = true;
    }

    if ( _rsp_cue_f )
    {
        _callback_resp( _rx_buffer );
        _rx_buffer_len = 0;
        _timer_f = false;
        _rsp_cue_f = false;
        _rsp_rdy_f = true;
    }

    if ( _timeout_f )
    {
        _callback_resp( "timeout" );
        _rx_buffer_len = 0;
        _timer_f = false;
        _rsp_cue_f = false;
        _rsp_rdy_f = true;
    }
}

void lora_rf_cmd_send( char *cmd,
                       char *args )
{
    strcpy( _tx_buffer, cmd );

    if ( *args != '\0' )
    {
        strcat( _tx_buffer, " " );
        strcat( _tx_buffer, args );
    }

    strcat( _tx_buffer, "\r" );
    strcat( _tx_buffer, "\n" );

    _cmd_rdy_f = true;
    _rsp_rdy_f = false;
}

void lora_rf_cmd_parse( CMD_LVL_t level,
                        CMD_CTL_t control,
                        CMD_OPT_t option,
                        char *args )
{
    _strcpy_const( _tx_buffer, cmd_lvl[ level ] );

    if ( control )
    {
        strcat( _tx_buffer, " " );
        _strcat_const( _tx_buffer, cmd_ctl[ control ] );
    }

    if ( option )
    {
        strcat( _tx_buffer, " " );
        _strcat_const( _tx_buffer, cmd_opt[ option ] );
    }

    if ( strlen( args ) )
    {
       strcat( _tx_buffer, " " );
       strcat( _tx_buffer, args );
    }

    strcat( _tx_buffer, "\r\n" );

    _cmd_rdy_f = true;
    _rsp_rdy_f = false;
}

void lora_rf_rx_isr( char rx_input )
{
    static bool _rx_sentence_f;

    if( rx_input != '\r' && rx_input != '\n' )
    {
        _rx_buffer[ _rx_buffer_len++ ] = rx_input;

    } else if ( rx_input == '\r' ) {

        _rx_sentence_f = true;

    } else if ( rx_input == '\n' && _rx_sentence_f ) {

        _rx_buffer[ _rx_buffer_len++ ] = '\0';
        _rx_sentence_f = false;
        _rsp_cue_f = true;
        
    } else {

    }
}

void lora_rf_tick_isr()
{
    if ( _timer_use_f )
    {
        if ( _timer_f && ( _ticker++ > _timer_max ) )
        {
            _timeout_f = true;
            _cmd_cue_f = false;
        }
    }
}

void lora_rf_timer_conf( uint32_t timer_limit )
{
    if ( timer_limit )    //write_uart_p( data_in );
    {
        _timer_max = timer_limit;
        _timer_use_f = true;

    } else {

        _timer_max = TIMER_EXPIRED;
        _timer_use_f = false;
    }
}

void lora_rf_set_rsp_rdy( bool value )
{
    _rsp_rdy_f = value;
}

bool lora_rf_rsp_rdy()
{
    return _rsp_rdy_f;
}
/*************** END OF FUNCTIONS *********************************************/
