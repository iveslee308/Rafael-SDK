/** @file
 *
 * @brief BLE HID peripheral role demo.
 *
 */

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "bsp.h"
#include "bsp_console.h"
#include "util_log.h"
#include "util_printf.h"
#include "ble_app.h"

/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/

/**************************************************************************************************
 *    LOCAL FUNCTIONS
 *************************************************************************************************/
static void bsp_btn_event_handle(bsp_event_t event)
{
    switch (event)
    {
    case BSP_EVENT_BUTTONS_0:
        // disable sleep mode
        Lpm_Low_Power_Mask(LOW_POWER_MASK_BIT_TASK_BLE_APP);
        break;

    case BSP_EVENT_BUTTONS_1:
        privacy_status_change();
        break;

    case BSP_EVENT_UART_RX_RECV:
    case BSP_EVENT_UART_RX_DONE:
    {
#if ((IO_CAPABILITY_SETTING == KEYBOARD_ONLY) || (IO_CAPABILITY_SETTING == KEYBOARD_DISPLAY) || (IO_CAPABILITY_SETTING == DISPLAY_YESNO) )
        static uint8_t rx_buffer[6];
        static uint8_t index = 0;
        char ch;

        bsp_console_stdin_str(&ch, 1);

        if ((ch == '\n') || (ch == '\r'))
        {
            // set passkey
            passkey_set(rx_buffer, index);

            // reset index
            index = 0;

            // enable sleep mode
            Lpm_Low_Power_Unmask(LOW_POWER_MASK_BIT_TASK_BLE_APP);
        }
        else
        {
            if (index == 6)
            {
                index = 0;
            }
            rx_buffer[index++] = ch;
        }
#endif
    }
    break;

    case BSP_EVENT_UART_BREAK:
        // diable sleep mode
        Lpm_Low_Power_Mask(LOW_POWER_MASK_BIT_TASK_BLE_APP);
        break;

    default:
        break;
    }
}

/* pin mux setting init*/
static void pin_mux_init(void)
{
    int i;

    /*set all pin to gpio, except GPIO16, GPIO17 */
    for (i = 0; i < 32; i++)
    {
        if ((i != 16) && (i != 17))
        {
            pin_set_mode(i, MODE_GPIO);
        }
    }
    return;
}

/**************************************************************************************************
 *    GLOBAL FUNCTIONS
 *************************************************************************************************/
int main(void)
{
    /* pinmux init */
    pin_mux_init();

    /* delay init */
    Delay_Init();

    /* low power mode init */
    Lpm_Set_Low_Power_Level(LOW_POWER_LEVEL_SLEEP0);
    Lpm_Enable_Low_Power_Wakeup(LOW_POWER_WAKEUP_GPIO0);
    Lpm_Enable_Low_Power_Wakeup(LOW_POWER_WAKEUP_UART_RX);

    /* initil Button and press button0 to disable sleep mode & initil Console & UART */
    bsp_init((BSP_INIT_BUTTONS |
              BSP_INIT_DEBUG_CONSOLE |
              BSP_INIT_UART), bsp_btn_event_handle);

    /* retarget stdout for utility & initial utility logging */
    utility_register_stdout(bsp_console_stdout_char, bsp_console_stdout_string);
    util_log_init();

    NVIC_SetPriority(Uart0_IRQn, 5);
    NVIC_SetPriority(Uart1_IRQn, 5);

    /* enable protocol debug message */
    //util_log_on(UTIL_LOG_PROTOCOL);

    /* application init */
    app_init();

    /* scheduler start */
    vTaskStartScheduler();

    while (1)
    {
    }
}
