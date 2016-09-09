/*******************************************************************************
* Title                 :   LORA LICK
* Filename              :   lora.c
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

#include "lora.h"
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

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/

/* Buffers */
static volatile char            _tx_buffer[ MAX_TRANSFER_SIZE ];
static volatile char            _rx_buffer[ MAX_TRANSFER_SIZE ];
static volatile int             _rx_buffer_len;

/* Timer Flags and Counter */
static volatile bool            _timer_f;
static volatile bool            _timeout_f;
static volatile bool            _timer_use_f;
static volatile uint32_t        _ticker;
static volatile uint32_t        _timer_max;

/* Process Flags */
static volatile bool            _rsp_rdy_f;
static volatile bool            _lora_rdy_f;

/* Response vars */
static bool                     _rsp_f;
static char**                   _rsp_buffer;
static bool                     _callback_default;
static void ( *_callback_resp )( char *response );

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/* String functions for constants */
static void _strcpy_const
(
		char* dst,
        const char* src
);

static void _strcat_const
(
		char* dst,
        const char* src
);

static void _lora_resp
(
		void
);

static int _lora_par
(
		void
);

static void _lora_read
(
        void
);

static void _lora_write
(
        void
);

/******************************************************************************
* Function Definitions - Private
*******************************************************************************/

static void _strcpy_const
(
		char* dst,
		const char* src
)
{
    while ( ( *dst++ = *src++ ) != '\0' );
}

static void _strcat_const
(
		char* dst,
		const char* src
)
{
    _strcpy_const( &dst[ strlen( dst ) ], src );
}

static void _lora_resp
(
		void
)
{
	_rx_buffer_len  = 0;
	_lora_rdy_f     = false;
	_rsp_rdy_f      = false;
	_rsp_f      	= true;
}

static int _lora_par
(
		void
)
{
	if( !strcmp( _rx_buffer, "invalid_param" ) )
	    return 1;
	if( !strcmp( _rx_buffer, "not_joined" ) )
	    return 2;
	if( !strcmp( _rx_buffer, "no_free_ch" ) )
	    return 3;
	if( !strcmp( _rx_buffer, "silent" ) )
	    return 4;
	if( !strcmp( _rx_buffer, "frame_counter_err_rejoin_needed" ) )
	    return 5;
	if( !strcmp( _rx_buffer, "busy" ) )
	    return 6;
	if( !strcmp( _rx_buffer, "mac_paused" ) )
	    return 7;
	if( !strcmp( _rx_buffer, "invalid_data_len" ) )
	    return 8;
	if( !strcmp( _rx_buffer, "keys_not_init" ) )
	    return 9;
	return 0;
}

static int _lora_repar
(
		void
)
{
	if( !strcmp( _rx_buffer, "mac_err" ) )
	    return 10;
	if( !strcmp( _rx_buffer, "mac_tx_ok" ) )
	    return 0;
	if( !strcmp( _rx_buffer, "mac_rx" ) )
	    return 12;
	if( !strcmp( _rx_buffer, "invalid_data_len" ) )
	    return 13;
	if( !strcmp( _rx_buffer, "radio_err" ) )
	    return 14;
	if( !strcmp( _rx_buffer, "radio_tx_ok" ) )
	    return 0;
	if( !strcmp( _rx_buffer, "radio_rx" ) )
	    return 0;
	if( !strcmp( _rx_buffer, "accepted" ) )
	    return 0;
	if( !strcmp( _rx_buffer, "denied" ) )
	    return 18;
	return 0;
}

static void _lora_write
(
        void
)
{
    char *ptr = _tx_buffer;

    while( *ptr )
        if( !lora_hal_rts() )
            lora_hal_write( *ptr++ );

    lora_hal_write( '\r' );
    lora_hal_write( '\n' );

    _rx_buffer_len  = 0;
    _lora_rdy_f     = false;
    _rsp_rdy_f      = false;
    _timer_f        = true;
    _rsp_f          = true;
}

static void _lora_read
(
        void
)
{
    if( !_rsp_f )
    {
        lora_hal_cts( true );
        _callback_resp( _rx_buffer );
        lora_hal_cts( false );

    } else if( _rsp_f ) {

        lora_hal_cts( true );
        strcpy( *_rsp_buffer, _rx_buffer );
        lora_hal_cts( false );
    }

    _lora_rdy_f     = true;
    _rsp_rdy_f      = false;
    _timer_f        = false;
    _rsp_f          = false;
}

/******************************************************************************
* Function Definitions - Public
*******************************************************************************/
void lora_init
(
        bool RTS_line,
        bool CTS_line,
        bool CB_default,
        void ( *response_p )( char *response )
)
{
    lora_hal_init( RTS_line, CTS_line );
    lora_hal_rst( 1 );
    Delay_ms( 100 );
    lora_hal_rst( 0 );
    Delay_ms( 100 );
    lora_hal_rst( 1 );
    Delay_ms( 500 );
    lora_hal_cts( 1 );
    
    memset( _tx_buffer, 0, MAX_CMD_SIZE + MAX_DATA_SIZE );
    memset( _rx_buffer, 0, MAX_RSP_SIZE + MAX_DATA_SIZE );
    
    _timer_max          = TIMER_EXPIRED;
    _rx_buffer_len      = 0;
    _ticker             = 0;
    _timer_f            = false;
    _timeout_f          = false;
    _timer_use_f        = false;
    _rsp_f              = false;
    _rsp_rdy_f          = false;
    _lora_rdy_f         = true;
    _callback_resp      = response_p;
    _callback_default   = CB_default;
}

void lora_process
(
        void
)
{
    if ( _rsp_rdy_f )
        _lora_read();

    if ( _timeout_f )
        _lora_read();
}

void lora_cmd
(
        char *cmd,
        char *args,
        char *response
)
{
    while( !_lora_rdy_f )
        lora_process();

    strcpy( _tx_buffer, cmd );
    
    if( *args != 0 )
    {
        strcat( _tx_buffer, " " );
        strcat( _tx_buffer, args );
    }

    _rsp_buffer = &response;
    _lora_write();

    while( !_lora_rdy_f )
        lora_process();
}

int lora_mac_tx
(
        PL_t payload,
        uint8_t port_no,
        char *buffer,
        char *response
)
{
    int res 	  = 0;
	char tmp[ 5 ] = { 0 };

    while( !_lora_rdy_f )
        lora_process();

    _strcpy_const( _tx_buffer, ( char* )LORA_MAC_TX );
    _strcat_const( _tx_buffer, _payload[ payload ]);
    ByteToStr( port_no, tmp );
    strcat( _tx_buffer, Ltrim( tmp ) );
    strcat( _tx_buffer, " " );
    strcat( _tx_buffer, buffer );
    _rsp_buffer = &response;
    _lora_write();

    while( !_lora_rdy_f )
        lora_process();

    if( ( res = _lora_par() ) )
        return res;

    _lora_resp();

    do {

    	while( !_lora_rdy_f )
    		lora_process();

    } while( ( res = _lora_repar() ) == 12 );

    return res;
}

int lora_join
(
        JM_t join_mode,
        char *response
)
{
	int res 	  = 0;

    while( !_lora_rdy_f )
        lora_process();

    _strcpy_const( _tx_buffer, ( char* )LORA_JOIN );
    _strcat_const( _tx_buffer, _join_mode[ join_mode ] );
    _rsp_buffer = &response;
    _lora_write();

    while( !_lora_rdy_f )
        lora_process();

    if( ( res = _lora_par() ) )
        return res;

    _lora_resp();

    while( !_lora_rdy_f )
        lora_process();

    return _lora_repar();
}

int lora_rx
(
        uint16_t window_size,
        char *response
)
{
	int res 	  = 0;
	char tmp[ 7 ] = { 0 };

    while( !_lora_rdy_f )
        lora_process();

    _strcpy_const( _tx_buffer, ( char* )LORA_RADIO_RX );
    IntToStr( window_size, tmp );
    strcat( _tx_buffer, Ltrim( tmp ) );
    _rsp_buffer = &response;
    _lora_write();

    while( !_lora_rdy_f )
        lora_process();

    if( ( res = _lora_par() ) )
        return res;

    _lora_resp();

    while( !_lora_rdy_f )
        lora_process();

    return _lora_repar();
}

int lora_tx
(
        char *buffer
)
{
	int res 	  = 0;

    while( !_lora_rdy_f )
        lora_process();

    _strcpy_const( _tx_buffer, ( char* )LORA_RADIO_TX );
    strcat( _tx_buffer, buffer );

    _rsp_buffer = &buffer;
    _lora_write();

    while( !_lora_rdy_f )
        lora_process();

    if( ( res = _lora_par() ) )
        return res;

    _lora_resp();

    while( !_lora_rdy_f )
        lora_process();

    return _lora_repar();
}

void lora_rx_isr( char rx_input )
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
        _rsp_rdy_f = true;
        
    } else {

    }
}

void lora_tick_isr()
{
    if( _timer_use_f )
        if( _timer_f && ( _ticker++ > _timer_max ) )
            _timeout_f = true;
}

void lora_tick_conf( uint32_t timer_limit )
{
    if ( timer_limit )
    {
        _timer_max = timer_limit;
        _timer_use_f = true;

    } else {

        _timer_max = TIMER_EXPIRED;
        _timer_use_f = false;
    }
}
/*************** END OF FUNCTIONS *********************************************/