/**
 * @file rfb_comm_common.h
 * @author
 * @date
 * @brief Brief single line description use for indexing
 *
 * More detailed description can go here
 *
 *
 * @see http://
 */
#ifndef _RFB_COMM_COMMON_H_
#define _RFB_COMM_COMMON_H_
/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include "rf_mcu.h"
#include "rfb.h"
#if (MODULE_ENABLE(SUPPORT_MULTITASKING))
#include "sys_arch.h"
#endif

/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
#define RF_TX_Q_ID                             (0)
#define MAX_RX_BUFF_NUM                        (4)


#if (MODULE_ENABLE(SUPPORT_MULTITASKING))

/**************************************************************************************************
 * EXTERN DEFINITIONS
 *************************************************************************************************/
extern sys_queue_t g_rfb_event_queue_handle;


/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/

typedef struct
{
    uint16_t  length;     // Payload Length
    uint8_t   *p_data;    // Payload Data
} rfb_event_msg_t;

#endif

/**************************************************************************************************
 *    Global Prototypes
 *************************************************************************************************/
RFB_EVENT_STATUS rfb_comm_frequency_set(uint32_t rf_frequency);
RFB_EVENT_STATUS rfb_comm_single_tone_mode_set(uint8_t single_tone_mode);
RFB_EVENT_STATUS rfb_comm_rx_enable_set(bool rx_continuous, uint32_t rx_timeout);
RFB_EVENT_STATUS rfb_comm_rx_reserve_set(uint32_t rx_start_time, uint32_t rx_on_time);
RFB_EVENT_STATUS rfb_comm_rf_idle_set(void);
RFB_WRITE_TXQ_STATUS rfb_comm_tx_data_send(uint16_t packet_length, uint8_t *tx_data_address, uint8_t InitialCwAckRequest, uint8_t Dsn);
void rfb_comm_init(rfb_interrupt_event_t *_rfb_interrupt_event);
RFB_EVENT_STATUS rfb_comm_rssi_read(uint8_t *rssi);
RFB_EVENT_STATUS rfb_comm_agc_set(uint8_t agc_enable, uint8_t lna_gain, uint8_t vga_gain, uint8_t tia_gain);
RFB_EVENT_STATUS rfb_comm_rf_sleep_set(bool sleep_enable_flag);
RFB_EVENT_STATUS rfb_comm_fw_version_get(uint32_t *rfb_version);
RFB_EVENT_STATUS rfb_comm_auto_state_set(bool rxOnWhenIdle);
RFB_EVENT_STATUS rfb_comm_clock_set(uint8_t modem_type, uint8_t band_type, uint8_t clock_mode);
RFB_EVENT_STATUS rfb_comm_tx_power_set(uint8_t band_type, uint8_t power_index);
RFB_EVENT_STATUS rfb_comm_tx_power_set_oqpsk(uint8_t band_type, uint8_t power_index);
RFB_EVENT_STATUS rfb_comm_key_set(uint8_t *pKey);
RFB_EVENT_STATUS rfb_comm_pta_control_set(uint8_t enable, uint8_t inverse);
RFB_EVENT_STATUS rfb_comm_wake_on_radio_set(uint32_t rf_frequency, uint16_t rx_on_time, uint32_t sleep_time);
RFB_EVENT_STATUS rfb_comm_2ch_scan_frequency_set(uint8_t scan_enable, uint32_t rf_freq1, uint32_t rf_freq2);
#endif

