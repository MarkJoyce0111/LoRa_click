/****************************************************************************
* Title                 :   CLICKNAME click
* Filename              :   CLICKNAME.c
* Author                :   MSV
* Origin Date           :   30/01/2016
* Notes                 :   CLICKNAME Example
*****************************************************************************/
/**************************CHANGE LIST **************************************
*
*    Date    Software Version    Initials       Description
*  30/01/16       1.0.0            MSV        Interface Created.
*
*****************************************************************************/
/**
 * @file CLICKNAME.c
 * @brief <h2> CLICKNAME Example </h2>
 *
 * @par
 * Example for <a href="http://www.mikroe.com">MikroElektronika's</a>
 * CLICKNAME click board.
 */

/**
 * @page LIB_INFO Library Info
 * @date        28 Jan 2016
 * @author      Milos Vidojevic
 * @copyright   GNU Public License
 * @version     1.0.0 - Initial testing and verification
 */

/**
 * @page TEST_CFG Test Configurations
 *
 * ### Test configuration STM : ###
 * @par
 * -<b> MCU           </b> :      STM32F107VC
 * -<b> Dev. Board    </b> :      EasyMx Pro v7
 * -<b> Oscillator    </b> :      72 Mhz internal
 * -<b> Ext. Modules  </b> :      CLICKNAME Click
 * -<b> SW            </b> :      MikroC PRO for ARM 4.7.1
 *
 * ### Test configuration PIC32 : ###
 * @par
 * -<b> MCU           </b> :      PIC32MX795F512L
 * -<b> Dev. Board    </b> :      EasyPIC Fusion v7
 * -<b> Oscillator    </b> :      80 Mhz internal
 * -<b> Ext. Modules  </b> :      CLICKNAME Click
 * -<b> SW            </b> :      MikroC PRO for PIC 3.6.0
 *
 * ### Test configuration FT90x : ###
 * @par
 * -<b> MCU           </b> :      FT900Q
 * -<b> Dev. Board    </b> :      EasyFT90x v7
 * -<b> Oscillator    </b> :      100 Mhz internal
 * -<b> Ext. Modules  </b> :      CLICKNAME Click
 * -<b> SW            </b> :      MikroC PRO for FT90x 1.2.1
 *
 * ### Test configuration PIC : ###
 * @par
 * -<b> MCU           </b> :      PIC18F87K22
 * -<b> Dev. Board    </b> :      EasyPIC Pro v7
 * -<b> Oscillator    </b> :      16 Mhz external
 * -<b> Ext. Modules  </b> :      CLICKNAME Click
 * -<b> SW            </b> :      MikroC PRO for PIC 6.6.3
 *
 * ### Test configuration dsPIC : ###
 * @par
 * -<b> MCU           </b> :      dsPIC33EJ256GP710A
 * -<b> Dev. Board    </b> :      EasyPIC Fusion v7
 * -<b> Oscillator    </b> :      8 Mhz internal
 * -<b> Ext. Modules  </b> :      CLICKNAME Click
 * -<b> SW            </b> :      MikroC PRO for dsPIC 6.2.1
 *
 * ### Test configuration AVR : ###
 * @par
 * -<b> MCU           </b> :      ATMEGA32
 * -<b> Dev. Board    </b> :      EasyAVR v7
 * -<b> Oscillator    </b> :      8 Mhz external
 * -<b> Ext. Modules  </b> :      CLICKNAME Click
 * -<b> SW            </b> :      MikroC PRO for FT90x 6.1.1
 */

/**
 * @mainpage
 * <h3> CLICKNAME General Description </h3>
 * @par
 *
 *
 * <h3> CLICKNAME Features </h3>
 * @par
 *
 *
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include <stdint.h>

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
static uint8_t _CLICKNAME_i2c_address;

/******************************************************************************
* Function Prototypes
*******************************************************************************/
static void CLICKNAME_reset( void );

/******************************************************************************
* Private Function Definitions
*******************************************************************************/
/*
 * Reset the CLICKNAME
 *
 * ... explain the function...
 */
static void CLICKNAME_reset( void )
{

}

/******************************************************************************
* Public Function Definitions
*******************************************************************************/
/**
 * @brief <h3> CLICKNAME Initialization </h3>
 *
 * @par
 * Prepares CLICKNAME for usage and sets needed values to default.
 *
 * @note
 * This function must be called first.
 *
 * @param[in]   input   - ...
 * @param[out]  output  - ...
 *
 * @return
 * Function returns ...
 *
 * #### Example : ####
 * @code
 *
 * @endcode
 */
uint8_t CLICKNAME_init( uint16_t input, uint8_t *output )
{

}

void main()
{

}

/*************** END OF FUNCTIONS *********************************************/
