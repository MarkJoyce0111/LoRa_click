/**
 * @file lora_hal.h
 *
 * @brief Hal layer for the Mikroelektronika's Lora click board.
 *
 ******************************************************************************/

#ifndef LORA_HAL_H_
#define LORA_HAL_H_

#include <stdint.h>

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
void lora_hal_init
(
        int rts_use,
        int cts_use
);

/**
 * @brief Hardware Reset Pin
 *
 * Sets RST pin to logic high.
 */
void lora_hal_rst
(
        int logic
);

/**
 * @brief RTS Line
 *
 * Sets RST pin to logic low.
 */
int lora_hal_rts
(
        void
);

/**
 * @brief CTS Line
 *
 * @param[in] stop - ( 0 stop sending / 1 - send more )
 */
void lora_hal_cts
(
        int stop
);

/**
 * @brief lora_rf_hal_write
 * @param ch
 *
 * @return
 */
void lora_hal_write
(
        char ch
);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /*LORA_HAL_H_*/

/*** End of File **************************************************************/
