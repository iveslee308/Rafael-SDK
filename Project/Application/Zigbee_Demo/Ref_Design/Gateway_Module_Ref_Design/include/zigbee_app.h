// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
#ifndef __ZIGBEE_APP_H__
#define __ZIGBEE_APP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "uart_handler.h"
#include "zigbee_stack_api.h"

#define ZB_ZCL_SUPPORT_CLUSTER_OTA_UPGRADE
#define ZB_HA_ENABLE_OTA_UPGRADE_SERVER

#define UART_TRANSPARENT_CMD 0x66

typedef enum
{
    APP_INIT_EVT    = 0,
    APP_NOT_JOINED_EVT,
    APP_IDLE_EVT,
    APP_ZB_START_EVT,
    APP_ZB_RESET_EVT,
} app_main_evt_t;

typedef enum
{
    APP_QUEUE_ZIGBEE_EVT    = 0,
    APP_QUEUE_BLE_EVT,
    APP_QUEUE_ISR_BUTTON_EVT,
    APP_QUEUE_UART_MSG_EVT,
} app_queue_evt_t;

typedef struct APP_DEVICE_ENTRY
{
    uint8_t valid     : 2;
    uint8_t joined    : 2;
    uint8_t cap       : 2;
    uint8_t           : 2;
    uint16_t short_addr;
    uint8_t ieee_addr[8];
    uint16_t deviceID;
    uint16_t in_cluster_count;
    uint16_t clusterID[10];
    uint8_t ep_counts;
    uint8_t ep_list[10];
} app_device_entry_t;

typedef struct APP_DB
{
    app_device_entry_t device_table[32];
} app_db_t;

extern zb_af_device_ctx_t simple_desc_gateway_ctx;
extern app_db_t gt_app_db;

void app_db_update(void);
void zigbee_app_init(void);
void zigbee_app_evt_change(uint32_t evt);
void zigbee_app_dump_device_table(void);
void zigbee_app_queue_send_to(uint32_t event, sys_tlv_t *pt_tlv);
void raf_cmd_cfm_handler(sys_tlv_t *pt_tlv);

uart_handler_data_sts_t zigbee_gateway_cmd_parser(uint8_t *pBuf, uint16_t plen, uint16_t *datalen, uint16_t *offset);
void zigbee_gateway_cmd_proc(uint8_t *pBuf, uint32_t len);
void zigbee_gateway_cmd_send(uint32_t cmd_id, uint16_t addr, uint8_t addr_mode, uint8_t src_endp, uint8_t *pParam, uint32_t len);

void zigbee_zcl_msg_read_rsp_cb_reg(void *cb);
void zigbee_zcl_msg_write_rsp_cb_reg(void *cb);
void zigbee_zcl_msg_cfg_report_rsp_cb_reg(void *cb);

uart_handler_data_sts_t xmodem_parser(uint8_t *pBuf, uint16_t plen, uint16_t *datalen, uint16_t *offset);
void xmodem_recv(sys_tlv_t *pt_tlv);

#ifdef __cplusplus
};
#endif
#endif /* __ZIGBEE_APP_H__ */
