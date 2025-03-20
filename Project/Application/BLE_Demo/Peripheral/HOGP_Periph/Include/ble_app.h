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
#include "ble_security_manager.h"

/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
#define IO_CAPABILITY_SETTING       KEYBOARD_DISPLAY   //selected IO Capability: KEYBOARD_ONLY / NOINPUT_NOOUTPUT / KEYBOARD_DISPLAY / DISPLAY_ONLY

/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/
typedef enum
{
    QUEUE_TYPE_APP_REQ,   /**< Application queue type: application request.*/
    QUEUE_TYPE_OTHERS,    /**< Application queue type: others including BLE events and BLE service events.*/
} app_queue_param_type_t;

typedef enum
{
    APP_REQUEST_IDLE,                    /**< Application request event: idle.*/
    APP_REQUEST_ADV_START,               /**< Application request event: advertising start.*/
    APP_REQUEST_HIDS_PASSKEY_ENTRY,      /**< Application request event: passkey entry.*/
    APP_REQUEST_HIDS_NUMERIC_COMP_ENTRY, /**< Application request event: numeric comparison entry.*/
    APP_REQUEST_HIDS_NTF,                /**< Application request event: HID notification.*/
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

/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/

/** @brief BLE initialization.
 * @details Initial BLE stack and application task.
 *
 * @return none.
 */
void app_init(void);


/** @brief Set pairing passkey.
 *
 * @param[in] p_data: a pointer to the passkey.
 * @param[in] length: data length.
 *
 * @return none.
 */
void passkey_set(uint8_t *p_data, uint8_t length);


#ifdef __cplusplus
};
#endif

#endif /* __BLE_APP_H__*/
