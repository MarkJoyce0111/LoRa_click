/*
    __lora_driver.c

-----------------------------------------------------------------------------

  This file is part of mikroSDK.

  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

----------------------------------------------------------------------------- */

#include "__lora_driver.h"
#include "__lora_hal.c"

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------------------------------------- MACROS */

/**
 * Timer Limit ( ms ) */
static const uint16_t LORA_TIMER_EXPIRED =  50000;
/**
 * Number of Response Indexes */
static const uint8_t LORA_RESP_COUNT = 100;
/**
 * Response String Max Size */
static const uint8_t LORA_MAX_RSP_SIZE = 20;
/**
 * Command String Max Size */
static const uint8_t LORA_MAX_CMD_SIZE = 64;
/**
 * Data String Max Size */
static const uint16_t LORA_MAX_DATA_SIZE = 256;
static const uint16_t LORA_MAX_TRANSFER_SIZE = 384;

/* Payload */
static const char _LORA_PL_CNF[7] = "cnf ";
static const char _LORA_PL_UNCNF[7] = "uncnf ";
/* Join mode */
static const char _LORA_JM_OTAA[5] = "otaa";
static const char _LORA_JM_ABP[5] = "abp";

char LORA_CMD_SYS_GET_VER[] = "sys get ver";
char LORA_CMD_MAC_PAUSE[] = "mac pause";
char LORA_CMD_RADIO_SET_WDT[] = "radio set wdt 0";
char LORA_ARG_0[] = "0";


/* ---------------------------------------------------------------- VARIABLES */

#ifdef   __LORA_DRV_I2C__
static uint8_t _slaveAddress;
#endif

/* Buffers */
static volatile char            _tx_buffer[ LORA_MAX_TRANSFER_SIZE ];
static volatile char            _rx_buffer[ LORA_MAX_TRANSFER_SIZE ];
static volatile uint16_t        _rx_buffer_len;

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
static char*                    _rsp_buffer;
static bool                     _callback_default;
static void ( *_callback_resp )( char *response );

/* -------------------------------------------- PRIVATE FUNCTION DECLARATIONS */

static uint8_t  _strlen(const char *s);
static void     _strcpy(char *dest, const char* src);
static void     _strcat(char *dest, const char *src);
static void     _memset(void *s, uint8_t c, size_t n);
static int8_t   _strcmp(const char* s1, const char* s2);

static void _lora_resp();
static uint8_t _lora_par();
static uint8_t _lora_repar();
static void _lora_write();
static void _lora_read();


/* --------------------------------------------- PRIVATE FUNCTION DEFINITIONS */


static void _lora_resp()
{
    _rx_buffer_len  = 0;
    _lora_rdy_f     = false;
    _rsp_rdy_f      = false;
    _rsp_f          = true;
}

static uint8_t _strlen(char *s)
{
    char *p = s;
    while (*s) ++s;
    return s - p;
}

void _strcpy(char *dest, char* src)
{
    char *ret = dest;
    while (*dest++ = *src++);
}

static void _strcat(char *dest,  char *src)
{
    char *ret = dest;
    while (*dest)
        dest++;
    while (*dest++ = *src++);
}
static void _strncpy(uint8_t *dest, uint8_t *src, uint16_t count)
{
    char *tmp  = dest;

    uint16_t i = 0;

    while (0 != count)
    {
        if (0 != (*tmp = src[i]))
        {
            i++;
        }
        tmp++;
        count--;
    }
}

/*void _xtoi(char* origin, uint8_t* result)
{
    uint8_t szlen = _strlen(origin);
    uint8_t idx, ptr, factor;

    if(szlen > 0)
    {
         *result = 0;
         factor = 1;

         for(idx = szlen-1; idx >= 0; --idx)
         {
               if(isxdigit( *(origin+idx)))
               {
                    if( *(origin + idx) >= 97)
                         ptr = ( *(origin + idx) - 97) + 10;
                    else if( *(origin + idx) >= 65)
                         ptr = ( *(origin + idx) - 65) + 10;
                    else
                         ptr = *(origin + idx) - 48;
                    *result += (ptr * factor);
                    factor *= 16;
                }

          }
     }
}*/

static void _memset(uint8_t *s, uint8_t c, size_t n)
{
    unsigned char *p = s;
    while(n--)
        *p++ = (unsigned char)c;
}

static int8_t _strcmp(char* s1, char* s2)
{
    while(*s1 && (*s1==*s2))
        s1++,s2++;
    return *( unsigned char*)s1-*( unsigned char*)s2;
}



static uint8_t _lora_par()
{
    if( !_strcmp( _rx_buffer, "invalid_param" ) )
        return 1;
    if( !_strcmp( _rx_buffer, "not_joined" ) )
        return 2;
    if( !_strcmp( _rx_buffer, "no_free_ch" ) )
        return 3;
    if( !_strcmp( _rx_buffer, "silent" ) )
        return 4;
    if( !_strcmp( _rx_buffer, "frame_counter_err_rejoin_needed" ) )
        return 5;
    if( !_strcmp( _rx_buffer, "busy" ) )
        return 6;
    if( !_strcmp( _rx_buffer, "mac_paused" ) )
        return 7;
    if( !_strcmp( _rx_buffer, "invalid_data_len" ) )
        return 8;
    if( !_strcmp( _rx_buffer, "keys_not_init" ) )
        return 9;
    return 0;
}
static uint8_t _lora_repar()
{
    if( !_strcmp( _rx_buffer, "mac_err" ) )
        return 10;
    if( !_strcmp( _rx_buffer, "mac_tx_ok" ) )
        return 0;
    if( !_strcmp( _rx_buffer, "mac_rx" ) )
        return 12;
    if( !_strcmp( _rx_buffer, "invalid_data_len" ) )
        return 13;
    if( !_strcmp( _rx_buffer, "radio_err" ) )
        return 14;
    if( !_strcmp( _rx_buffer, "radio_tx_ok" ) )
        return 0;
    if( !_strcmp( _rx_buffer, "radio_rx" ) )
        return 0;
    if( !_strcmp( _rx_buffer, "accepted" ) )
        return 0;
    if( !_strcmp( _rx_buffer, "denied" ) )
        return 18;
    return 0;
}

static void _lora_write()
{
    char *ptr = _tx_buffer;

    while( *ptr )
        if( !hal_gpio_intGet() )
            hal_uartWrite( *ptr++ );

    hal_uartWrite( '\r' );
    hal_uartWrite( '\n' );

    _rx_buffer_len  = 0;
    _lora_rdy_f     = false;
    _rsp_rdy_f      = false;
    _timer_f        = true;
    _rsp_f          = true;
}

static void _lora_read()
{
    if( !_rsp_f )
    {
        hal_gpio_csSet( true );
        _callback_resp( _rx_buffer );
        hal_gpio_csSet( false );

    } 
    else if( _rsp_f )
    {
        hal_gpio_csSet( true );
        _strcpy( _rsp_buffer, _rx_buffer );
        hal_gpio_csSet( false );
    }

    _lora_rdy_f     = true;
    _rsp_rdy_f      = false;
    _timer_f        = false;
    _rsp_f          = true;
}


/* --------------------------------------------------------- PUBLIC FUNCTIONS */

#ifdef   __LORA_DRV_SPI__

void lora_spiDriverInit(T_LORA_P gpioObj, T_LORA_P spiObj)
{
    hal_spiMap( (T_HAL_P)spiObj );
    hal_gpioMap( (T_HAL_P)gpioObj );

    // ... power ON
    // ... configure CHIP
}

#endif
#ifdef   __LORA_DRV_I2C__

void lora_i2cDriverInit(T_LORA_P gpioObj, T_LORA_P i2cObj, uint8_t slave)
{
    _slaveAddress = slave;
    hal_i2cMap( (T_HAL_P)i2cObj );
    hal_gpioMap( (T_HAL_P)gpioObj );

    // ... power ON
    // ... configure CHIP
}

#endif
#ifdef   __LORA_DRV_UART__

void lora_uartDriverInit(T_LORA_P gpioObj, T_LORA_P uartObj)
{
    hal_uartMap( (T_HAL_P)uartObj );
    hal_gpioMap( (T_HAL_P)gpioObj );

    // ... power ON
    // ... configure CHIP
}

#endif

// GPIO Only Drivers - remove in other cases
void lora_gpioDriverInit(T_LORA_P gpioObj)
{
    hal_gpioMap( (T_HAL_P)gpioObj );

    // ... power ON
}

/* ----------------------------------------------------------- IMPLEMENTATION */
/******************************************************************************
*  LoRa INIT
*******************************************************************************/

void lora_init(bool CB_default, void ( *response_p )( char *response ))
{
    hal_gpio_rstSet( 1 );
    Delay_100ms();
    hal_gpio_rstSet( 0 );
    Delay_100ms();
    Delay_100ms();
    Delay_100ms();
    hal_gpio_rstSet( 1 );
    Delay_100ms();
    hal_gpio_csSet( 1 );
    
    _memset( _tx_buffer, 0, LORA_MAX_CMD_SIZE + LORA_MAX_DATA_SIZE );
    _memset( _rx_buffer, 0, LORA_MAX_RSP_SIZE + LORA_MAX_DATA_SIZE );
    
    _timer_max          = LORA_TIMER_EXPIRED;
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
    
    Delay_1sec();
}
/******************************************************************************
*  LoRa CMD
*******************************************************************************/
void lora_cmd(char *cmd,  char *response)
{
    char buf[] = "";
    uint8_t *pom = buf;
    while( !_lora_rdy_f )
        lora_process();

    _strcpy( _tx_buffer, cmd );

    _rsp_buffer = response;
    _lora_write();

    while( !_lora_rdy_f )
        lora_process();
}
/******************************************************************************
* LoRa MAC TX
*******************************************************************************/
uint8_t lora_mac_tx(char* payload, char* port_no, char *buffer, char *response)
{
    uint8_t res   = 0;

    while( !_lora_rdy_f )
        lora_process();

    _strcpy( _tx_buffer, ( char* )LORA_MAC_TX );
    _strcat( _tx_buffer, payload);
    _strcat( _tx_buffer, port_no );
    _strcat( _tx_buffer, " " );
    _strcat( _tx_buffer, buffer );
    _rsp_buffer = response;
    _lora_write();

    while( !_lora_rdy_f )
        lora_process();

    if( ( res = _lora_par() ) )
        return res;

    _lora_resp();

    do 
    {
        while( !_lora_rdy_f )
            lora_process();

    } while( ( res = _lora_repar() ) == 12 );

    return res;
}
/******************************************************************************
*  LoRa JOIN
*******************************************************************************/
uint8_t lora_join(char* join_mode, char *response)
{
    uint8_t res = 0;

    while( !_lora_rdy_f )
        lora_process();

    _strcpy( _tx_buffer, ( char* )LORA_JOIN );
    _strcat( _tx_buffer, join_mode );
    _rsp_buffer = response;
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
/******************************************************************************
* LORA RX
*******************************************************************************/
uint8_t lora_rx(char* window_size, char *response)
{
    uint8_t res = 0;

    while( !_lora_rdy_f )
        lora_process();

    _strcpy( _tx_buffer, "radio rx " );
    _strcat( _tx_buffer, window_size );
    _rsp_buffer = response;
    _lora_write();

   while( !_lora_rdy_f )
        lora_process();

    if(res = _lora_par())
        return res;

    _lora_resp();

    while( !_lora_rdy_f )
        lora_process();

    return _lora_repar();
}
/******************************************************************************
* LORA TX
*******************************************************************************/
uint8_t lora_tx( char *buffer )
{
    uint8_t res = 0;
    
    lora_process();
    _strcpy( _tx_buffer, "radio tx ");
    _strcat( _tx_buffer, buffer );

    _lora_write();


        lora_process();

    if( ( res = _lora_par() ) )
        return res;

    _lora_resp();
    lora_process();

    return _lora_repar();
}
/******************************************************************************
* LORA RX ISR
*******************************************************************************/
void lora_rx_isr( char rx_input )
{
    static bool _rx_sentence_f;

    _rx_buffer[ _rx_buffer_len++ ] = rx_input;
     if ( rx_input == '\r' )
    {
        _rx_buffer[ _rx_buffer_len++ ] = '\0';
        _rsp_rdy_f = true;  
    } 
}
/******************************************************************************
* LORA TICK ISR
*******************************************************************************/
void lora_tick_isr()
{
    if( _timer_use_f )
        if( _timer_f && ( _ticker++ > _timer_max ) )
            _timeout_f = true;
}
/******************************************************************************
* LoRa TICK CONF
*******************************************************************************/
void lora_tick_conf( uint32_t timer_limit )
{
    if ( timer_limit )
    {
        _timer_max = timer_limit;
        _timer_use_f = true;
    } 
    else 
    {
        _timer_max = LORA_TIMER_EXPIRED;
        _timer_use_f = false;
    }
}
/******************************************************************************
*  LoRa PROCESS
*******************************************************************************/
void lora_process()
{
    if (hal_uartReady())
    {
        char tmp = hal_uartRead();
        lora_rx_isr( tmp );
    }
    if ( _rsp_rdy_f )
    {        
        _lora_read();
    }
    if ( _timeout_f )
    {
        _lora_read();
    }
}
/******************************************************************************
*  LoRa DATA
*******************************************************************************/
/*char lora_rxData()
{
     char _data;
     char rsp_data[30];
     _strncpy( rsp_data, &_rx_buffer[11], 4);
     _xtoi(rsp_data,&_data);

     return _data;
}*/

/* -------------------------------------------------------------------------- */
/*
  __lora_driver.c

  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
   This product includes software developed by the MikroElektonika.

4. Neither the name of the MikroElektonika nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY MIKROELEKTRONIKA ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MIKROELEKTRONIKA BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------------- */