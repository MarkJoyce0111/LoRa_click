![MikroE](http://www.mikroe.com/img/designs/beta/logo_small.png)

---

# LoRa Click

- **CIC Prefix**  : LORA
- **Author**      : Katarina Perendic
- **Verison**     : 1.0.0
- **Date**        : Feb 2018.

---

### Software Support

We provide a library for the LoRa Click on our [LibStock](https://libstock.mikroe.com/projects/view/1643/lora-click) 
page, as well as a demo application (example), developed using MikroElektronika 
[compilers](http://shop.mikroe.com/compilers). The demo can run on all the main 
MikroElektronika [development boards](http://shop.mikroe.com/development-boards).

**Library Description**

Library cover all functionalities of the click board through simplified API calls.

Key functions :

- ```uint8_t lora_tx( char *buffer )``` - Transmit function,send message provided using buffer
- ```uint8_t lora_rx(char* window_size, char *response)``` - Receive function, provides message using response buffer
- ```void lora_process()``` - Library state machine, must be placed inside infinite loop

**Examples Description**

The demo application is composed of three sections :

- System Initialization - Initializes UART module and CS pin, RST pin as output and INT pin as input
- Application Initialization - Initializes driver and LoRa module found on click board
- Application Task - (code snippet) - Can be used as receiver od transmiter, it is necessary to uncomment appropirate part of the code.
The transmiter sends one by one byte each one second while receiver receives message using continous mode and logs received data on UART. 

```.c
void applicationTask()
{
    lora_process();

// RECEIVER
    rxState = lora_rx( 0, &tmp_txt[0]);

    if (rxState == 0)
    {
        _data = xtoi(&tmp_txt[11]);
        mikrobus_logWrite( &_data,_LOG_BYTE);
        mikrobus_logWrite( " ",_LOG_LINE);
    }

// TRANSMITER
    for (cnt = 0; cnt < 7; cnt++)
    {
        send_data = sendMessage[cnt];
        IntToHex(send_data,sendHex);
        txState = lora_tx( &sendHex[0] );
        
        if (txState == 0)
        {
            mikrobus_logWrite( "  Response : ",_LOG_TEXT );
            mikrobus_logWrite( tmp_txt,_LOG_LINE );
        }
        Delay_1sec();
    }
}
```

The full application code, and ready to use projects can be found on our 
[LibStock](https://libstock.mikroe.com/projects/view/1643/lora-click) page.

mikroE Libraries used in the example:

- UART
- Conversions

**Additional notes and informations**

Depending on the development board you are using, you may need 
[USB UART click](http://shop.mikroe.com/usb-uart-click), 
[USB UART 2 Click](http://shop.mikroe.com/usb-uart-2-click) or 
[RS232 Click](http://shop.mikroe.com/rs232-click) to connect to your PC, for 
development systems with no UART to USB interface available on the board. The 
terminal available in all Mikroelektronika 
[compilers](http://shop.mikroe.com/compilers), or any other terminal application 
of your choice, can be used to read the message.

---
---
