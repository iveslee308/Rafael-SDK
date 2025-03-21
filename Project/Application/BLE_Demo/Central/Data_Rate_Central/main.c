/** @file
 *
 * @brief BLE TRSP Cental role demo.
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
static void timer_handler(uint32_t timer_id)
{
    if (timer_id == APP_HW_TIMER_ID)
    {
        timer_count_update();
    }
}

static void bsp_btn_event_handle(bsp_event_t event)
{
    switch (event)
    {
    case BSP_EVENT_BUTTONS_0:
        // disable sleep mode
        Lpm_Low_Power_Mask(LOW_POWER_MASK_BIT_TASK_BLE_APP);
        break;

    case BSP_EVENT_UART_RX_RECV:
    case BSP_EVENT_UART_RX_DONE:
    {
        char ch;
        bsp_console_stdin_str(&ch, 1);

        if (uart_data_handler(ch) == true)
        {
            // recieved '\n' or '\r' --> enable sleep mode
            Lpm_Low_Power_Unmask(LOW_POWER_MASK_BIT_TASK_BLE_APP);
        }
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

/* timer init*/
static void hw_timer0_init(void)
{
    timer_config_mode_t cfg;

    cfg.int_en = ENABLE;
    cfg.mode = TIMER_PERIODIC_MODE;
    /*the input clock is 32M/s, so it will become 4M ticks per second */
    cfg.prescale = TIMER_PRESCALE_8;

    Timer_Open(APP_HW_TIMER_ID, cfg, timer_handler);
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

    /* HW timer0 init */
    hw_timer0_init();

    /* application init */
    app_init();

    /* scheduler start */
    vTaskStartScheduler();

    while (1)
    {
    }
}
