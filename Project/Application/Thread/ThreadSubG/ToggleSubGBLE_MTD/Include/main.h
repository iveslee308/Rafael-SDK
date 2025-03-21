
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <openthread-core-config.h>
#include <openthread/config.h>

#include <openthread/cli.h>
#include <openthread/diag.h>
#include <openthread/tasklet.h>
#include <openthread/platform/logging.h>
#include <openthread/thread.h>
#include <openthread/thread_ftd.h>
#include <openthread/udp.h>
#include <openthread/logging.h>
#include <openthread/cli.h>

#include "openthread-system.h"
#include "cli/cli_config.h"
#include "common/code_utils.hpp"
#include "common/debug.hpp"
#include "cm3_mcu.h"
#include "mem_mgmt.h"

/* Utility Library APIs */
#include "util_log.h"
#include "util_printf.h"

#include <stdint.h>
#include "ble_api.h"

/*app_commission.c*/
#define APP_DOOR_SENSOR_USE 0

typedef struct
{
    uint8_t br_mac[8];
    uint8_t br_networkkey[16];
    uint8_t br_channel;
    uint16_t br_panid;
} __attribute__((packed)) app_commission_data_t;

typedef struct
{
    uint8_t started;
    app_commission_data_t data;
} __attribute__((packed)) app_commission_t;

typedef struct
{
    uint8_t mask;
    uint16_t lens;
    app_commission_data_t data;
    uint8_t crc;
    uint8_t mask_end;
} __attribute__((packed)) ble_commission_data_t;
//=============================================================================
//                Public Function Declaration
//=============================================================================

void app_commission_start();
void app_commission_erase();
void app_commission_data_write(app_commission_data_t *a_commission_data);
bool app_commission_data_check();
void app_commission_get(app_commission_t *a_commission);

/*app_ble_task.h*/
typedef enum
{
    QUEUE_TYPE_APP_REQ,   /**< Application queue type: application request.*/
    QUEUE_TYPE_OTHERS,    /**< Application queue type: others including BLE events and BLE service events.*/
} app_queue_param_type_t;

typedef enum
{
    APP_REQUEST_IDLE,             /**< Application request event: idle.*/
    APP_REQUEST_ADV_START,        /**< Application request event: advertising start.*/
    APP_REQUEST_TRSPS_DATA_SEND,  /**< Application request event: TRSP server data send.*/
} app_request_t;

typedef enum
{
    STATE_STANDBY,        /**< Application state: standby.*/
    STATE_ADVERTISING,    /**< Application state: advertising.*/
    STATE_CONNECTED,      /**< Application state: connected.*/
} ble_state_t;


typedef struct
{
    uint8_t   host_id;    /**< Application request parameter: host id.*/
    uint16_t  app_req;    /**< Application request parameter: @ref app_request_t "application request event".*/
} app_req_param_t;


typedef struct
{
    uint8_t   event;        /**< Application queue parameter: event.*/
    uint8_t   from_isr;     /**< Application queue parameter: Dose the Request come from interruption? */
    uint16_t  param_type;   /**< Application queue parameter: @ref app_queue_param_type_t "application queue type".*/
    union
    {
        app_req_param_t  app_req;   /**< Application queue parameter: application request event. */
        ble_tlv_t        *pt_tlv;   /**< Application queue parameter: parameters (type: @ref ble_event_t, length, and value). */
    } param;
} app_queue_t;

/** @brief BLE initialization.
 * @details Initial BLE stack and application task.
 *
 * @return none.
 */
void ble_app_init(void);


/** @brief Application UART data handler.
 *
 * @param[in] ch: input character.
 *
 * @return true: ready to enable sleep mode.
 */
bool uart_data_handler(char ch);

/*app_thread_task.h*/
#if PLAFFORM_CONFIG_ENABLE_SUBG == TRUE
#define DEF_CHANNEL 1
#else
#define DEF_CHANNEL 11
#endif

#define SENSOR_COMMAND_LENS_MAX 256

typedef struct
{
    uint16_t Preamble;       //0xFAFB
    uint8_t FrameLengths;
    uint8_t FrameSN;
    uint8_t FrameClass;      ///< 0x00: read from leader, 0x01: trigger from child, 0x10: write from leader to child,(MSB bit(1) is ACK bit)
    uint8_t NetWorkId[8];    ///< ChildId(extaddr)
    uint8_t PID;             ///< 0: one door sensor, 1: one smart plug, 2: one sps30, 3: one ens160 ....
    uint8_t CID;             ///< Command ID(0: off, 1: on,  2:ack ota version , 3: wait ota)
    uint8_t CommandLengths;
    uint8_t CommandData[SENSOR_COMMAND_LENS_MAX];
    uint16_t FrameCRC;
} __attribute__((packed)) app_sensor_data_t;


typedef enum
{
    APP_SENSOR_CONTROL_EVENT = 0x1,
    APP_SENSOR_GET_OTA_VERSION_EVENT,
    APP_SENSOR_WAIT_OTA_EVENT,
    APP_SENSOR_DATA_RECEIVED
} app_sensor_event_t;

typedef struct
{
    uint8_t event;
    uint8_t data[300];
    uint16_t data_lens;
} __attribute__((packed)) app_sensor_event_queue_t;

/* record the interrupt event triggered in deep sleep*/
extern uint8_t g_sensor_event;
/* record the button 1 had hold event*/
extern bool g_button1_hold;

void thread_app_task_start();
void thread_app_sensor_data_generate(uint8_t event);
int thread_app_sensor_data_parse(uint8_t *payload, uint16_t payloadlength, void *data);
void thread_app_sensor_data_queue_push(uint8_t event, uint8_t *data, uint16_t data_lens);

/*app_udp.c*/
uint8_t app_sock_init(otInstance *instance);
otError app_udp_send(otIp6Address dst_addr, uint8_t *data, uint16_t data_lens);

#ifdef __cplusplus
};
#endif
#endif //__MAIN_H