/****************************************************************************
* Title                 :   LORA RF
* Filename              :   lora_rf_rf.h
* Author                :   MSV
* Origin Date           :   02/12/2015
* Notes                 :   None
*****************************************************************************/
/**************************CHANGE LIST **************************************
*
*    Date    Software Version    Initials   Description
*  02/12/15    XXXXXXXXXXX         MSV      Interface Created
*
*****************************************************************************/
/**
 * @file lora_rf.h
 *
 * @brief Functions for Mikroelektronika's LoRa RF click board.
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
#ifndef LORA_RF_H_
#define LORA_RF_H_
/******************************************************************************
* Includes
*******************************************************************************/
#include <stdlib.h>
#include <stdint.h>
#include "lora_rf_hw.h"
/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
/**
 * Sleep time minimum*/
#define SLEEP_TIME_MIN          100
/**
 * Sleep time maximum*/
#define SLEEP_TIME_MAX          4294967296
/**
 * Lowest EEPROM address*/
#define EEPROM_ADDRESS_MIN      0x300
/**
 * Highest EEPROM address*/
#define EEPROM_ADDRESS_MAX      0x3FF
/**
 * The Lowest MAC Transmit Port */
#define PORT_NO_MIN             1
/**
 * The Highest MAC Transmit Port */
#define PORT_NO_MAX             233
/**
 * Power Index Limit*/
#define POWER_INDEX_MAX         5
/**
 * Data Rate Limit */
#define DATA_RATE_MAX           7
/**
 * Battery Level Limit*/
#define BATTERY_LEVEL_MAX       255
/**
 * Retransmission Limit*/
#define RETRANSMISSION_MAX      255
/**
 * Link Check Limit*/
#define LINK_CHECK_MAX          65535
/**
 * RX Window 1 Delay Limit*/
#define RX_1_DELAY_MAX          65535
/**
 * Duty Cycle Limit*/
#define DUTY_CYCLE_MAX          65535
/**
 * Data Rate Range Limit*/
#define DATA_RANGE_MAX          7
/**
 * The Lowest RX 2 Windiow Frequency at BAND_868 */
#define RX_2_868_MIN            863000000
/**
 * The Highest RX 2 Windiow Frequency at BAND_868 */
#define RX_2_868_MAX            870000000
/**
 * The Lowest RX 2 Windiow Frequency at BAND_433 */
#define RX_2_433_MIN            433050000
/**
 * The Highest RX 2 Windiow Frequency at BAND_433*/
#define RX_2_433_MAX            434790000
/**
 * Lowest channel ID */
#define CHANNEL_ID_MIN          3
/**
 * Highest channel ID */
#define CHANNEL_ID_MAX          15
/**
 * The Lowest Channel Frequency at BAND_868 */
#define CHANNEL_868_MIN         863000000
/**
 * The Highest Channel Frequency at BAND_868 */
#define CHANNEL_868_MAX         870000000
/**
 * The Lowest Channel Frequency at BAND_868 */
#define CHANNEL_433_MIN         433050000
/**
 * The Highest Channel Frequency at BAND_868 */
#define CHANNEL_433_MAX         434790000
/**
 * The Lowest Power Index */
#define POWER_OUT_MIN           -3
/**
 * The Highest Power Index */
#define POWER_OUT_MAX           15
/**
 * FSK Bitrate Limit */
#define FSK_BIT_RATE_MAX        65535
/**
 * Frequency Deviation Limit */
#define FREQ_DEVIATION_MAX      65535
/**
 * Preable Length Limit */
#define PREAMBLE_LENGTH_MAX     65535
/**
 * The Lowest Radio Frequency at BAND_433 */
#define RADIO_433_MIN           433000000
/**
 * The Highest Radio Frequency at BAND_433 */
#define RADIO_433_MAX           434800000
/**
 * The Lowest Radio Frequency at BAND_838 */
#define RADIO_868_MIN           863000000
/**
 * The Highest Radio Frequency at BAND_868 */
#define RADIO_868_MAX           870000000
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
 * @enum BND_t
 * @brief Band Type
 *
 * Used by funuctions
 *
 * @link lora_rf_mac_reset @endlink and
 * @link lora_rf_get_rx2 @endlink
 */
typedef enum
{
    /**
     * 433 MHz band */
    BND_433,
    /**
     * 868 MHz band */
    BND_868

}BND_t;

/**
 * @enum PL_t
 * @brief Payload Type
 *
 * Used by funuction
 *
 * @link lora_rf_mac_tx @endlink
 */
typedef enum
{
    /**
     * Confirmed payload */
    PL_CNF,
    /**
     * Uncofirmed payload */
    PL_UNCNF

}PL_t;

/**
 * @enum JM_t
 * @brief Join Procedure Type
 *
 * @link lora_rf_join @endlink
 */
typedef enum
{
    /**
     * Over the air activation */
    JM_OTAA,
    /**
     * Activation by personalization */
    JM_ABP

}JM_t;

/**
 * @enum MOD_t
 * @brief Modulation Type
 */
typedef enum
{
    /**
     * LoRa modulation */
    MOD_LORA,
    /**
     * FSK modulation */
    MOD_FSK

}MOD_t;

/**
 * @enum STATUS_t
 * @brief Status Type
 */
typedef enum
{
    /**
     * ON status */
    STATUS_ON,
    /**
     * OFF status */
    STATUS_OFF

} STATUS_t;
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
 * @brief Library Initialization
 *
 * Must be called before other functions.
 */
void lora_rf_init( void );

/**
 * @brief User Callback
 *
 * User can provide his own function to monitor all module responses. Disabling
 * of callback can be done by providing NULL pointer. All data is returned as
 * string so it can be very usefull for log.
 *
 * @param[in] rsp_p - callback function
 */
void lora_rf_callback( void ( *rsp_p )( char *rsp ) );

/**
 * @ingroup SYS_CMD_LVL
 * @brief Sleep Mode
 *
 * Puts the system in Sleep for a number of milliseconds.
 * The module can be forced to exit from Sleep by sending a break condition
 * followed by a 0x55 character at the new baud rate. Note that the break
 * condition needs to be long enough not to be interpreted as a valid
 * character at the current baud rate.
 *
 * @param[in] sleep_time ( 100 ~ 4294967296 )
 * @retval ( true / false )
 *
 * @code
 * int x;
 *
 * x = lora_rf_sleep( 10000 );
 * @endcode
 */
int lora_rf_sleep( uint32_t sleep_time );

/**
 * @ingroup SYS_CMD_LVL
 * @brief Module Reset
 *
 * Resets and restarts the module. Stored internal configurations will be
 * loaded automatically upon reboot.
 */
int lora_rf_reset( void );

/**
 * @ingroup SYS_CMD_LVL
 * @brief Factory Reset
 *
 * Resets the module’s configuration data and user EEPPROM to
 * factory default values and restarts the module.
 * After factory reset, the module will automatically reset and all
 * configuration parameters are restored to factory default values.
 */
int lora_rf_f_reset( void );

/**
 * @ingroup SYS_CMD_LVL
 * @brief Read EEPROM Memory
 *
 * Get data from user EEPROM.
 *
 * @param[in] address ( 0x300 ~ 0x3FF )
 * @param[out] buffer ( "00" ~ "FF" )
 */
int lora_rf_eeprom( uint16_t address,
                    char *buffer );

/**
 * @ingroup SYS_CMD_LVL
 * @brief Write EEPROM Memory
 *
 * User provided <buffer> will be stored on EEPROM <address>.
 * Both <address> and <buffer> must be entered as hex values.
 *
 * @param[in] address ( 0x300 ~ 0x3FF )
 * @param[in] buffer ( "00" ~ "FF" )
 */
int lora_rf_set_eeprom( uint16_t address,
                        char *buffer );
/**
 * @ingroup SYS_CMD_LVL
 * @brief Get Firmware Version
 *
 * Returns the information on hardware platform, firmware version,
 * release date.
 *
 * @param[out] buffer
 */
int lora_rf_fw_ver( char *buffer );

/**
 * @ingroup SYS_LVL_CMD
 * @brief Get Device Voltage
 *
 * Informs the module to do an ADC conversion on the Vdd.
 * The measurement is converted and returned as a voltage (mV).
 *
 * @param[out] buffer
 */
int lora_rf_vdd( char *buffer );

/**
 * @ingroup SYS_LVL_CMD
 * @brief Get Hadrware EUI
 *
 * Reads the preprogrammed EUI node address from the module.
 * The value returned by this command is a globally unique number.
 */
int lora_rf_hw_eui( char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief MAC Reset
 *
 * Resets the RN2483 module to a specific frequency band.
 * This command will automatically reset the software LoRaWAN stack and
 * initialize it with the parameters for the selected band.
 *
 * @note
 * This command will set default values for most of the LoRaWAN parameters.
 * Everything set prior to this command will lose its set value.
 *
 * @param[in] band - ( BND_433 / BND_868 )
 */
int lora_rf_mac_reset( BND_t band );

/**
 * @ingroup MAC_CMD_LVL
 * @brief MAC Transmit
 *
 * Sends the data string on a specified port number and sets default
 * values for most of the LoRaWAN parameters.
 * This command may reply with two responses. The first response will be
 * received immediately after entering the command. Second reply will be
 * received after the end of the uplink transmission.
 *
 * A confirmed message will expect an acknowledgment from the server; otherwise,
 * the message will be retransmitted by the number indicated by the command mac
 * set retx, where as an unconfirmed message will not expect any
 * acknowledgment back from the server.If the automatic reply feature is
 * enabled and the server sets the Frame Pending bit or initiates downlink
 * confirmed transmissions, multiple responses will be displayed after each
 * downlink packet is received by the module.
 *
 * If the automatic reply feature is enabled and the server sets the Frame
 * Pending bit or initiates downlink confirmed transmissions, multiple
 * responses will be displayed after each downlink packet is received by the
 * module.
 *
 * @note
 * Buffer parameter is used to provide data for transmission and if there is
 * response data - buffer will be used as respose storage.
 *
 * @param[in] type - ( PL_CNF / PL_UNCNF )
 * @param[in] port_no - ( 1 ~ 233 )
 * @param[in] buffer
 * @param[out] buffer
 */
int lora_rf_mac_tx( PL_t type,
                    uint8_t port_no,
                    char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Join Network
 *
 * Informs the RN2483 module to join the configured network.
 * Module activation type is selected with <mode>. Before joining the network,
 * the specific parameters for each activation type should be configured
 * (for over the air activation: device EUI, application EUI, application key;
 * for activation by personalization: device address, network session key,
 * application session key.
 *
 * This command may reply with two responses. The first response will be
 * received immediately after entering the command. In case the command is valid.
 * Second reply will be received after the end of the join procedure.
 *
 * @param[in] mode - ( JM_OTAA / JM_ABP )
 */
int lora_rf_join( JM_t mode );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Save Configuration
 *
 * Saves LoRaWAN Class A configuration parameters to the user EEPROM.
 * The mac save command must be issued after configuration parameters have been
 * appropriately entered from the mac set commands. This command will save
 * LoRaWAN Class A protocol configuration parameters to the user EEPROM.
 * When the next sys reset command is issued, the LoRaWAN Class A protocol
 * configuration will be initialized with the last saved parameters.
 *
 * @note
 * Savable parameters are:
 * ( band / deveui / appeui / appkey / nwkskey / appskey / devaddr /
 *   ch freq / ch dcycle / ch drrange / ch status )
 */
int lora_rf_save( void );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Force Enable
 *
 * Enables the RN2483 module after the LoRaWAN network server commanded
 * the end device to become silent immediately.
 * The network can issue a certain command (Duty Cycle Request frame with
 * parameter 255) that would require the RN2483 module to go silent immediately.
 * This mechanism disables any further communication of the module, effectively
 * isolating it from the network. Using mac forceENABLE, after this network
 * command has been received, restores the module’s connectivity by allowing it
 * to send data.
 */
int lora_rf_force_en( void );

/**
 * @ingroup MAC_CMD_LVL
 * @brief MAC Pause
 *
 * Pauses LoRaWAN stack functionality to allow transceiver (radio)
 * configuration.
 * Through the use of mac pause, radio commands can be generated between a
 * LoRaWAN Class A protocol uplink application (mac tx command), and the LoRaWAN
 * Class A protocol Receive windows (second response for the mac tx command).
 * This command will reply with the time interval in milliseconds that the
 * transceiver can be used without affecting the LoRaWAN functionality.
 * The maximum value is returned whenever the LoRaWAN stack functionality is
 * in Idle state and the transceiver can be used without restrictions.
 * ‘0’ is returned when the LoRaWAN stack functionality cannot be paused.
 * After the radio configuration is complete, the mac resume command should be
 * used to return to LoRaWAN Class A protocol commands.
 *
 * @note
 * If already joined to a network, this command must be called before
 * configuring the radio parameters, initiating radio reception,
 * or transmission.
 *
 * @param[out] buffer - time ms
 */
int lora_rf_pause( char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief MAC Resume
 *
 * Restores the LoRaWAN stack functionality
 *
 * @note
 * This command must be called after all radio commands have been issued and
 * all the corresponding asynchronous messages have been replied.
 */
int lora_rf_resume( void );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Set Device Address
 *
 * Sets the unique network device address for RN2483 module.
 * The <buffer> must be unique address to the current network.
 * This must be directly set solely for activation by personalization devices.
 * This parameter must not be set before attempting to join using over-the-air
 * activation because it will be overwritten once the join process is over.
 *
 * @note
 * If this parameter had previously been saved to user EEPROM by issuing
 * the mac save command, after modifying its value, the mac save
 * command should be called again.
 *
 * @param[in] buffer - ( "00000000" ~ "FFFFFFFF" )
 */
int lora_rf_set_dev_addr( char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Get Device Address
 *
 * For more info
 * @link lora_rf_set_dev_addr @endlink
 *
 * @param[out] buffer
 */
int lora_rf_dev_addr( char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Set Device EUI
 *
 * Sets the globally unique identifier for the RN2483 module.
 * The identifier must be set by the host MCU.
 *
 * @note
 * If this parameter was previously saved to user EEPROM by issuing the mac
 * save command, after modifying its value, the mac save command should be
 * called again.
 *
 * @param[in] buffer - ( "0000000000000000" ~ "FFFFFFFFFFFFFFFF" )
 */
int lora_rf_set_dev_eui( char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Get Device EUI
 *
 * For more info
 * @link lora_rf_set_dev_eui @endlink
 *
 * @param[out] buffer
 */
int lora_rf_dev_eui( char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Set Application EUI
 *
 * Sets the application identifier for the RN2483 module.
 * The application identifier should be used to identify device types
 * (sensor device, lighting device, etc.) within the network.
 *
 * @note
 * If this parameter was previously saved to user EEPROM by issuing the mac
 * save command, after modifying its value, the mac save command should be
 * called again.
 *
 * @param[in] buffer - ( "0000000000000000" ~ "FFFFFFFFFFFFFFFF" )
 */
int lora_rf_set_app_eui( char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Get Application EUI
 *
 * For more info
 * @link lora_rf_set_app_eui @endlink
 *
 * @param[out] buffer
 */
int lora_rf_app_eui( char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Set Power Index
 *
 * Sets the output power to be used on the next transmissions.
 *
 * @param[in] buffer - ( "0" ~ "5" )
 */
int lora_rf_set_pwridx( uint8_t pwr );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Get Power Index
 *
 * For more info
 * @link lora_rf_set_pwridx @endlink
 *
 * @param[out] buffer
 */
int lora_rf_pwridx( char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Set Data Rate
 *
 * Sets the data rate to be used for the next transmissions.
 *
 * @param[in] buffer - ( 0 ~ 7 )
 */
int lora_rf_set_dr( uint8_t dr );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Get Data Rate
 *
 * For more info
 * @link lora_rf_set_dr @endlink
 * @param[out] buffer
 */
int lora_rf_dr( char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Set Adaptive Data Rate
 *
 * Sets the state of adaptive data rate for the device.
 * The server is informed about the status of the module’s ADR in
 * every uplink frame it receives from the ADR field in uplink data packet.
 * If ADR is enabled, the server will optimize the data rate and the
 * transmission power of the module based on the information collected
 * from the network.
 *
 * @param[in] status ( STATUS_ON / STATUS_OFF )
 */
int lora_rf_set_adr( STATUS_t status );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Get Adaptive Data Rate
 *
 * For more info
 * @link lora_rf_set_adr @endlink
 *
 * @param[out] buffer
 */
int lora_rf_adr( char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Set Retransmission
 *
 * Sets the number of retransmissions to be used for an uplink
 * confirmed packet, if no downlink acknowledgment is received from the server.
 *
 * @param[in] buffer - ( "0" ~ "255" )
 */
int lora_rf_set_retr( uint8_t retr );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Get Retransmission
 *
 * For more info
 * @link lora_rf_set_retr @endlink
 *
 * @param[out] buffer
 */
int lora_rf_retr( char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Set RX 1 Delay
 *
 * Sets the value used for the first receive window delay.
 *
 * Set the delay between the transmission and the first reception window to the
 * <rxdelay> in milliseconds. The delay between the transmission and the second
 * reception window is calculated in software as the delay between the
 * transmission and the first reception window + 1000 (ms).
 *
 * @param[in] buffer - ( 0 ~ 65535 )
 */
int lora_rf_set_rx_1_dly( uint16_t dly );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Get RX 1 Delay
 *
 * For more info
 * @link lora_rf_set_rx_1_dly @endlink
 *
 * @param[out] buffer
 */
int lora_rf_rx_1_dly( char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Set Automatic Replay
 *
 * Sets or gets the state of the automatic reply.
 *
 * By enabling the automatic reply, the module will transmit a packet without a
 * payload immediately after a confirmed downlink is received, or when the
 * Frame Pending bit has been set by the server. If set to OFF, no automatic
 * reply will be transmitted.
 *
 * @note
 * The RN2483 module implementation will initiate automatic transmissions
 * with no application payload if the automatic reply feature is enabled and the
 * server sets the Frame Pending bit or initiates a confirmed downlink
 * transmission. In this case, if all enabled channels are busy due to duty cycle
 * limitations, the stack will wait for the first channel that will become free to
 * transmit. The user will not be able to initiate uplink transmissions until the
 * automatic transmissions are done.
 *
 * @param[in] buffer - ( STATUS_ON / STATUS_OFF )
 */
int lora_rf_set_ar( STATUS_t status );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Get Automatic Replay
 *
 * For more info
 * @link lora_rf_set_ar @endlink
 *
 * @param[out] buffer
 */
int lora_rf_ar( char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Set RX 2 Settings
 *
 * Sets the data rate and frequency used for the second Receive window.
 *
 * The configuration of the receive window parameters should be in concordance
 * with the server configuration.
 *
 * @param[in] data_rate - ( 0 ~ 7 )
 * @param[in] freq - ( "863000000" ~ "870000000" / "433050000" ~ "434790000" )
 */
int lora_rf_set_rx2( uint8_t data_rate,
                     uint32_t freq );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Get RX 2 Settings
 *
 * Returns information about current rx2 settings for specified band.
 *
 * @param[in] band ( BND_433 / BND_868 )
 * @param[out] buffer
 */
int lora_rf_rx2( BND_t band,
                 char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Channels Frequency
 *
 * Sets the module operation frequency on a given channel ID.
 * The default channels (0-2) cannot be modified in terms of frequency.
 *
 * @note
 * If this parameter was previously saved to user EEPROM by issuing the
 * mac save command, after modifying its value, the mac save command
 * should be called again.
 *
 * @param[in] ch - ( 3 ~ 15 )
 * @param[in] freq - ( 863000000 ~ 870000000 / 433050000 ~ 434790000 )
 */
int lora_rf_set_ch_freq( uint8_t ch,
                         uint32_t freq );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Get Channels Frequency
 *
 * For more info
 * @link lora_rf_set_ch_freq @endlink
 *
 * @param[in] ch - ( 3 ~ 15 )
 * @param[out] buffer
 */
int lora_rf_ch_freq( uint8_t ch,
                     char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Set Channels Duty Cycle
 *
 * Sets the module operation duty cycle on a given channel ID.
 * The default settings consider only the three default channels (0-2), and
 * their default duty cycle is 0.33%. If a new channel is created either by the
 * server or by the user, all the channels (including the default ones) must be
 * updated by the user in terms of duty cycle to comply with the ETSI
 * regulations. If this parameter was previously saved to user EEPROM by
 * issuing the mac save command, after modifying its value, the mac save
 * command should be called again.
 *
 * @note
 * If this parameter was previously saved to user EEPROM by issuing the mac
 * save command, after modifying its value, the mac save command
 * should be called again.
 *
 * @param[in] ch ( 0 ~ 15 )
 * @param[in] dc - ( "0" ~ "65535" )
 */
int lora_rf_set_ch_dcycle( uint8_t ch,
                           uint16_t dc );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Get Channels Duty Cycle
 *
 * For more info
 * @link lora_rf_set_ch_freq @endlink
 *
 * @param ch ( 0 ~ 15 )
 * @param[out] buffer
 */
int lora_rf_ch_dcycle( uint8_t ch,
                       char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Set Channels Data Rate Range
 *
 * Sets the module allowed data rate range (min.- max.) allowed on a
 * given channel ID.
 * By doing this the module can vary data rates between the <min> and <max> on
 * the specified <ch>.
 *
 * @note
 * If this parameter was previously saved to user EEPROM by issuing the mac
 * save command, after modifying its value, the mac save command
 * should be called again.
 *
 * @param[in] ch ( 0 ~ 15 )
 * @param[in] min ( 0 ~ 7 )
 * @param[in] max ( 0 ~ 7 )
 */
int lora_rf_set_ch_dr_rng( uint8_t ch,
                           uint8_t min,
                           uint8_t max );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Get Channels Data Rate Range
 *
 * For more info
 * @link lora_rf_set_ch_dr_rng @endlink
 *
 * @param[in] ch ( 0 ~ 15 )
 * @param[out] buffer
 */
int lora_rf_ch_dr_rng( uint8_t ch,
                       char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Set Channels status
 *
 * Sets the use of the specified channel ID.
 * <ch> parameters (frequency, data range, duty cycle) must be issued prior to
 * enabling the status of that channel. If this parameter was previously saved
 * to user EEPROM by issuing the mac save command, after modifying its value,
 * the mac save command should be called again.
 *
 * @note
 * <ChannelId> parameters (frequency, data range, duty cycle) must be issued
 * prior to enabling the status of that channel.
 * If this parameter was previously saved to user EEPROM by issuing the mac
 * save command, after modifying its value, the mac save command should be
 * called again.
 *
 * @param[in] - ch ( 0 ~ 15 )
 * @param[in] - status ( STATUS_OFF / STATUS_ON )
 */
int lora_rf_set_ch_st( uint8_t ch,
                       STATUS_t status );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Get Channels status
 *
 * For more info
 * @link lora_rf_set_ch_st @endlink
 *
 * @param[in] - ch ( 0 ~ 15 )
 * @param[out] - buffer
 */
int lora_rf_ch_st( uint8_t ch,
                   char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Set Network Session Key
 *
 * Sets the network session key for the RN2483 module.
 * It should be modified with each session between the module and network.
 * The key should remain the same until the communication session between
 * devices is terminated.
 *
 * @note
 * If this parameter was previously saved to user EEPROM by issuing the
 * mac save command, after modifying its value, the mac save command
 * should be called again.
 *
 * @param[in] buffer - ( "00000000000000000000000000000000 ~
 *                       "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" )
 */
int lora_rf_set_ns_key( char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Set Application Session Key
 *
 * Sets the application session key for the RN2483 module.
 * This key is unique, created for each occurrence of communication, when the
 * network requests an action taken by the application.
 *
 * @note
 * If this parameter was previously saved to user EEPROM by issuing the
 * mac save command, after modifying its value, the mac save command
 * should be called again.
 *
 * @param[in] buffer - ( "00000000000000000000000000000000 ~
 *                       "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" )
 */
int lora_rf_set_as_key( char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Set Application Key
 *
 * Sets the application key for the module.
 * The application key is used to identify a grouping over module units which
 * perform the same or similar task.
 *
 * @note
 * If this parameter was previously saved to user EEPROM by issuing the
 * mac save command, after modifying its value, the mac save command
 * should be called again.
 *
 * @param[in] buffer - ( "00000000000000000000000000000000 ~
 *                       "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" )
 */
int lora_rf_set_app_key( char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Set Battery Level
 *
 * Sets the battery level needed for Device Status Answer frame
 * command response.
 *
 * @param[in] bat_lvl - ( 0 ~ 255 )
 */
int lora_rf_set_battery( uint8_t bat_lvl );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Set Link Check
 *
 * Sets the time interval for the link check process to be triggered.
 * A <lincheck> of ‘0’ will disable the link check process. When the time
 * interval expires, the next application packet that will be sent to the server
 * will include also a link check MAC command.
 *
 * @param[in] seconds - ( 0 ~ 65535 )
 */
int lora_rf_set_lnk_chk( uint16_t seconds );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Get MAC Band
 *
 * Gets the current frequency band in operation.
 *
 * The band reflects the module’s operation types.
 *
 * @param[out] - buffer
 */
int lora_rf_band( char *buffer );

/**
 * @brief Get RX 2 Delay
 *
 * Gets the interval value stored for receive window 2 delay.
 *
 * @param[out] - buffer
 */
int lora_rf_rx_2_dly( char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Duty Cycle Prescaler
 *
 * Gets the duty cycle prescaler which can only be configured by
 * the server.
 * The value of the prescaler can be configured only by the server through use
 * of the Duty Cycle Request frame. Upon reception of this command from the
 * server, the duty cycle prescaler is changed for all enabled channels.
 *
 * @param[out] - buffer
 */
int lora_rf_dcycle_ps( char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Get Demodulation Margine
 *
 * Gets the demodulation margin as received in the last
 * Link Check Answer frame.
 *
 * @param[out] - buffer
 */
int lora_rf_dem_marg( char *buffer );

/**
 * @ingroup MAC_CMD_LVL
 * @brief Module Status
 *
 * Gets the current status of the RN2483 module.
 *
 * <b>Response bits meaning :</b>
 * <ul><li><b> b0 :</b> Join status <ul>
 * <li>0 – network not joined</li><li>1 – network joined</li></ul>
 * </li><li><b> b1, b2, b3 :</b> Mac state<ul>
 * <li>0 – Idle (transmissions are possible)</li>
 * <li>1 – Transmission occurring</li>
 * <li>2 – Before the opening of Receive window 1</li>
 * <li>3 – Receive window 1 is open</li>
 * <li>4 – Between Receive window 1 and Receive window 2</li>
 * <li>5 – Receive window 2 is open</li>
 * <li>6 – Ack_timeout )</li></ul></li><li>
 * <b> b4 :</b> Automatic reply status<ul>
 * <li>0 – disabled</li><li>1 – enabled</li>
 * </ul></li><li><b> b5 : </b> ADR status<ul>
 * <li>0 – ADR is disabled</li><li>1 – ADR is enabled</li>
 * </ul></li><li><b> b6 : </b> Silent immediately status<ul>
 * <li>0 – disabled</li><li>1 – enabled</li></ul></li><li>
 * <b> b7 : </b> Mac pause status<ul><li>0 – mac is not paused</li>
 * <li>1 – mac is paused</li></ul></li><li><b> b8 : </b> RFU
 * </li><li><b> b9 : </b> Link check status<ul>
 * <li>0 – link check is disabled</li><li>1 – link check is enabled</li>
 * </ul></li><li><b> b10 : </b> Channels updated<ul>
 * <li>0 – not updated</li>
 * <li>1 – updated via CFList or NewChannelReq MAC command</li></ul>
 * </li><li><b> b11 :</b> Output power updated<ul><li>0 – not updated</li>
 * <li>1 – updated via LinkADRReq MAC command</li></ul></li> <li>
 * <b>b12 : </b> NbRep updated<ul><li>0 – not updated</li>
 * <li>1 – updated via LinkADRReq MAC command</li></ul></li><li>
 * <b> b13 : </b> Prescaler updated<ul><li>0 – not updated</li>
 * <li>1 – updated via DutyCycleReq MAC command</li>
 * </ul></li><li><b> b14 : </b> Second Receive window parameters updated<ul>
 * <li>0 – not updated</li><li>1 – updated RX ParamSetupReq command</li></ul>
 * </li><li><b> b15 : </b> TX timing setup updated<ul><li>0 – not updated</li>
 * <li>1 – updated via RX TimingSetupReq MAC command</li></ul></li></ul>
 *
 * @param[out] - buffer
 */
int lora_rf_status( char *buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Radio Receive
 *
 * Configures the radio to receive simple radio packets according to
 * prior configuration settings.
 * <window_size> is decimal number representing the number of symbols (for LoRa
 * modulation) or time out (for FSK modulation) that the receiver will be opened
 * Set <window_size> to ‘0’ in order to enable the continuous reception mode.
 * Continuous Reception mode will be exited once a valid packet is received.
 * The mac pause command must be called before any radio transmission or
 * reception, even if no MAC operations have been initiated before.
 * Ensure the radio Watchdog Timer time out is higher than the Receive
 * window size.
 *
 * This command may reply with two responses. The first response will be
 * received immediately after entering the command. If the command is valid.
 * Second reply will be received after the reception of a packet or after the
 * time out occurred.
 *
 * @note
 * Ensure the radio Watchdog Timer time out is higher than the Receive
 * window size.
 * The mac pause command must be called before any radio transmission or
 * reception, even if no MAC operations have been initiated before.
 *
 * @param[in] window_size - ( 0 ~ 65535 )
 * @param[out] - buffer
 */
int lora_rf_rx( uint16_t window_size, char *buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Radio Transmit
 *
 * Configures a simple radio packet transmission according to prior
 * configuration settings.
 * <data>: hexadecimal value representing the data to be transmitted, from 0
 * to 255 bytes for LoRa modulation and from 0 to 64 bytes for FSK modulation.
 * The mac pause command must be called before any radio transmission or
 * reception, even if no MAC operations have been initiated before.
 *
 * This command may reply with two responses. The first response will be
 * received immediately after entering the command. If the command is valid.
 * Second reply will be received after the effective transmission.
 *
 * @note
 * In order to meet ETSI regulations in the given frequency bands, the radio
 * has to use either Listen Before Talk (LBT) + Adaptive Frequency Agility
 * (AFA) or duty cycle limitations. By issuing the radio tx <data> command the
 * module does not perform LBT before transmission, thus the user has to make
 * sure that duty cycle limits are not violated.
 *
 * @param[in] buffer - ( data in hex format )
 */
int lora_rf_tx( char *buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Continuous Wave
 *
 * Puts the module into a Continuous Wave (cw) Transmission for system
 * tuning or certification use.
 *
 * CW mode allows the user to put the transceiver into Transmission mode to
 * observe the generated signal. By altering the settings for the radio the u
 * ser can observe the changes in transmissions levels.
 *
 * @note
 * Please note that using radio cw off resets the module, this command
 * being semantically identical to sys reset.
 *
 * @param[in] state - ( STATUS_ON / STATUS_OFF )
 */
int lora_rf_cw( STATUS_t state );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Set Data Shaping
 *
 * Sets data shaping for frequency shift keying (FSK) modulation
 * type.
 *
 * Modifies the data shaping applied to FSK transmissions. Entering any
 * <gfbt> other than 0 ( none ) will result in a Gaussian Filter BT being
 * applied to transmissions in FSK mode.
 *
 * @param[in] buffer - ( "none" / "1.0" / "0.5" / "0.3" )
 */
int lora_rf_set_bt( char *buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Get Data Shaping
 *
 * For more info
 * @link lora_rf_set_bt @endlink
 *
 * @param[out] - buffer
 */
int lora_rf_bt( char *buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Set Modulation Mode
 *
 * Sets the module Modulation mode.
 * Changes the modulation method being used by the module. Altering the mode of
 * operation does not affect previously set parameters, variables or registers.
 * FSK mode also allows GFSK transmissions when data shaping is enabled.
 *
 * @param[in] buffer - ( "lora" / "fsk" )
 */
int lora_rf_set_mod( MOD_t modulation );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Get Modulation Mode
 *
 * For more info
 * @link lora_rf_set_mod @endlink
 *
 * @param[out] - buffer
 */
int lora_rf_mod( char *buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Set Operation Frequency
 *
 * Sets the current operation frequency for the radio.:
 *
 * @param[in] buffer - ( 433000000 ~ 434800000 / 863000000 ~ 870000000 )
 */
int lora_rf_set_freq( uint32_t freq );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Get Operation Frequency
 *
 * For more info
 * @link lora_rf_set_freq @endlink
 *
 * @param[out] - buffer
 */
int lora_rf_freq( char *buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Set Radio Power
 *
 * Sets the output power level used by the radio during
 * transmission.
 * Changes the transceiver output power. However, note that the transceiver is
 * designed to transmit a maximum of +14 dBm. It is possible to set the output
 * power above the regulatory limits. This power setting allows some
 * compensation on the cable or transmission line loss.
 *
 * @param[in] buffer - ( -3 ~ 15 )
 */
int lora_rf_set_power( int8_t pwr );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Get Radio Power
 *
 * For more info
 * @link lora_rf_set_freq @endlink
 *
 * @param[out] - buffer
 */
int lora_rf_power( char *buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Set Spreading Factor
 *
 * Set the requested spreading factor (SF) to be used during
 * transmission.
 *
 * @param[in] buffer - ( "sf7" / "sf8" / "sf9" / "sf10" / "sf11" / "sf12" )
 */
int lora_rf_set_sf( char *buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Get Spreading Factor
 *
 * For more info
 * @link lora_rf_set_sf @endlink
 *
 * @param[out] - buffer
 */
int lora_rf_sf( char *buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Set Frequency Correction
 *
 * Set the value used by the automatic frequency correction
 * bandwidth.
 *
 * Modifies the automatic frequency correction bandwidth for
 * receiving / transmitting.
 *
 * @param[in] buffer - ( "250" / "125" / "62.5" / "31.3" / "15.6" / "7.8" /
 *                       "3.9" / "200" / "100" / "50" / "25" / "12.5" /
 *                       "6.3" / "3.1" / "166.7" / "83.3" / "41.7" /
 *                       "20.8" / "10.4" / "5.2" / "2.6" )
 */
int lora_rf_set_freq_corr( char *buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Get Frequency Correction
 *
 * For more info
 * @link lora_rf_set_freq_corr @endlink
 *
 * @param[out] - buffer
 */
int lora_rf_freq_corr( char *buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Set Receive Bandwidth
 *
 * Sets the operational receive bandwidth.
 *
 * @param[in] buffer - ( "250" / "125" / "62.5" / "31.3" / "15.6" / "7.8" /
 *                       "3.9" / "200" / "100" / "50" / "25" / "12.5" /
 *                       "6.3" / "3.1" / "166.7" / "83.3" / "41.7" /
 *                       "20.8" / "10.4" / "5.2" / "2.6" )
 */
int lora_rf_set_rx_bw( char *buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Get Receive Bandwidth
 *
 * For more info
 * @link lora_rf_set_rx_bw @endlink
 *
 * @param[out] - buffer
 */
int lora_rf_rx_bw( char *buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Set FSK Bit Rate
 *
 * Sets the frequency shift keying (FSK) bit rate.
 *
 * @param[in] buffer - ( 0 ~ 65535 )
 */
int lora_rf_set_fsk_br( uint16_t buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Get FSK Bit Rate
 *
 * For more info
 * @link lora_rf_set_fsk_br @endlink
 *
 * @param[out] - buffer
 */
int lora_rf_fsk_br( char *buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Set Frequency Deviation
 *
 * Sets the frequency deviation allowed by the end device during
 * operation.
 *
 * @param[in] buffer - ( 0 ~ 65535 )
 */
int lora_rf_set_freq_dev( uint16_t freq_dev );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Get Frequency Deviation
 *
 * For more info
 * @link lora_rf_set_freq_dev @endlink
 *
 * @param[out] - buffer
 */
int lora_rf_freq_dev( char *buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Set Preamble Length
 *
 * Sets the preamble length usage during transmissions.
 *
 * @param[in] buffer - ( "0" ~ "65535" )
 */
int lora_rf_set_pr_len( uint16_t pr_len );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Get Preamble Length
 *
 * For more info
 * @link lora_rf_set_pr_len @endlink
 *
 * @param[out] - buffer
 */
int lora_rf_pr_len( char *buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Set CRC
 *
 * Sets a CRC header useage.
 *
 * @param[in] buffer - ( STATUS_ON / STATUS_OFF )
 */
int lora_rf_set_crc( STATUS_t status );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Get CRC
 *
 * For more info
 * @link lora_rf_set_crc @endlink
 *
 * @param[out] - buffer
 */
int lora_rf_crc( char *buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Set IQ Inversion
 *
 * Sets IQ inversion usage.
 *
 * @param[in] buffer - ( STATUS_ON / STATUS_OFF )
 */
int lora_rf_set_iq_inv( STATUS_t status );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Get IQ Inversion
 *
 * For more info
 * @link lora_rf_set_iq_inv @endlink
 *
 * @param[out] - buffer
 */
int lora_rf_iq_inv( char *buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Set Coding Rate
 *
 * Sets or gets the coding rate used by the radio.
 * Modifies the coding rate currently being used by the radio.
 *
 * @param[in] buffer - ( "4/5" / "4/6" / "4/7" / "4/8" )
 */
int lora_rf_set_cr( char *buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Get Coding Rate
 *
 * For more info
 * @link lora_rf_cr @endlink
 *
 * @param[out] - buffer
 */
int lora_rf_cr( char *buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Set Watchdog
 *
 * Sets or gets the time-out limit for the radio Watchdog Timer.
 * Updates the time-out length, in milliseconds, applied to the radio Watchdog
 * Timer. If this functionality is enabled, then the Watchdog Timer is started
 * for every transceiver reception or transmission. The Watchdog Timer is
 * stopped when the operation in progress in finished.
 *
 * @note
 * Ensure the value configured for the Watchdog Timer matches the radio
 * configurations. For example, set the <watchDog> value to ‘0’ in order to
 * disable this functionality during the radio continuous reception.
 *
 * @param[in] wdog - ( 0 ~ 4294967295 )
 */
int lora_rf_set_wdog( uint32_t wdog );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Get Watchdog
 *
 * For more info
 * @link lora_rf_set_wdog @endlink
 *
 * @param[out] - buffer
 */
int lora_rf_wdog( char *buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Set Radio Bandwidth
 *
 * Sets the value used for the radio bandwidth.
 *
 * @param[in] buffer - ( 125 / 250 / 500 )
 */
int lora_rf_set_bw( uint16_t bw );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Get Radio Bandwidth
 *
 * For more info
 * @link lora_rf_set_bw @endlink
 *
 * @param[out] - buffer
 */
int lora_rf_bw( char *buffer );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Sync Word
 *
 * Set the sync word used.
 * Configures the sync word used during communication.
 *
 * @param[in] syncword - ( "00" ~ "FF" )
 */
int lora_rf_syncw( char *syncword );

/**
 * @ingroup RADIO_CMD_LVL
 * @brief Get Signal Noise Ratio
 *
 * Get the signal noise ratio ( SNR ) of the last received packet.
 *
 * @param[out] - buffer
 */
int lora_rf_snr( char *buffer );

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* LORA_RF_H_ */
/*** End of File **************************************************************/
