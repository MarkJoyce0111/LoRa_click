/*
    __lora_driver.h

-----------------------------------------------------------------------------

  This file is part of mikroSDK.
  
  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

----------------------------------------------------------------------------- */

/**
@file   __lora_driver.h
@brief    LoRa Driver
@mainpage LoRa Click
@{

@image html libstock_fb_view.jpg

@}

@defgroup   LORA
@brief      LoRa Click Driver
@{

| Global Library Prefix | **LORA** |
|:---------------------:|:-----------------:|
| Version               | **1.0.0**    |
| Date                  | **Feb 2018.**      |
| Developer             | **MikroE Team**     |

*/
/* -------------------------------------------------------------------------- */

#include "stdint.h"
#include "stdbool.h"


#ifndef _LORA_H_
#define _LORA_H_
/** 
 * @macro T_LORA_P
 * @brief Driver Abstract type 
 */
#define T_LORA_P    const uint8_t*

/** @defgroup LORA_COMPILE Compilation Config */              /** @{ */

//  #define   __LORA_DRV_SPI__                            /**<     @macro __LORA_DRV_SPI__  @brief SPI driver selector */
//  #define   __LORA_DRV_I2C__                            /**<     @macro __LORA_DRV_I2C__  @brief I2C driver selector */                                          
  #define   __LORA_DRV_UART__                           /**<     @macro __LORA_DRV_UART__ @brief UART driver selector */ 

                                                                       /** @} */
/** @defgroup LORA_VAR Variables */                           /** @{ */

#define LORA_MAC_TX                   "mac tx "
#define LORA_JOIN                     "mac join "
#define LORA_RADIO_TX                 "radio tx "
#define LORA_RADIO_RX                 "radio rx "

extern char LORA_CMD_SYS_GET_VER[];
extern char LORA_CMD_MAC_PAUSE[];
extern char LORA_CMD_RADIO_SET_WDT[];
extern char LORA_ARG_0[];

                                                                       /** @} */
#ifdef __cplusplus
extern "C"{
#endif

/** @defgroup LORA_INIT Driver Initialization */              /** @{ */

#ifdef   __LORA_DRV_SPI__
void lora_spiDriverInit(T_LORA_P gpioObj, T_LORA_P spiObj);
#endif
#ifdef   __LORA_DRV_I2C__
void lora_i2cDriverInit(T_LORA_P gpioObj, T_LORA_P i2cObj, uint8_t slave);
#endif
#ifdef   __LORA_DRV_UART__
void lora_uartDriverInit(T_LORA_P gpioObj, T_LORA_P uartObj);
#endif

// GPIO Only Drivers - remove in other cases
void lora_gpioDriverInit(T_LORA_P gpioObj);
                                                                       /** @} */
/** @defgroup LORA_FUNC Driver Functions */                   /** @{ */

/**
 * @brief Initialization
 *
 * Must be called before any other operation. Resets the module and sets all
 * flags and parameters to default value.
 *
 * @note Module restart issues the response from the module with current
 * firmware version.
 *
 * @param[in] - pointer to user made callback function that receiving response
 *      as argument and will be executed one every response
 *
 */
void lora_init(bool CB_default, void ( *response_p )( char *response ));
/**
 * @brief Main Process
 *
 * Function must be placed inside the infinite while loop.
 */
void lora_process();
/**
 * @brief Receiver
 *
 * Must be placed inside the user made uart interrupt routine.
 *
 * @param[in] rx_input - data from uart receive register
 */
void lora_rx_isr( char rx_input );
/**
 * @brief Timer
 *
 * Used for host timing. Should be placed inside the previously made interrupt
 * routine made by user that occurs on every one milisecond.
 */
void lora_tick_isr();
/**
 * @brief Timer Configuration
 *
 * Used to configure host watchdog. When timeout occurs response with no data
 * will be parsed. If user provide 0 as argument timer will be turned off. By
 * default after the initialization timer limit is turned on and set to
 * @link TIMER_EXPIRED @endlink
 *
 * @note
 * RN2483 have it's own watchdog timer, this one should be longer than watchdog
 * and can be used to restart the module in case of when library does not receiving
 * responses from the module.
 *
 * @param[in] timer_limit ( 0 ~ 4294967296 )
 */
void lora_tick_conf( uint32_t timer_limit );
/**
 * @brief Sender
 *
 * Used by parser to send data inside the software buffer. User can send valid
 * commands with this function but command string and data should be well
 * formated.
 *
 * @param[in] command - well formated command string
 * @param[in] cmd_len - command string length
 * @param[in] buffer - data buffer if needed
 * @param[in] count - size of data
 */
void lora_cmd(uint8_t *cmd,  char *response);
uint8_t lora_mac_tx(char* payload, char* port_no, char *buffer, char *response);
uint8_t lora_join(char* join_mode, char *response);
uint8_t lora_rx(char* window_size, char *response);
uint8_t lora_tx( char *buffer );
char lora_rxData();






                                                                       /** @} */
#ifdef __cplusplus
} // extern "C"
#endif
#endif

/**
    @example Click_LoRa_STM.c
    @example Click_LoRa_TIVA.c
    @example Click_LoRa_CEC.c
    @example Click_LoRa_KINETIS.c
    @example Click_LoRa_MSP.c
    @example Click_LoRa_PIC.c
    @example Click_LoRa_PIC32.c
    @example Click_LoRa_DSPIC.c
    @example Click_LoRa_AVR.c
    @example Click_LoRa_FT90x.c
    @example Click_LoRa_STM.mbas
    @example Click_LoRa_TIVA.mbas
    @example Click_LoRa_CEC.mbas
    @example Click_LoRa_KINETIS.mbas
    @example Click_LoRa_MSP.mbas
    @example Click_LoRa_PIC.mbas
    @example Click_LoRa_PIC32.mbas
    @example Click_LoRa_DSPIC.mbas
    @example Click_LoRa_AVR.mbas
    @example Click_LoRa_FT90x.mbas
    @example Click_LoRa_STM.mpas
    @example Click_LoRa_TIVA.mpas
    @example Click_LoRa_CEC.mpas
    @example Click_LoRa_KINETIS.mpas
    @example Click_LoRa_MSP.mpas
    @example Click_LoRa_PIC.mpas
    @example Click_LoRa_PIC32.mpas
    @example Click_LoRa_DSPIC.mpas
    @example Click_LoRa_AVR.mpas
    @example Click_LoRa_FT90x.mpas
*/                                                                     /** @} */
/* -------------------------------------------------------------------------- */
/*
  __lora_driver.h

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