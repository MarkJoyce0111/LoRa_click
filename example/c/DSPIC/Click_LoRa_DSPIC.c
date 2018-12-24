/*
Example for LoRa Click

    Date          : Feb 2018.
    Author        : MikroE Team

Test configuration dsPIC :
    
    MCU                : P33FJ256GP710A
    Dev. Board         : EasyPIC Fusion v7
    dsPIC Compiler ver : v7.0.1.0

---
Description :

The application is composed of three sections :

- System Initialization - Initializes UART module and CS pin, RST pin as output and INT pin as input
- Application Initialization - Initializes driver init and LoRa init
- Application Task - (code snippet) - The transceiver sends one by one byte in UART which is for the work of the lora,
the Receiver mode, receives one byte and logs it on usbuart. Receiver mode is default mode.

*/

#include "Click_LoRa_types.h"
#include "Click_LoRa_config.h"


uint8_t sendMessage[8] = { 'M', 'i', 'k', 'r', 'o', 'E',' ', 0 };
char tmp_txt[ 50 ];
char sendHex[ 50 ];
char rspTxt[ 50 ];
char rsp_data[10];
uint8_t cnt;
uint8_t send_data;
uint8_t _data;
uint8_t rxState;
uint8_t txState;

void systemInit()
{
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_INT_PIN, _GPIO_INPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_RST_PIN, _GPIO_OUTPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_CS_PIN, _GPIO_OUTPUT );
    mikrobus_uartInit( _MIKROBUS1, &_LORA_UART_CFG[0] );
    mikrobus_logInit( _LOG_USBUART_B, 57600 );

    mikrobus_logWrite("--- System init ---", _LOG_LINE );
}

void lora_cbk( char* response )
{

}
void applicationInit()
{
    lora_uartDriverInit( (T_LORA_P)&_MIKROBUS1_GPIO, (T_LORA_P)&_MIKROBUS1_UART );
    lora_init( 0, &lora_cbk );
    // start
    lora_cmd( &LORA_CMD_SYS_GET_VER[0], &tmp_txt[0] );
    lora_cmd( &LORA_CMD_MAC_PAUSE[0],  &tmp_txt[0] );
    mikrobus_logWrite("mac pause",_LOG_LINE);
    mikrobus_logWrite(&tmp_txt[0],_LOG_LINE);

    lora_cmd( &LORA_CMD_RADIO_SET_WDT[0], &tmp_txt[0] );
    mikrobus_logWrite("radio set wdt 0",_LOG_LINE);
    mikrobus_logWrite(&tmp_txt[0],_LOG_LINE);
}

void applicationTask()
{
    lora_process();
// RECEIVER
    rxState = lora_rx( &LORA_ARG_0[0], &tmp_txt[0]);
    if( rxState == 0)
    {
        _data = xtoi(&tmp_txt[11]);
        mikrobus_logWrite( &_data,_LOG_BYTE);
        mikrobus_logWrite( " ",_LOG_LINE);
    }

// TRANSCEIVER
/*for (cnt = 0; cnt < 7; cnt++ )
    {
        send_data = sendMessage[cnt];
        IntToHex(send_data,sendHex);
        txState = lora_tx( &sendHex[0] );
        if( txState == 0)
		{
            mikrobus_logWrite( "  Response : ",_LOG_TEXT );
            mikrobus_logWrite( &tmp_txt[0],_LOG_LINE );
        }
        Delay_1sec();
    }*/
}

void main()
{
    systemInit();
    applicationInit();

    while (1)
    {
        applicationTask();
    }
}