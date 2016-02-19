> ![MikroE](http://www.mikroe.com/img/designs/beta/logo_small.png)
> #Lora RF Click#
> ##By [MikroElektronika](http://www.mikroe.com)
---

## Installation
Use the package manager to install the library for your architecture.

### Test communication with the module - are all posible radio functions 

Test is for PIC32 MCUs.
```
#include "lora_rf.h"

sbit RF_LORA_RST at LATC1_bit;
sbit RF_LORA_RST_Direction at TRISC1_bit;

char rsp[ 256 ];

void system_init( void );

void system_init( )
{
    RF_LORA_RST_Direction = 0;
    Delay_ms( 100 );

    UART5_Init( 115200 );
    Delay_ms( 100 );
    UART2_Init( 57600 );
    Delay_ms( 100 );

    AD1PCFG = 0xFFFF;

    U2IP0_bit = 0;
    U2IP1_bit = 1;
    U2IP2_bit = 1;
    U2RXIE_bit = 1;
    EnableInterrupts();
}

void get_response( char *resp )
{
    UART5_Write_Text( resp );
    UART5_Write_Text( "\r\n" );
}

void main()
{
    system_init();
    lora_rf_init();
    lora_rf_callback( get_response );
    
    lora_rf_sleep( 300 );
    lora_rf_reset();
    lora_rf_set_eeprom( 0x301, "AA" );
    lora_rf_eeprom( 0x301, rsp );
    lora_rf_fw_ver( rsp );
    lora_rf_vdd( rsp );
    lora_rf_hw_eui( rsp );
    lora_rf_mac_reset( BND_868 );
    lora_rf_join( JM_ABP );
    lora_rf_save();
    lora_rf_force_en();
    lora_rf_pause( rsp );
    lora_rf_set_dev_addr( "0A0A0A0A" );
    lora_rf_dev_addr( rsp );
    lora_rf_set_dev_eui( "000000000000000E" );
    lora_rf_dev_eui( rsp );
    lora_rf_set_app_eui( "000000000000FFFE" );
    lora_rf_app_eui( rsp );
    lora_rf_set_pwridx( 5 );
    lora_rf_pwridx( rsp );
    lora_rf_set_dr( 5 );
    lora_rf_dr( rsp );
    lora_rf_set_adr( STATUS_ON );
    lora_rf_adr( rsp );
    lora_rf_set_retr( 200 );
    lora_rf_retr( rsp );
    lora_rf_set_rx_1_dly( 2000 );
    lora_rf_rx_1_dly( rsp );
    lora_rf_set_ar( STATUS_ON );
    lora_rf_ar( rsp );
    lora_rf_set_rx2( 5, 86500000 );
    lora_rf_rx2( BND_868, rsp );
    lora_rf_set_ch_freq( 5, 865000000 );
    lora_rf_ch_freq( 5, rsp );
    lora_rf_set_ch_dcycle( 5, 2000 );
    lora_rf_ch_dcycle( 5, rsp );
    lora_rf_set_ch_dr_rng( 5, 3, 6 );
    lora_rf_ch_dr_rng( 5, rsp );
    lora_rf_set_ns_key( "000000000000000000000000000000AE" );
    lora_rf_set_as_key( "000000000000000000000000000AB000" );
    lora_rf_set_app_key( "00000000000000000000CE0000000000" );
    lora_rf_set_battery( 125 );
    lora_rf_set_lnk_chk( 2000 );
    lora_rf_band( rsp );
    lora_rf_rx_2_dly( rsp );
    lora_rf_dcycle_ps( rsp );
    lora_rf_dem_marg( rsp );
    lora_rf_status( rsp );
    lora_rf_rx( 50, "AC" );
    lora_rf_tx( "AB" );
    lora_rf_cw( STATUS_ON );
    lora_rf_cw( STATUS_OFF );
    lora_rf_set_bt( "1.0" );
    lora_rf_bt( rsp );
    lora_rf_set_mod( MOD_LORA );
    lora_rf_mod( rsp );
    lora_rf_set_freq( 864000000 );
    lora_rf_freq( rsp );
    lora_rf_set_power( 4 );
    lora_rf_power( rsp );
    lora_rf_set_sf( "sf7" );
    lora_rf_sf( rsp );
    lora_rf_set_freq_corr( "250" );
    lora_rf_freq_corr( rsp );
    lora_rf_set_rx_bw( "250" );
    lora_rf_rx_bw( rsp );
    lora_rf_set_fsk_br( 20000 );
    lora_rf_fsk_br( rsp );
    lora_rf_set_freq_dev( 20000 );
    lora_rf_freq_dev( rsp );
    lora_rf_set_pr_len( 20000 );
    lora_rf_pr_len( rsp );
    lora_rf_set_crc( STATUS_ON );
    lora_rf_crc( rsp );
    lora_rf_set_iq_inv( STATUS_ON );
    lora_rf_iq_inv( rsp );
    lora_rf_set_cr( "4/6" );
    lora_rf_cr( rsp );
    lora_rf_set_wdog( 5000 );
    lora_rf_wdog( rsp );
    lora_rf_set_bw( 125 );
    lora_rf_bw( rsp );
    lora_rf_syncw( "FE" );
    lora_rf_snr( rsp );

    while( 1 )
    {
        lora_rf_process();
    }
}

void UART2interrupt() iv IVT_UART_2 ilevel 6 ics ICS_AUTO
{
      if( IFS1 & ( 1 << U2RXIF ))
      {
           char tmp = UART2_Read();
           lora_rf_rx_isr( tmp );
           U2RXIF_bit = 0;
      }
}

```
