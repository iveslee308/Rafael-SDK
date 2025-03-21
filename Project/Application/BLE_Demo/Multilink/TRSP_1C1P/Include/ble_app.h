#ifndef __BLE_APP_H__
#define __BLE_APP_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdint.h>
#include "ble_api.h"


/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/


/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/
typedef enum
{
    QUEUE_TYPE_APP_REQ,   /**< Application queue type: application request.*/
    QUEUE_TYPE_OTHERS,    /**< Application queue type: others including BLE events and BLE service events.*/
} app_queue_param_type;

typedef enum
{
    STATE_STANDBY,          /**< Application state: standby.*/
    STATE_ADVERTISING,      /**< Application state: advertising.*/
    STATE_SCANNING,
    STATE_INITIATING,
    STATE_CONNECTED,        /**< Application state: connected.*/
} ble_state_t;

typedef struct
{
    uint8_t   host_id;      /**< Application request parameter: host id.*/
    uint16_t  app_req;      /**< Application request parameter: @ref app_request_t "application request event".*/
} app_req_param_t;

typedef enum
{
    APP_REQUEST_IDLE = 0x00,
    APP_REQUEST_SCAN_START,
    APP_REQUEST_SCAN_STOP,
    APP_REQUEST_CREATE_CONN,
    APP_REQUEST_TRSPC_MULTI_CMD,
    APP_REQUEST_ADV_START,
    APP_REQUEST_ADV_STOP,
    APP_REQUEST_DATA_SEND,
    APP_REQUEST_DISCONNECT,
    APP_REQUEST_PROCESS_UART_CMD,
} app_request_t;


typedef struct
{
    uint8_t   event;        /**< Application queue parameter: event.*/
    uint8_t   from_isr;     /**< Application queue parameter: Dose the Request come from interruption? */
    uint16_t  param_type;   /**< Application queue parameter: @ref app_queue_param_type "application queue type".*/
    union
    {
        app_req_param_t  app_req;   /**< Application queue parameter: application request event. */
        ble_tlv_t        *pt_tlv;   /**< Application queue parameter: parameters (type: @ref ble_event_t, length, and value). */
    } param;
} app_queue_t;

/**************************************************************************************************
 *    EXTERN DEFINITIONS
 *************************************************************************************************/
extern uint8_t g_trsp_mtu[2];
extern uint8_t tx_data_buff[2][1024];
extern uint16_t tx_index[2];

/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/

/** @brief BLE initialization.
 * @details Initial BLE stack and application task.
 *
 * @return none.
 */
void app_init(void);


/** @brief Application UART data handler.
 *
 * @param[in] ch: input character.
 *
 * @return true: ready to enable sleep mode.
 */
bool uart_data_handler(char ch);


/** @brief Application Resuet Set command.
 *
 * @param[in] host_id : the link's id.
 * @param[in] request : request parameter.
 * @param[in] from_isr: Is Request from interrupt.
 *
 * @return true: Send OK, false: Send fail..
 */
bool app_request_set(uint8_t host_id, app_request_t request, bool from_isr);


#ifdef __cplusplus
};
#endif

#endif /* __BLE_APP_H__*/
