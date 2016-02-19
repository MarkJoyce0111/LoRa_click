/****************************************************************************
* Title                 :   LORA RF CLICK HAL
* Filename              :   lora_rf_hal.h
* Author                :   MSV
* Origin Date           :   02/12/2015
* Notes                 :   None
*****************************************************************************/
/**************************CHANGE LIST **************************************
*
*    Date    Software Version    Initials   Description
*  02/12/15    XXXXXXXXXXX         MSV      Interface Created.
*
*****************************************************************************/
/**
 * @file lora_rf_hal.h
 *
 * @brief Hal layer for the Mikroelektronika's RF Lora click board.
 *
 * @date 10 FEB 2016
 * @author Milos Vidojevic
 * @copyright GNU Public License
 *
 * @version 2.0.0
 *
 * @note Test configuration:
 *  MCU:             PIC32MX795F512L
 *  Dev.Board:       EasyPIC Fusion v7
 *  Oscillator:      80 Mhz
 *  Ext. Modules:    LoRa RF Click
 *  SW:              PIC32 3.6.0
 */
#ifndef LORA_RF_HAL_H_
#define LORA_RF_HAL_H_
/******************************************************************************
* Includes
*******************************************************************************/
#include <stdint.h>
/******************************************************************************
* Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Configuration Constants
*******************************************************************************/

/******************************************************************************
* Macros
*******************************************************************************/

/******************************************************************************
* Typedefs
*******************************************************************************/

/******************************************************************************
* Variables
*******************************************************************************/

/******************************************************************************
* Function Prototypes
*******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

/**
 * @brief Hal layer initialization.
 *
 * Used by HW layer initialization.
 */
void lora_rf_hal_init( void );

/**
 * @brief RST High
 *
 * Sets RST pin to logic high.
 */
void lora_rf_hal_rst_high( void );

/**
 * @brief RST Low
 *
 * Sets RST pin to logic low.
 */
void lora_rf_hal_rst_low( void );

/**
 * @brief Writer
 *
 * Writes data through uart interface.
 *
 * @param[in] data_in
 */
void lora_rf_hal_write_byte( char *data_in );

#ifdef __cplusplus
} // extern "C"
#endif

#endif /*LORA_RF_HAL_H_*/

/*** End of File **************************************************************/
