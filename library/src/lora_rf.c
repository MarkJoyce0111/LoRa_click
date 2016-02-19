/******************************************************************************
* Title                 :   LORA RF
* Filename              :   lora_rf.c
* Author                :   MSV
* Origin Date           :   10/10/2015
* Notes                 :   None
*******************************************************************************/
/*************** MODULE REVISION LOG ******************************************
*
*    Date    Software Version    Initials   Description
*  16/10/15         1.1             MSV      Module Created.
*
*******************************************************************************/

/**
 * @file rf_lora.c
 *
 * @brief Functions for MikroElektronika's LoRa RF click board.
 */
 
/******************************************************************************
* Includes
*******************************************************************************/
#include "lora_rf.h"
/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/
static const char bands[ 2 ][ 4 ] =
{
    "433", "868"
};

static const char payloads[ 2 ][ 6 ] =
{
    "cnf", "uncnf"
};

static const char join_modes[ 2 ][ 5 ] =
{
    "otaa", "abp"
};

static const char states[ 2 ][ 4 ] =
{
    "on", "off"
};
/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static char _response[ MAX_RSP_SIZE + MAX_DATA_SIZE ];
/******************************************************************************
* Function Prototypes
*******************************************************************************/
static void ( *_callback )( char *response );

void _lora_reponse( char *rsp )
{
    strcpy( _response, rsp );

    // DBG
    //UART5_Write_Text( rsp );
    //UART5_Write_Text( "\r\n" );

    if ( _callback != NULL )
        _callback( _response );
}
/******************************************************************************
* Function Definitions
*******************************************************************************/
void lora_rf_init()
{
    _callback = NULL;

    lora_rf_set_callback( _lora_reponse );
    lora_rf_hw_init();
}

void lora_rf_callback( void ( *rsp_p )( char *rsp ) )
{
    _callback = rsp_p;
}

int lora_rf_sleep( uint32_t time_ms )
{
    char tmp[ 15 ] = { 0 };

    if ( time_ms < SLEEP_TIME_MIN )
        return -1;

    LongToStr( time_ms, tmp );
    lora_rf_cmd_parse( CMD_LVL_SYS, CMD_CTL_SLEEP, 0, Ltrim( tmp ) );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_reset()
{
    lora_rf_cmd_parse( CMD_LVL_SYS, CMD_CTL_RESET, 0, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();
        
    return 0;
}

int lora_rf_f_reset()
{
    lora_rf_cmd_parse( CMD_LVL_SYS, CMD_CTL_FACT_RST, 0, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    return 0;
}

int lora_rf_eeprom( uint16_t address,
                    char *buffer )
{
    char *tmp = buffer;
    char addr_txt[5] = { 0 };

    if( address < EEPROM_ADDRESS_MIN || address > EEPROM_ADDRESS_MAX )
        return -1;

    IntToHex( address, addr_txt );
    lora_rf_cmd_parse( CMD_LVL_SYS, CMD_CTL_GET, CMD_OPT_NVM, addr_txt );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_eeprom( uint16_t address,
                        char *buffer )
{
    char *tmp = buffer;
    char addr_txt[5] = { 0 };
    char temp_args[6] = { 0 };

    if( address < EEPROM_ADDRESS_MIN && address > EEPROM_ADDRESS_MAX )
        return -1;

    if( strlen( tmp ) != 2 )
        return -2;

    IntToHex( address, addr_txt );
    strcpy( temp_args, "" );
    strcat( temp_args, Ltrim( addr_txt ) );
    strcat( temp_args, " " );
    strcat( temp_args, tmp );
    lora_rf_cmd_parse( CMD_LVL_SYS, CMD_CTL_SET, CMD_OPT_NVM, temp_args );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if( strcmp( _response, "ok" ) )
        return 1;
        
    return 0;
}

int lora_rf_fw_ver( char *buffer )
{
    char *tmp = buffer;
    
    lora_rf_cmd_parse( CMD_LVL_SYS, CMD_CTL_GET, CMD_OPT_VER, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();
    
    strcpy( tmp, _response );

    return 0;
}

int lora_rf_vdd( char *buffer )
{
    char *tmp = buffer;
    
    lora_rf_cmd_parse( CMD_LVL_SYS, CMD_CTL_GET, CMD_OPT_VDD, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );
    
    return 0;
}

int lora_rf_hw_eui( char *buffer )
{
    char *tmp = buffer;
    
    lora_rf_cmd_parse( CMD_LVL_SYS, CMD_CTL_GET, CMD_OPT_HWEUI, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_mac_reset( BND_t band )
{
    if ( !band )
    {
        lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_RESET, 0, "433" );

    } else {

        lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_RESET, 0, "868" );
    }

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if( strcmp( _response, "ok" ) )
        return 1;
        
    return 0;
}

int lora_rf_mac_tx( PL_t pld,
                    uint8_t port_no,
                    char *buffer )
{
    char *tmp = buffer;
    char port_no_txt[ 5 ] = { 0 };
    char temp_data[ MAX_CMD_SIZE + MAX_DATA_SIZE ] = { 0 };

    if( port_no < PORT_NO_MIN || port_no > PORT_NO_MAX )
        return -1;

    if( strlen( tmp ) > 256 )
        return -2;

    if ( !pld )
    {
        strcpy( temp_data, "cnf" );

    } else {

        strcpy( temp_data, "uncnf" );
    }

    ByteToStr( port_no, port_no_txt );
    strcat( temp_data, " " );
    strcat( temp_data, Ltrim( port_no_txt ) );
    strcat( temp_data, " " );
    strcat( temp_data, tmp );
    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_TX, 0, temp_data );

    while ( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
    {
        return 1;

    } else {

        *tmp = '\0';

        while ( strncmp( _response, "mac_tx_ok", 9 ) )
        {
            lora_rf_set_rsp_rdy( false );

            while ( !lora_rf_rsp_rdy() )
                lora_rf_process();

            if( !strncmp( _response, "mac_rx", 6 ) )
            {
                strcat( tmp, &_response[ 6 ] );
            }
        }
    }

    return 0;
}

int lora_rf_join( JM_t mode )
{
    if ( !mode )
        lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_JOIN, 0, "otaa" );
    else
        lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_JOIN, 0, "abp" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
    {
        return 1;

    } else {

        lora_rf_set_rsp_rdy( false );

        while ( !lora_rf_rsp_rdy() )
            lora_rf_process();
    }

    if ( strcmp( _response, "accepted" ) )
        return 1;

    return 0;
}

int lora_rf_save()
{
    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_SAVE, 0, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_force_en()
{
    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_FORCE_EN, 0, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_pause( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_PAUSE, 0, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_resume()
{
    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_RESUME, 0, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_set_dev_addr( char *buffer )
{
    char *tmp = buffer;

    if( strlen( tmp ) != 8 )
        return -1;

    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_SET, CMD_OPT_DEV_ADDR, tmp );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_dev_addr( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_GET, CMD_OPT_DEV_ADDR, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_dev_eui( char *buffer )
{
    char *tmp = buffer;

    if( strlen( tmp ) != 16 )
        return -1;

    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_SET, CMD_OPT_DEV_EUI, tmp );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_dev_eui( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_GET, CMD_OPT_DEV_EUI, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_app_eui( char *buffer )
{
    char *tmp = buffer;

    if( strlen( tmp ) != 16 )
        return -1;

    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_SET, CMD_OPT_APP_EUI, tmp );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_app_eui( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_GET, CMD_OPT_APP_EUI, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_pwridx( uint8_t pwr_idx )
{
    char tmp[ 5 ] = { 0 };

    if( pwr_idx > POWER_INDEX_MAX )
        return -1;
    
    ByteToStr( pwr_idx, tmp );
    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_SET, CMD_OPT_PWRIDX, Ltrim( tmp ) );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}
int lora_rf_pwridx( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_GET, CMD_OPT_PWRIDX, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_dr( uint8_t dr )
{
    char tmp[ 5 ] = { 0 };

    if( dr > DATA_RATE_MAX )
        return -1;

    ByteToStr( dr, tmp );
    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_SET, CMD_OPT_DR, Ltrim( tmp ) );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_dr( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_GET, CMD_OPT_DR, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_adr( STATUS_t status )
{
    if ( !status )
    {
        lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_SET, CMD_OPT_ADR, "on" );

    } else {

        lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_SET, CMD_OPT_ADR, "off" );
    }

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_adr( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_GET, CMD_OPT_ADR, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_retr( uint8_t retr )
{
    char tmp[ 5 ] = { 0 };
    
    ByteToStr( retr, tmp );
    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_SET, CMD_OPT_RETX, Ltrim( tmp ) );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_retr( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_GET, CMD_OPT_RETX, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_rx_1_dly( uint16_t dly )
{
    char tmp[ 10 ] = { 0 };
    
    IntToStr( dly, tmp );
    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_SET, CMD_OPT_RXDELAY1, 
                       Ltrim( tmp ) );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_rx_1_dly( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_GET, CMD_OPT_RXDELAY1, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_ar( STATUS_t status )
{
    if ( !status )
    {
        lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_SET, CMD_OPT_AR, "on" );

    } else {

        lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_SET, CMD_OPT_AR, "off" );
    }

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_ar( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_GET, CMD_OPT_AR, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_rx2( uint8_t data_rate,
                     uint32_t freq )
{
    char dr_txt[ 5 ] = { 0 };
    char freq_txt[ 15 ] = { 0 };
    char temp_args[ 20 ] = { 0 };

    if ( data_rate > DATA_RANGE_MAX )
        return -1;

    ByteToStr( data_rate, dr_txt );
    LongToStr( freq, freq_txt );
    strcpy( temp_args, Ltrim( dr_txt ) );
    strcat( temp_args, " " );
    strcat( temp_args, Ltrim( freq_txt ) );
    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_SET, CMD_OPT_RX_2, temp_args );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_rx2( BND_t band,
                 char *buffer )
{
    char *tmp = buffer;

    if ( !band )
    {
        lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_GET, CMD_OPT_RX_2, "433" );

    } else {

        lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_GET, CMD_OPT_RX_2, "868" );
    }

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_ch_freq( uint8_t ch,
                         uint32_t freq )
{
    char ch_txt[ 5 ] = { 0 };
    char freq_txt[ 15 ] = { 0 };
    char temp_args[ 20 ] = { 0 };

    if ( ch < CHANNEL_ID_MIN || ch > CHANNEL_ID_MAX )
        return -1;

    if( ! ( ( freq >= RX_2_433_MIN && freq <= RX_2_433_MAX ) ||
            ( freq >= RX_2_868_MIN && freq <= RX_2_868_MAX ) ) )
        return -2;

    ByteToStr( ch, ch_txt );
    LongToStr( freq, freq_txt );
    strcat( temp_args, Ltrim( ch_txt ) );
    strcat( temp_args, " " );
    strcat( temp_args, Ltrim( freq_txt ) );
    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_SET, CMD_OPT_CH_FREQ, temp_args );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_ch_freq( uint8_t ch,
                     char *buffer )
{
    char *tmp = buffer;
    char ch_txt[ 5 ] = { 0 };

    if ( ch < CHANNEL_ID_MIN || ch > CHANNEL_ID_MAX )
        return -1;

    ByteToStr( ch, ch_txt );
    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_GET, CMD_OPT_CH_FREQ, 
                       Ltrim( ch_txt ) );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_ch_dcycle( uint8_t ch,
                           uint16_t dc )
{
    char ch_txt[ 5 ] = { 0 };
    char dc_txt[ 10 ] = { 0 };
    char temp_args[ 15 ] = { 0 };

    if ( ch > CHANNEL_ID_MAX )
        return -1;

    ByteToStr( ch, ch_txt );
    IntToStr( dc, dc_txt );
    strcat( temp_args, Ltrim( ch_txt ) );
    strcat( temp_args, " " );
    strcat( temp_args, Ltrim( dc_txt ) );
    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_SET, CMD_OPT_CH_DCYCLE, temp_args );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_ch_dcycle( uint8_t ch,
                       char *buffer )
{
    char *tmp = buffer;
    char ch_txt[ 5 ] = { 0 };

    if ( ch > CHANNEL_ID_MAX )
        return -1;

    ByteToStr( ch, ch_txt );
    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_GET, CMD_OPT_CH_DCYCLE, 
                       Ltrim( ch_txt ) );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_ch_dr_rng( uint8_t ch,
                           uint8_t min,
                           uint8_t max )
{
    char ch_txt[ 5 ] = { 0 };
    char min_txt[ 5 ] = { 0 };
    char max_txt[ 5 ] = { 0 };
    char temp_args[15] = { 0 };

    if( ch > CHANNEL_ID_MAX )
        return -1;

    if( min > DATA_RANGE_MAX )
        return -2;

    if( max > DATA_RANGE_MAX )
        return -3;

    if( min >= max )
        return -4;

    ByteToStr( ch, ch_txt );
    ByteToStr( min, min_txt );
    ByteToStr( max, max_txt );
    strcat( temp_args, Ltrim( ch_txt ) );
    strcat( temp_args, " " );
    strcat( temp_args, Ltrim( min_txt ) );
    strcat( temp_args, " " );
    strcat( temp_args, Ltrim( max_txt ) );
    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_SET, CMD_OPT_CH_DRRANGE, temp_args );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_ch_dr_rng( uint8_t ch,
                       char *buffer )
{
    char *tmp = buffer;
    char ch_txt[5] = { 0 };

    if( ch > CHANNEL_ID_MAX )
        return -1;

    ByteToStr( ch, ch_txt );

    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_GET, CMD_OPT_CH_DRRANGE, 
                       Ltrim( ch_txt ) );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_ch_st( uint8_t ch,
                       STATUS_t status )
{
    char ch_txt[ 5 ] = { 0 };
    char temp_args[ 10 ] = { 0 };

    if( ch > CHANNEL_ID_MAX )
        return -1;

    ByteToStr( ch, ch_txt );
    strcpy( temp_args, Ltrim( ch_txt ) );
    strcat( temp_args, " " );

    if ( !status )
    {
        strcat( temp_args, "on" );

    } else {

        strcat( temp_args, "off" );
    }

    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_SET, CMD_OPT_CH_STATUS, temp_args );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_ch_st( uint8_t ch,
                   char *buffer )
{
    char *tmp = buffer;
    char ch_txt[ 5 ] = { 0 };

    if( ch > CHANNEL_ID_MAX )
        return -1;

    ByteToStr( ch, ch_txt );
    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_GET, CMD_OPT_CH_STATUS, 
                       Ltrim( ch_txt ) );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_ns_key( char *buffer )
{
    char *tmp = buffer;

    if( strlen( tmp ) != 32 )
        return -1;

    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_SET, CMD_OPT_NWKSKEY, tmp );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_set_as_key( char *buffer )
{
    char *tmp = buffer;

    if( strlen( tmp ) != 32 )
        return -1;

    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_SET, CMD_OPT_APPSKEY, tmp );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_set_app_key( char *buffer )
{
    char *tmp = buffer;

    if( strlen( tmp ) != 32 )
        return -1;

    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_SET, CMD_OPT_APPKEY, tmp );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_set_battery( uint8_t bat_lvl )
{
    char tmp[5] = { 0 };
    
    ByteToStr( bat_lvl, tmp );
    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_SET, CMD_OPT_BAT, Ltrim( tmp ) );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_set_lnk_chk( uint16_t seconds )
{
    char tmp[10] = { 0 };

    IntToStr( seconds, tmp );
    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_SET, CMD_OPT_LINKCHK, 
                       Ltrim( tmp ) );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_band( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_GET, CMD_OPT_BAND, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_rx_2_dly( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_GET, CMD_OPT_RXDELAY2, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_dcycle_ps( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_GET, CMD_OPT_DCYCLEPS, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_dem_marg( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_GET, CMD_OPT_MRGN, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_status( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_MAC, CMD_CTL_GET, CMD_OPT_STAT, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_rx( uint16_t window_size,
                char *buffer )
{
    char *tmp = buffer;
    char window_size_txt[10] = { 0 };

    IntToStr( window_size, window_size_txt );
    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_RX, 0, Ltrim( window_size_txt ) );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
    {
        return 1;

    } else {

        lora_rf_set_rsp_rdy( false );

        while( !lora_rf_rsp_rdy() )
            lora_rf_process();

        if( strncmp( _response, "radio_rx", 8 ) )
        {
            return 1;
            
        } else {

            strcpy( tmp, _response );
        }
    }
    
    return 0;
}

int lora_rf_tx( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_TX, 0, tmp );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
    {
        return 1;

    } else {

        while( strncmp( _response, "radio_tx_ok", 11 ) )
        {
            lora_rf_set_rsp_rdy( false );

            while( !lora_rf_rsp_rdy() )
                lora_rf_process();
        }
    }

    return 0;
}

int lora_rf_cw( STATUS_t status )
{
    if ( !status )
    {
        lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_CW, 0, "on" );

    } else {

        lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_CW, 0, "off" );
    }

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_bt( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_GET, CMD_OPT_BT, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_bt( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_SET, CMD_OPT_BT, tmp );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_mod( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_GET, CMD_OPT_MOD, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_mod( MOD_t modulation )
{
    if ( !modulation )
    {
        lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_SET, CMD_OPT_MOD, "lora" );

    } else {

        lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_SET, CMD_OPT_MOD, "fsk" );
    }

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_set_freq( uint32_t freq )
{
    char freq_txt[ 10 ] = { 0 };

    if( ! ( ( freq >= RADIO_433_MIN && freq <= RADIO_433_MAX ) ||
      ( freq >= RADIO_868_MIN && freq <= RADIO_868_MAX ) ) )
        return -2;

    LongToStr( freq, freq_txt );
    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_SET, CMD_OPT_FREQ, 
                       Ltrim( freq_txt ) );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_freq( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_GET, CMD_OPT_FREQ, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_power( int8_t pwr )
{
    char pwr_txt[ 5 ] = { 0 };

    if( pwr < POWER_OUT_MIN || pwr > POWER_OUT_MAX )
        return -1;

    ByteToStr( pwr, pwr_txt );
    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_SET, CMD_OPT_PWR, 
                       Ltrim( pwr_txt ) );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_power( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_GET, CMD_OPT_PWR, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_sf( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_SET, CMD_OPT_SF, tmp );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_sf( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_GET, CMD_OPT_SF, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_freq_corr( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_SET, CMD_OPT_AFCBW, tmp );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_freq_corr( char *buffer )
{
    char *tmp = buffer;
    
    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_GET, CMD_OPT_AFCBW, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_rx_bw( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_SET, CMD_OPT_RXBW, tmp );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_rx_bw( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_GET, CMD_OPT_RXBW, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_fsk_br( uint16_t br )
{
    char br_txt[ 10 ] = { 0 };

    IntToStr( br, br_txt );
    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_SET, CMD_OPT_BITRATE, 
                       Ltrim( br_txt ) );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_fsk_br( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_GET, CMD_OPT_BITRATE, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_freq_dev( uint16_t freq_dev )
{
    char freq_txt[ 10 ] = { 0 };

    IntToStr( freq_dev, freq_txt );
    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_SET, CMD_OPT_FDEV, 
                       Ltrim( freq_txt ) );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_freq_dev( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_GET, CMD_OPT_FDEV, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_pr_len( uint16_t pr_len )
{
    char pr_len_txt[ 10 ] = { 0 };

    IntToStr( pr_len, pr_len_txt );
    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_SET, CMD_OPT_PRLEN, 
                       Ltrim( pr_len_txt ) );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_pr_len( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_GET, CMD_OPT_PRLEN, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_crc( STATUS_t status )
{
    if ( !status )
    {
        lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_SET, CMD_OPT_CRC, "on" );

    } else {

        lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_SET, CMD_OPT_CRC, "off" );
    }

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_crc( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_GET, CMD_OPT_CRC, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_iq_inv( STATUS_t status )
{
    if ( status )
    {
        lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_SET, CMD_OPT_IQI, "on" );

    } else {

        lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_SET, CMD_OPT_IQI, "on" );
    }

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_iq_inv( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_GET, CMD_OPT_IQI, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_cr( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_SET, CMD_OPT_C_RATE, tmp );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_cr( char *buffer )
{
    char *tmp = buffer;
    
    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_GET, CMD_OPT_C_RATE, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_wdog( uint32_t wdog )
{
    char wdog_txt[ 15 ] = { 0 };

    LongToStr( wdog, wdog_txt );
    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_SET, CMD_OPT_WDT, 
                       Ltrim( wdog_txt ) );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_wdog( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_GET, CMD_OPT_WDT, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_set_bw( uint16_t bw )
{
    char bw_txt[ 10 ] = { 0 };

    if ( bw != 125 && bw != 250 && bw != 500 )
        return -1;

    IntToStr( bw, bw_txt );
    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_SET, CMD_OPT_BW, 
                       Ltrim( bw_txt ) );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_bw( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_GET, CMD_OPT_BW, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}

int lora_rf_syncw( char *syncword )
{
    char *tmp = syncword;

    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_SET, CMD_OPT_SYNC, tmp );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    if ( strcmp( _response, "ok" ) )
        return 1;

    return 0;
}

int lora_rf_snr( char *buffer )
{
    char *tmp = buffer;

    lora_rf_cmd_parse( CMD_LVL_RADIO, CMD_CTL_GET, CMD_OPT_SNR, "" );

    while( !lora_rf_rsp_rdy() )
        lora_rf_process();

    strcpy( tmp, _response );

    return 0;
}
/*************** END OF FUNCTIONS *********************************************/