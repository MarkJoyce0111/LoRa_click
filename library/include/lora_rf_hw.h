/****************************************************************************
* Title                 :   LORA RF CLICK ENGINE
* Filename              :   lora_rf_engine.h
* Author                :   MSV
* Origin Date           :   24/11/2015
* Notes                 :   None
*****************************************************************************/
/**************************CHANGE LIST **************************************
*
*    Date    Software Version    Initials   Description
*  24/11/15         2.0            MSV       Interface Created.
*
*****************************************************************************/
/**
 * @file lora_rf_hw.h
 *
 * @brief HW library of the module.
 *
 * Hardware and communication control.
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
/**
 * @mainpage
 * @section General Description
 * @par
 * LoRa RF click carries Microchip's RN2483 fully certified LoRa Sub-GHz,
 * 433/868 MHz European R&TTE Directive Assessed Radio Modem.
 * Two antenna connectors allow you to choose which of the two frequency bands
 * will be employed.
 * @par
 * This module handles the LoRaWAN Class A protocol and provides an
 * optimized text command/response interface to the host system. The required
 * configuration for accessing a LoRa technology network is minimal and can
 * be stored in the module’s EEPROM, allowing for factory configuration of these
 * parameters, lowering the requirements for the host system while also
 * increasing system security.
 * @par
 * LoRa RF click communicates with the target board MCU through the mikroBUS
 * UART interface (CTS, TXD, RXD), with the addition of a Reset pin (RST).
 * The board is designed to use either a 3.3V or a 5V power supply.
 *
 * @section Features
 * <ul>
 * <li>LoRaWAN Class A protocol compliance</li>
 * <li>Integrated FSK, GFSK and LoRa technology transceiver allowing the
 * user to transmit custom packets using these protocols</li>
 * <li>Globally unique 64-bit identifier (EUI-64)</li>
 * <li>Intelligent Low-Power mode with programmable/on-demand wake-up</li>
 * <li>Bootloader for firmware upgrade</li>
 * <li>All configuration and control done over UART using simple ASCII
 * commands</li>
 * </ul>
 *
 * @page conf Configuration
 * @par
 * In order to communicate with a LoRa network, a specific number of
 * parameters need to be configured. Since two distinctive methods are offered
 * for a device to become part of the network, each of these requires different
 * parameters:
 * <ul>
 * <li>Over-the-Air Activation (OTAA), where a device negotiates network
 * encryption keys at the time it joins the network. For this, the device EUI,
 * application EUI and application key need to be configured and then the OTAA
 * procedure can start.</li>
 * <li>Activation by Personalization (ABP) where the device already contains
 * the network keys and can directly start communication with the network.
 * Configuring the device address, network session key and application session
 * key is sufficient for this type of initialization.</li>
 * </ul>
 * @par
 * All of the RN2483 module’s settings and commands are transmitted over UART
 * using the ASCII interface. he default settings for the UART interface are
 * 57600 bps, 8 bits, no parity, 1 Stop bit, no flow control.
 */
/**
 * @defgroup CMD_BY_LVL Command Levels
 * @brief Module Command Levels
 *
 * @par
 * There are three types of commands that can be used, and each allows access
 * to different module functions :
 * <ul>
 * <li>
 * MAC - Class A configuration and control
 * </li>
 * <li>
 * Radio - Radio configuration and control
 * </li>
 * <li>
 * Sys - Other module functions
 * </li>
 *
 * @defgroup MAC_CMD_LVL MAC
 * @ingroup CMD_BY_LVL
 * @brief Mac commands for LoRa RF click
 *
 * @defgroup SYS_CMD_LVL System
 * @ingroup CMD_BY_LVL
 * @brief System Commands for LoRa RF click.
 *
 * @defgroup RADIO_CMD_LVL Radio
 * @ingroup CMD_BY_LVL
 * @brief Radio commands for LoRa RF click
 */
#ifndef LORA_RF_ENGINE_H_
#define LORA_RF_ENGINE_H_
/******************************************************************************
* Includes
*******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
/**
 * Timer Limit ( ms ) */
#define TIMER_EXPIRED                                       20000
/**
 * Number of Response Indexes */
#define RESP_COUNT                                          19
/**
 * Response String Max Size */
#define MAX_RSP_SIZE                                        20
/**
 * Command String Max Size */
#define MAX_CMD_SIZE                                        64
/**
 * Data String Max Size */
#define MAX_DATA_SIZE                                       256
/******************************************************************************
* Configuration Constants
*******************************************************************************/

/******************************************************************************
* Macros
*******************************************************************************/

/******************************************************************************
* Typedefs
*******************************************************************************/
/**
 * @enum CMD_LVL_t
 * @brief Command level
 *
 * Used by command parser.
 * @link lora_rf_parse_command @endlink
 */
typedef enum
{
    CMD_LVL_NLVL,
    /**
     * System Commands for LoRa RF click.
     * @link SYS_CMD_LVL @endlink */
    CMD_LVL_SYS = 1,
    /**
     * Mac commands for LoRa RF click.
     * @link MAC_CMD_LVL @endlink */
    CMD_LVL_MAC = 2,
    /**
     * Radio commands for LoRa RF click
     * @link RADIO_CMD_LVL @endlink */
    CMD_LVL_RADIO = 3

}CMD_LVL_t;

/**
 * @enum CMD_CTL_t
 * @brief Control Commands
 *
 * Used by command parser.
 * @link lora_rf_parse_command @endlink
 */
typedef enum
{
    CMD_CTL_NCTL,
    /**
     * System level command - for more info see
     * @link lora_sleep @endlink */
    CMD_CTL_SLEEP = 1,
    /**
     * System and MAC level command - for more info see
     * @link lora_reset @endlink
     * or
     * @link lora_mac_reset @endlink */
    CMD_CTL_RESET = 2,
    /**
     * System level command - for more info see
     * @link lora_erase_fw @endlink */
    CMD_CTL_ERASE_FW = 3,
    /**
     * System level command - for more info see
     * @link lora_fct_rst @endlink */
    CMD_CTL_FACT_RST = 4,
    /**
     * MAC level command - for more info see
     * @link lora_join @endlink */
    CMD_CTL_JOIN = 5,
    /**
     * MAC level command - for more info see
     * @link lora_save @endlink */
    CMD_CTL_SAVE = 6,
    /**
     * MAC level command - for more info see
     * @link lora_force_en @endlink */
    CMD_CTL_FORCE_EN = 7,
    /**
     * MAC level command - for more info see
     * @link lora_pause @endlink */
    CMD_CTL_PAUSE = 8,
    /**
     * MAC level command - for more info see
     * @link lora_resume @endlink */
    CMD_CTL_RESUME = 9,
    /**
     * Radio level command - for more info see
     * @link lora_rx @endlink */
    CMD_CTL_RX = 10,
    /**
     * MAC and Radio level command - for more info see
     * @link lora_mac_tx @endlink
     * and
     * @link lora_tx @endlink */
    CMD_CTL_TX = 11,
    /**
     * Radio level command - for more info see
     * @link lora_cw @endlink */
    CMD_CTL_CW = 12,
    /**
     * Sys, MAC and Radio level command for setting or getting various
     * parameters. */
    CMD_CTL_GET = 13,
    /**
     * Sys, MAC and Radio level command for setting or getting various
     * parameters. */
    CMD_CTL_SET = 14

}CMD_CTL_t;

/**
 * @enum CMD_OPT_t
 * @brief Optional set and get commands for all three command levels
 *
 * Used by command parser.
 * @link lora_rf_parse_command @endlink
 */
typedef enum
{
    CMD_OPT_NO_OPT,
    /**
     * System level, get and set available command. */
    CMD_OPT_PINDIG,
    /**
     * System level, get and set available command - for more info see
     * @link lora_eeprom @endlink */
    CMD_OPT_NVM,
    /**
     * System level, get available command - for more info see
     * @link lora_fw_ver @endlink */
    CMD_OPT_VER,
    /**
     * System level, get available command - for more info see
     * @link lora_vdd @endlink */
    CMD_OPT_VDD,
    /**
     * System level, get available command - for more info see
     * @link lora_hw_eui @endlink */
    CMD_OPT_HWEUI,
    /**
     * MAC level, set available command - for more info see
     * @link lora_ns_key @endlink */
    CMD_OPT_NWKSKEY,
    /**
     * MAC level, set available command - for more info see
     * @link lora_as_key @endlink */
    CMD_OPT_APPSKEY,
    /**
     * MAC level, set available command - for more info see
     * @link lora_app_key @endlink */
    CMD_OPT_APPKEY,
    /**
     * MAC level, set available command - for more info see
     * @link lora_battery @endlink */
    CMD_OPT_BAT,
    /**
     * MAC level, set available command - for more info see
     * @link lora_lnk_chk @endlink */
    CMD_OPT_LINKCHK,
    /**
     * MAC level, get and set available command - for more info see
     * @link lora_mac_ch_freq @endlink */
    CMD_OPT_CH_FREQ,
    /**
     * MAC level, get and set available command - for more info see
     * @link lora_mac_ch_d_cycle @endlink */
    CMD_OPT_CH_DCYCLE,
    /**
     * MAC level, get and set available command - for more info see
     * @link lora_mac_ch_dr_range @endlink */
    CMD_OPT_CH_DRRANGE,
    /**
     * MAC level, get and set available command - for more info see
     * @link lora_mac_ch_status @endlink */
    CMD_OPT_CH_STATUS,
    /**
     * MAC level, get and set available command - for more info see
     * @link lora_dev_addr @endlink */
    CMD_OPT_DEV_ADDR,
    /**
     * MAC level, get and set available command - for more info see
     * @link lora_dev_eui @endlink */
    CMD_OPT_DEV_EUI,
    /**
     * MAC level, get and set available command - for more info see
     * @link lora_app_eui @endlink */
    CMD_OPT_APP_EUI,
    /**
     * MAC level, get and set available command - for more info see
     * @link lora_dr @endlink */
    CMD_OPT_DR,
    /**
     * MAC level, get and set available command - for more info see
     * @link lora_pwridx @endlink */
    CMD_OPT_PWRIDX,
    /**
     * MAC level, get and set available command - for more info see
     * @link lora_adr @endlink */
    CMD_OPT_ADR,
    /**
     * MAC level, get and set available command - for more info see
     * @link lora_retr @endlink */
    CMD_OPT_RETX,
    /**
     * MAC level, get and set available command - for more info see
     * @link lora_ar @endlink */
    CMD_OPT_AR,
    /**
     * MAC level, get and set available command - for more info see
     * @link lora_rx2 @endlink and
     * @link lora_get_rx2 @endlink
     */
    CMD_OPT_RX_2,
    /**
     * MAC level, get available command - for more info see
     * @link lora_band @endlink */
    CMD_OPT_BAND,
    /**
     * MAC level, get and set available command - for more info see
     * @link lora_rx_1_dly @endlink */
    CMD_OPT_RXDELAY1,
    /**
     * MAC level, get available command - for more info see
     * @link lora_rx_2_dly @endlink */
    CMD_OPT_RXDELAY2,
    /**
     * MAC level, get available command - for more info see
     * @link lora_dcycle_ps @endlink */
    CMD_OPT_DCYCLEPS,
    /**
     * MAC level, get available command - for more info see
     * @link lora_dem_marg @endlink */
    CMD_OPT_MRGN,
    CMD_OPT_GWBN,
    /**
     * MAC level, get available command - for more info see
     * @link lora_status @endlink */
    CMD_OPT_STAT,
    /**
     * MAC level, get and set available command - for more info see
     * mac_channel functions */
    CMD_OPT_CH,
    /**
     * Radio level, set available command - for more info see
     * @link lora_syncw @endlink */
    CMD_OPT_SYNC,
    /**
     * Radio level, get and set available command - for more info see
     * @link lora_bt @endlink */
    CMD_OPT_BT,
    /**
     * Radio level, get and set available command - for more info see
     * @link lora_mod @endlink */
    CMD_OPT_MOD,
    /**
     * Radio level, get and set available command - for more info see
     * @link lora_freq @endlink */
    CMD_OPT_FREQ,
    /**
     * Radio level, get and set available command - for more info see
     * @link lora_power @endlink */
    CMD_OPT_PWR,
    /**
     * Radio level, get and set available command - for more info see
     * @link lora_sf @endlink */
    CMD_OPT_SF,
    /**
     * Radio level, get and set available command - for more info see
     * @link lora_freq_corr @endlink */
    CMD_OPT_AFCBW,
    /**
     * Radio level, get and set available command - for more info see
     * @link lora_rx_bw @endlink */
    CMD_OPT_RXBW,
    /**
     * Radio level, get and set available command - for more info see
     * @link lora_fsk_br @endlink */
    CMD_OPT_BITRATE,
    /**
     * Radio level, get and set available command - for more info see
     * @link lora_freq_dev @endlink */
    CMD_OPT_FDEV,
    /**
     * Radio level, get and set available command - for more info see
     * @link lora_pr_len @endlink */
    CMD_OPT_PRLEN,
    /**
     * Radio level, get and set available command - for more info see
     * @link lora_crc @endlink */
    CMD_OPT_CRC,
    /**
     * Radio level, get and set available command - for more info see
     * @link lora_iq_inv @endlink */
    CMD_OPT_IQI,
    /**
     * Radio level, get and set available command - for more info see
     * @link lora_cr @endlink */
    CMD_OPT_C_RATE,
    /**
     * Radio level, get and set available command - for more info see
     * @link lora_wdog @endlink */
    CMD_OPT_WDT,
    /**
     * Radio level, get and set available command - for more info see
     * @link lora_bw @endlink */
    CMD_OPT_BW,
    /**
     * Radio level, get available command - for more info see
     * @link lora_snr @endlink */
    CMD_OPT_SNR

}CMD_OPT_t;
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
void lora_rf_hw_init( void );

/**
 * @brief Callback function
 *
 * @warning User should avoid usage of this function if functionality
 * of @link lora_rf.h @endlink is needed. Usage of this function after the
 * library initialization is not
 *
 * Use
 * @link lora_rf_callback @endlink
 * to monitor module responses.
 */
void lora_rf_set_callback( void ( *response_p )( char *response ) );

/**
 * @brief Main Process
 *
 * Function must be placed inside the infinite while loop.
 */
void lora_rf_process( void );

/**
 * @brief Receiver
 *
 * Must be placed inside the user made uart interrupt routine.
 *
 * @param[in] rx_input - data from uart receive register
 */
void lora_rf_rx_isr( char rx_input );

/**
 * @brief Timer
 *
 * Used for host timing. Should be placed inside the previously made interrupt
 * routine made by user that occurs on every one milisecond.
 */
void lora_rf_tick_isr( void );

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
void lora_rf_timer_conf( uint32_t timer_limit );

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
void lora_rf_cmd_send( char *command,
                       char *buffer );

/**
 * @brief Command Parser
 *
 * Used by
 * @link lora_rf.h @endlink
 * functions to parse commands and also can be used by user to send well
 * formated commands.
 *
 * @param[in] level - command level
 * @param[in] control - control command
 * @param[in] opt - option command
 * @param[in] args - pointer to data
 */
void lora_rf_cmd_parse( CMD_LVL_t level,
                        CMD_CTL_t control,
                        CMD_OPT_t opt,
                        char *args );

/**
 * @brief Response Flag
 *
 * Used by
 * @link lora_rf.h @endlink
 * functions to avoid executing of the next command before response.
 * User also can use this function to avoid sending multiple commands to
 * module.
 *
 * @retval ( true - response received / false - response still not received )
 */
bool lora_rf_rsp_rdy( void );

/**
 * @brief lora_rf_reset_response_rdy
 *
 * Used by
 * @link lora_rf.h @endlink
 * functions to reset value of response ready flag. This is needed for
 * command that can response with more that one response so user can
 * set response ready flag to false and block sending of next command
 * before receiveng the whole response from the module.
 *
 * @param[in] value
 */
void lora_rf_set_rsp_rdy( bool value );

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LORA_RF_ENGINE_H

/*** End of File **************************************************************/
