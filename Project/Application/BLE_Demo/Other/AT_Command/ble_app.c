/** @file ble_app.c
 *
 * @brief BLE AT command demo.
 * @details AT command: One for central role with TRSP service in client/server role and \n
 *                another one for Peripherl role with TRSP service in clientserver role.
 */

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "task_hci.h"
#include "ble_event.h"
#include "ble_printf.h"
#include "ble_profile.h"
#include "ble_app.h"
#include "atcmd.h"
#include "shell.h"

/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
#define APP_STACK_SIZE                  1024
#define BLE_APP_CB_QUEUE_SIZE           16
#define APP_ISR_QUEUE_SIZE              2
#define APP_REQ_QUEUE_SIZE              6
#define APP_QUEUE_SIZE                  (BLE_APP_CB_QUEUE_SIZE + APP_ISR_QUEUE_SIZE + APP_REQ_QUEUE_SIZE)

// LINK please refer to "ble_profile_def.c" --> att_db_mapping
#define APP_TRSP_C_HOST_ID              0         // TRSPC: Central
#define APP_TRSP_P_HOST_ID              1         // TRSPS: Peripheral

/**************************************************************************************************
 *    PUBLIC VARIABLES
 *************************************************************************************************/

/**************************************************************************************************
 *    LOCAL VARIABLES
 *************************************************************************************************/
static xQueueHandle g_app_msg_q;
static SemaphoreHandle_t semaphore_cb;
static SemaphoreHandle_t semaphore_isr;
static SemaphoreHandle_t semaphore_app;
static ble_cfg_t gt_app_cfg;

static atcmd_t ble_atcmd;
/**************************************************************************************************
 *    FUNCTION DECLARATION
 *************************************************************************************************/
bool app_request_set(uint8_t host_id, app_request_t request, bool from_isr);
static void svcs_gatts_data_init(ble_svcs_gatts_data_t *p_data);
static void svcs_trsps_data_init(ble_svcs_trsps_data_t *p_data);
static ble_err_t ble_init(void);
static void ble_app_main(app_req_param_t *p_param);
static void print_service_handles(uint8_t host_id);

extern void sleep_cli_console(void);
/**************************************************************************************************
 *    LOCAL FUNCTIONS
 *************************************************************************************************/
/* ------------------------------
 *  Handler
 * ------------------------------
 */
static void ble_svcs_dis_evt_handler(ble_evt_att_param_t *p_param)
{
    uint8_t *p_data;

    if (p_param->gatt_role == BLE_GATT_ROLE_CLIENT)
    {
        /* ----------------- Handle event from server ----------------- */
        switch (p_param->event)
        {
        case BLESERVICE_DIS_MANUFACTURER_NAME_STRING_READ_RSP_EVENT:
            p_data = pvPortMalloc(p_param->length + 1);
            if (p_data != NULL)
            {
                memcpy(p_data, p_param->data, p_param->length);
                *(p_data + p_param->length) = '\0';
                printf("Manufacturer name: %s\n", p_data);
                vPortFree(p_data);
            }
            break;

        case BLESERVICE_DIS_FIRMWARE_REVISION_STRING_READ_RSP_EVENT:
            p_data = pvPortMalloc(p_param->length + 1);
            if (p_data != NULL)
            {
                memcpy(p_data, p_param->data, p_param->length);
                *(p_data + p_param->length) = '\0';
                printf("FW rev.: %s\n", p_data);
                vPortFree(p_data);
            }
            break;

        default:
            break;
        }
    }
}

static void ble_svcs_trsps_evt_handler(ble_evt_att_param_t *p_param)
{
    uint8_t *p_data;

    if (p_param->gatt_role == BLE_GATT_ROLE_CLIENT)
    {
        /* ----------------- Handle event from server ----------------- */
        switch (p_param->event)
        {
        case BLESERVICE_TRSPS_UDATNI01_NOTIFY_EVENT:
        case BLESERVICE_TRSPS_UDATNI01_INDICATE_EVENT:
            p_data = pvPortMalloc(p_param->length + 1);
            if (p_data != NULL)
            {
                memcpy(p_data, p_param->data, p_param->length);
                *(p_data + p_param->length) = '\0';
                printf("From Host Id: %d, Data: %s\n", p_param->host_id, p_data);
                vPortFree(p_data);
            }
            break;

        default:
            break;
        }
    }

    if (p_param->gatt_role == BLE_GATT_ROLE_SERVER)
    {
        /* ----------------- Handle event from client ----------------- */
        switch (p_param->event)
        {
        case BLESERVICE_TRSPS_UDATRW01_WRITE_EVENT:
        case BLESERVICE_TRSPS_UDATRW01_WRITE_WITHOUT_RSP_EVENT:
            p_data = pvPortMalloc(p_param->length + 1);
            if (p_data != NULL)
            {
                memcpy(p_data, p_param->data, p_param->length);
                *(p_data + p_param->length) = '\0';
                printf("From Host Id: %d, Data: %s\n", p_param->host_id, p_data);
                vPortFree(p_data);
            }
            break;

        case BLESERVICE_TRSPS_UDATRW01_READ_EVENT:
        {
            ble_gatt_data_param_t p_data_param;
            uint8_t readData[] = "UDATRW01 data";

            p_data_param.host_id = p_param->host_id;
            p_data_param.handle_num = p_param->handle_num;
            p_data_param.p_data = readData;
            p_data_param.length = sizeof(readData) / sizeof(readData[0]) - 1;
            ble_cmd_gatt_read_rsp(&p_data_param);
        }
        break;

        default:
            break;
        }
    }
}

static void ble_evt_handler(ble_evt_param_t *p_param)
{
    switch (p_param->event)
    {
    case BLE_COMMON_EVT_READ_FILTER_ACCEPT_LIST_SIZE:
    {
        ble_evt_common_read_filter_accept_list_size_t *p_accept_list_param = (ble_evt_common_read_filter_accept_list_size_t *)&p_param->event_param.ble_evt_common.param.evt_read_accept_list_size;

        if (p_accept_list_param->status != BLE_HCI_ERR_CODE_SUCCESS)
        {
            printf("Read filter accept list size failed, error code = 0x%02x\n", p_accept_list_param->status);
        }
        else
        {
            printf("Read filter accept list size = %d\n", p_accept_list_param->filter_accept_list_size);
        }
    }
    break;

    case BLE_ADV_EVT_SET_ENABLE:
    {
        ble_evt_adv_set_adv_enable_t *p_adv_enable = (ble_evt_adv_set_adv_enable_t *)&p_param->event_param.ble_evt_adv.param.evt_set_adv_enable;

        if (p_adv_enable->status == BLE_HCI_ERR_CODE_SUCCESS)
        {
            if (p_adv_enable->adv_enabled == true)
            {
                ble_app_link_info[APP_TRSP_P_HOST_ID].state = STATE_ADVERTISING;
                printf("Advertising...\n");
            }
            else
            {
                ble_app_link_info[APP_TRSP_P_HOST_ID].state = STATE_STANDBY;
                printf("End Advertising...\n");
            }
        }
        else
        {
            printf("Advertising enable failed.\n");
        }
    }
    break;

    case BLE_SCAN_EVT_SET_ENABLE:
    {
        ble_evt_scan_set_scan_enable_t *p_scan_enable = (ble_evt_scan_set_scan_enable_t *)&p_param->event_param.ble_evt_scan.param.evt_set_scan_enable;
        if (p_scan_enable->status == BLE_HCI_ERR_CODE_SUCCESS)
        {
            if (p_scan_enable->scan_enabled == true)
            {
                printf("Scanning...\n");
            }
            else
            {
                printf("End Scanning...\n");
            }
        }
        else
        {
            printf("Scanning enable failed, status=%d.\n", p_scan_enable->status);
        }
    }
    break;

    case BLE_SCAN_EVT_ADV_REPORT:
    {
        ble_evt_scan_adv_report_t *p_scan_param = (ble_evt_scan_adv_report_t *)&p_param->event_param.ble_evt_scan.param.evt_adv_report;
        uint8_t name[32];
        uint8_t length;

        // show device name, address and RSSI value
        if ((ble_cmd_scan_report_adv_data_parsing(p_scan_param, GAP_AD_TYPE_LOCAL_NAME_COMPLETE, name, &length) == BLE_ERR_OK) ||
                (ble_cmd_scan_report_adv_data_parsing(p_scan_param, GAP_AD_TYPE_LOCAL_NAME_SHORTENED, name, &length) == BLE_ERR_OK))
        {
            name[length] = '\0';
            printf("Found [Name:%s] [Addr Type:%d,  Addr:%02x:%02x:%02x:%02x:%02x:%02x] [RSSI= %d]\n",
                   name,
                   p_scan_param->peer_addr.addr_type,
                   p_scan_param->peer_addr.addr[5], p_scan_param->peer_addr.addr[4],
                   p_scan_param->peer_addr.addr[3], p_scan_param->peer_addr.addr[2],
                   p_scan_param->peer_addr.addr[1], p_scan_param->peer_addr.addr[0],
                   p_scan_param->rssi);
        }
    }
    break;

    case BLE_GAP_EVT_CONN_CANCEL:
    {
        ble_evt_gap_create_conn_t *p_create_conn_param = (ble_evt_gap_create_conn_t *)&p_param->event_param.ble_evt_gap.param.evt_create_conn;

        if (p_create_conn_param->status != BLE_HCI_ERR_CODE_SUCCESS)
        {
            printf("Create connection cancel failed, error code = 0x%02x\n", p_create_conn_param->status);
        }
        else
        {
            printf("Creating a connection is cancelled.\n");
        }
    }
    break;

    case BLE_GAP_EVT_CONN_COMPLETE:
    {
        ble_evt_gap_conn_complete_t *p_conn_param = (ble_evt_gap_conn_complete_t *)&p_param->event_param.ble_evt_gap.param.evt_conn_complete;

        if (p_conn_param->status != BLE_HCI_ERR_CODE_SUCCESS)
        {
            printf("Connect failed, error code = 0x%02x\n", p_conn_param->status);
        }
        else
        {
            ble_app_link_info[p_conn_param->host_id].state = STATE_CONNECTED;

            printf("Connected, ID=%d, Connected to %02x:%02x:%02x:%02x:%02x:%02x\n",
                   p_conn_param->host_id,
                   p_conn_param->peer_addr.addr[5],
                   p_conn_param->peer_addr.addr[4],
                   p_conn_param->peer_addr.addr[3],
                   p_conn_param->peer_addr.addr[2],
                   p_conn_param->peer_addr.addr[1],
                   p_conn_param->peer_addr.addr[0]);

            printf("Interval:%d, Latency:%d, Timeout:%d\n",
                   p_conn_param->conn_interval,
                   p_conn_param->periph_latency,
                   p_conn_param->supv_timeout);
        }
    }
    break;

    case BLE_GAP_EVT_CONN_PARAM_UPDATE:
    {
        ble_evt_gap_conn_param_update_t *p_conn_param = (ble_evt_gap_conn_param_update_t *)&p_param->event_param.ble_evt_gap.param.evt_conn_param_update;

        if (p_conn_param->status != BLE_HCI_ERR_CODE_SUCCESS)
        {
            printf("Connection update failed, error code = 0x%02x\n", p_conn_param->status);
        }
        else
        {
            printf("Connection updated\n");
            printf("ID: %d, ", p_conn_param->host_id);
            printf("Interval: %d, ", p_conn_param->conn_interval);
            printf("Latency: %d, ", p_conn_param->periph_latency);
            printf("Supervision Timeout: %d\n", p_conn_param->supv_timeout);
        }
    }
    break;

    case BLE_GAP_EVT_PHY_READ:
    case BLE_GAP_EVT_PHY_UPDATE:
    {
        ble_evt_gap_phy_t *p_phy_param = (ble_evt_gap_phy_t *)&p_param->event_param.ble_evt_gap.param.evt_phy;
        if (p_phy_param->status != BLE_HCI_ERR_CODE_SUCCESS)
        {
            printf("PHY update/read failed, error code = 0x%02x\n", p_phy_param->status);
        }
        else
        {
            printf("PHY updated/read, ID: %d, TX PHY: %d, RX PHY: %d\n", p_phy_param->host_id, p_phy_param->tx_phy, p_phy_param->rx_phy);
        }
    }
    break;

    case BLE_ATT_GATT_EVT_MTU_EXCHANGE:
    {
        ble_evt_mtu_t *p_mtu_param = (ble_evt_mtu_t *)&p_param->event_param.ble_evt_att_gatt.param.ble_evt_mtu;
        printf("MTU Exchanged, ID:%d, size: %d\n", p_mtu_param->host_id, p_mtu_param->mtu);
    }
    break;

    case BLE_ATT_GATT_EVT_DATA_LENGTH_CHANGE:
    {
        ble_evt_data_length_change_t *p_data_len_param = (ble_evt_data_length_change_t *)&p_param->event_param.ble_evt_att_gatt.param.ble_evt_data_length_change;
        printf("Data length changed, ID: %d\n", p_data_len_param->host_id);
        printf("MaxTxOctets: %d  MaxTxTime:%d\n", p_data_len_param->max_tx_octets, p_data_len_param->max_tx_time);
        printf("MaxRxOctets: %d  MaxRxTime:%d\n", p_data_len_param->max_rx_octets, p_data_len_param->max_rx_time);
    }
    break;

    case BLE_GAP_EVT_DISCONN_COMPLETE:
    {
        ble_evt_gap_disconn_complete_t *p_disconn_param = (ble_evt_gap_disconn_complete_t *)&p_param->event_param.ble_evt_gap.param.evt_disconn_complete;
        if (p_disconn_param->status != BLE_HCI_ERR_CODE_SUCCESS)
        {
            printf("Disconnect failed, error code = 0x%02x\n", p_disconn_param->status);
        }
        else
        {
            ble_app_link_info[p_disconn_param->host_id].state = STATE_STANDBY;
            printf("Disconnect, ID:%d, Reason:0x%02x\n", p_disconn_param->host_id, p_disconn_param->reason);
        }
    }
    break;

    case BLE_ATT_GATT_EVT_DB_PARSE_COMPLETE:
    {
        ble_evt_att_db_parse_complete_t *p_parsing_param = (ble_evt_att_db_parse_complete_t *)&p_param->event_param.ble_evt_att_gatt.param.ble_evt_att_db_parse_complete;

        if (p_parsing_param->result == BLE_HCI_ERR_CODE_SUCCESS)
        {
            // Get all service client handles and related information
            client_handles_get(p_parsing_param->host_id);
            printf("DB Parsing completed, ID:%d status:0x%02x\n", p_parsing_param->host_id, p_parsing_param->result);

            print_service_handles(p_parsing_param->host_id);
        }
        else
        {
            printf("DB Parsing failed, ID:%d status:0x%02x\n", p_parsing_param->host_id, p_parsing_param->result);
        }
    }
    break;

    case BLE_GAP_EVT_RSSI_READ:
    {
        ble_evt_gap_rssi_read_t *p_rssi_param = (ble_evt_gap_rssi_read_t *)&p_param->event_param.ble_evt_gap.param.evt_rssi;

        if (p_rssi_param->status != BLE_HCI_ERR_CODE_SUCCESS)
        {
            printf("RSSI read failed, error code = 0x%02x\n", p_rssi_param->status);
        }
        else
        {
            printf("ID=%d, RSSI=%d\n", p_rssi_param->host_id, p_rssi_param->rssi);
        }
    }
    break;

    case BLE_SM_EVT_STK_GENERATION_METHOD:
    {
        ble_evt_sm_stk_gen_method_t *p_stk_method = (ble_evt_sm_stk_gen_method_t *)&p_param->event_param.ble_evt_sm.param.evt_stk_gen_method;
        if (p_stk_method->key_gen_method == PASSKEY_ENTRY)
        {
            //Start scanning user-entered passkey.
        }
        else if (p_stk_method->key_gen_method == PASSKEY_DISPLAY)
        {
            //user can generate a 6-digit random code, and display it for pairing.
        }
    }
    break;

    case BLE_SM_EVT_PASSKEY_CONFIRM:
    {
        //ble_evt_sm_passkey_confirm_param_t *p_sm_confirm_param = (ble_evt_sm_passkey_confirm_param_t *)&p_param->event_param.ble_evt_sm.param.evt_passkey_confirm_param;
    }
    break;

    case BLE_SM_EVT_AUTH_STATUS:
    {
        ble_evt_sm_auth_status_t *p_auth_result = (ble_evt_sm_auth_status_t *)&p_param->event_param.ble_evt_sm.param.evt_auth_status;
        printf("AUTH Report, ID:%d , STATUS:%d\n", p_auth_result->host_id, p_auth_result->status);
    }
    break;

    default:
        break;
    }
}

/* ------------------------------
 *  Methods
 * ------------------------------
 */
bool app_request_set(uint8_t host_id, app_request_t request, bool from_isr)
{
    app_queue_t p_app_q;

    p_app_q.event = 0; // from BLE
    p_app_q.param_type = QUEUE_TYPE_APP_REQ;
    p_app_q.param.app_req.host_id = host_id;
    p_app_q.param.app_req.app_req = request;

    if (from_isr == false)
    {
        if (xSemaphoreTake(semaphore_app, 0) == pdTRUE)
        {
            p_app_q.from_isr = false;
            if (xQueueSendToBack(g_app_msg_q, &p_app_q, 0) != pdTRUE)
            {
                // send error
                xSemaphoreGive(semaphore_app);
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        BaseType_t context_switch = pdFALSE;

        if (xSemaphoreTakeFromISR(semaphore_isr, &context_switch) == pdTRUE)
        {
            p_app_q.from_isr = true;
            context_switch = pdFALSE;
            if (xQueueSendToBackFromISR(g_app_msg_q, &p_app_q, &context_switch) != pdTRUE)
            {
                context_switch = pdFALSE;
                xSemaphoreGiveFromISR(semaphore_isr, &context_switch);
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    return true;
}

static void print_service_handles(uint8_t host_id)
{
    ble_info_link0_t *p_profile_info = (ble_info_link0_t *)ble_app_link_info[host_id].profile_info;

    printf("\n------BLE Profiles Information------\n");
    printf("Host Id = %d\n", host_id);
    printf("[GAP]:Client\n");
    printf("DEVICE NAME Handle: %d\n", p_profile_info->svcs_info_gaps.client_info.handles.hdl_device_name);
    printf("APPEARANCE handle: %d\n", p_profile_info->svcs_info_gaps.client_info.handles.hdl_appearance);
    printf("Preferred connection parameter Handle: %d\n\n", p_profile_info->svcs_info_gaps.client_info.handles.hdl_peripheral_preferred_connection_parameters);

    printf("[GATT]:Client\n");
    printf("service changed handle: %d\n", p_profile_info->svcs_info_gatts.client_info.handles.hdl_service_changed);
    printf("service changed CCCD handle: %d\n\n", p_profile_info->svcs_info_gatts.client_info.handles.hdl_service_changed_cccd);

    printf("[DIS]:Client\n");
    printf("Serial Number handle: %d\n", p_profile_info->svcs_info_dis.client_info.handles.hdl_serial_number_string);
    printf("Manufacturer Name handle: %d\n", p_profile_info->svcs_info_dis.client_info.handles.hdl_manufacturer_name_string);
    printf("System ID handle: %d\n", p_profile_info->svcs_info_dis.client_info.handles.hdl_system_id);
    printf("Firmware Revision handle: %d\n", p_profile_info->svcs_info_dis.client_info.handles.hdl_firmware_revision_string);
    printf("Model Number handle: %d\n", p_profile_info->svcs_info_dis.client_info.handles.hdl_model_number_string);
    printf("Hardware Revision handle: %d\n", p_profile_info->svcs_info_dis.client_info.handles.hdl_hardware_revision_string);
    printf("Software Revision handle: %d\n", p_profile_info->svcs_info_dis.client_info.handles.hdl_software_revision_string);
    printf("PnP ID handle: %d\n\n", p_profile_info->svcs_info_dis.client_info.handles.hdl_pnp_id);

    printf("[TRSP]:Client\n");
    printf("TRSP Read 01 handle: %d\n", p_profile_info->svcs_info_trsps.client_info.handles.hdl_udatr01);
    printf("TRSP Notification / Indication 01 handle: %d\n", p_profile_info->svcs_info_trsps.client_info.handles.hdl_udatni01);
    printf("TRSP Notification / Indication 01 CCCD handle: %d\n", p_profile_info->svcs_info_trsps.client_info.handles.hdl_udatni01_cccd);
    printf("TRSP Write 01 handle: %d\n\n", p_profile_info->svcs_info_trsps.client_info.handles.hdl_udatrw01);

    printf("[GAP]:Server\n");
    printf("DEVICE NAME Handle: %d\n", p_profile_info->svcs_info_gaps.server_info.handles.hdl_device_name);
    printf("APPEARANCE handle: %d\n", p_profile_info->svcs_info_gaps.server_info.handles.hdl_appearance);
    printf("Preferred connection parameter Handle: %d\n\n", p_profile_info->svcs_info_gaps.server_info.handles.hdl_peripheral_preferred_connection_parameters);

    printf("[GATT]:Server\n");
    printf("service changed handle: %d\n", p_profile_info->svcs_info_gatts.server_info.handles.hdl_service_changed);
    printf("service changed CCCD handle: %d\n\n", p_profile_info->svcs_info_gatts.server_info.handles.hdl_service_changed_cccd);

    printf("[DIS]:Server\n");
    printf("Serial Number handle: %d\n", p_profile_info->svcs_info_dis.server_info.handles.hdl_serial_number_string);
    printf("Manufacturer Name handle: %d\n", p_profile_info->svcs_info_dis.server_info.handles.hdl_manufacturer_name_string);
    printf("System ID handle: %d\n", p_profile_info->svcs_info_dis.server_info.handles.hdl_system_id);
    printf("Firmware Revision handle: %d\n", p_profile_info->svcs_info_dis.server_info.handles.hdl_firmware_revision_string);
    printf("Model Number handle: %d\n", p_profile_info->svcs_info_dis.server_info.handles.hdl_model_number_string);
    printf("Hardware Revision handle: %d\n", p_profile_info->svcs_info_dis.server_info.handles.hdl_hardware_revision_string);
    printf("Software Revision handle: %d\n", p_profile_info->svcs_info_dis.server_info.handles.hdl_software_revision_string);
    printf("PnP ID handle: %d\n\n", p_profile_info->svcs_info_dis.server_info.handles.hdl_pnp_id);

    printf("[TRSP]:Server\n");
    printf("TRSP Read 01 handle: %d\n", p_profile_info->svcs_info_trsps.server_info.handles.hdl_udatr01);
    printf("TRSP Notification / Indication 01 handle: %d\n", p_profile_info->svcs_info_trsps.server_info.handles.hdl_udatni01);
    printf("TRSP Notification / Indication 01 CCCD handle: %d\n", p_profile_info->svcs_info_trsps.server_info.handles.hdl_udatni01_cccd);
    printf("TRSP Write 01 handle: %d\n\n", p_profile_info->svcs_info_trsps.server_info.handles.hdl_udatrw01);
    printf("------------------------------------\n");
}

/* ------------------------------
 *  Application Task
 * ------------------------------
 */
static void app_evt_indication_cb(uint32_t data_len)
{
    int i32_err;
    uint8_t *p_buf;
    app_queue_t p_app_q;

    if (xSemaphoreTake(semaphore_cb, 0) == pdPASS)
    {
        do
        {
            p_buf = pvPortMalloc(data_len);
            if (!p_buf)
            {
                xSemaphoreGive(semaphore_cb);
                break;
            }

            p_app_q.event = 0;
            i32_err = ble_event_msg_recvfrom(p_buf, &data_len);
            p_app_q.param_type = QUEUE_TYPE_OTHERS;
            p_app_q.param.pt_tlv = (ble_tlv_t *)p_buf;

            if (i32_err == 0)
            {
                while (xQueueSendToBack(g_app_msg_q, &p_app_q, 1) != pdTRUE);
            }
            else
            {
                info_color(LOG_RED, "[%s] err = %d !\n", __func__, (ble_err_t)i32_err);
                vPortFree(p_buf);
                xSemaphoreGive(semaphore_cb);
            }
        } while (0);
    }
}

static void app_main_task(void)
{
    ble_err_t status;
    app_queue_t p_app_q;

    status = BLE_ERR_OK;

    // BLE default setting and profile init
    status = ble_init();
    if (status != BLE_ERR_OK)
    {
        printf("[DEBUG_ERR] ble_init() fail: %d\n", status);
        while (1);
    }

    for (;;)
    {
        if (xQueueReceive(g_app_msg_q, &p_app_q, portMAX_DELAY) == pdTRUE)
        {
            switch (p_app_q.param_type)
            {
            case QUEUE_TYPE_APP_REQ:
            {
                if (p_app_q.from_isr == true)
                {
                    xSemaphoreGive(semaphore_isr);
                }
                else
                {
                    xSemaphoreGive(semaphore_app);
                }
                ble_app_main(&p_app_q.param.app_req);
            }
            break;

            case QUEUE_TYPE_OTHERS:
            {
                if (p_app_q.param.pt_tlv != NULL)
                {
                    switch (p_app_q.param.pt_tlv->type)
                    {
                    case BLE_APP_GENERAL_EVENT:
                        ble_evt_handler((ble_evt_param_t *)p_app_q.param.pt_tlv->value);
                        break;

                    case BLE_APP_SERVICE_EVENT:
                    {
                        ble_evt_att_param_t *p_svcs_param = (ble_evt_att_param_t *)p_app_q.param.pt_tlv->value;

                        switch (p_svcs_param->gatt_role)
                        {
                        case BLE_GATT_ROLE_CLIENT:
                            att_db_link[p_svcs_param->host_id].p_client_db[p_svcs_param->cb_index]->att_handler(p_svcs_param);
                            break;

                        case BLE_GATT_ROLE_SERVER:
                            att_db_link[p_svcs_param->host_id].p_server_db[p_svcs_param->cb_index]->att_handler(p_svcs_param);
                            break;

                        default:
                            break;
                        }
                    }
                    break;

                    default:
                        break;
                    }

                    // free
                    vPortFree(p_app_q.param.pt_tlv);
                    xSemaphoreGive(semaphore_cb);
                }
            }
            break;

            default:
                break;
            }
        }
    }
}

static void ble_app_main(app_req_param_t *p_param)
{
    atcmd_main_handle(&ble_atcmd);
}

/* ------------------------------
 *  Application Initializations
 * ------------------------------
 */
static void svcs_gatts_data_init(ble_svcs_gatts_data_t *p_data)
{
    p_data->service_changed_cccd = 0;
}

static void svcs_trsps_data_init(ble_svcs_trsps_data_t *p_data)
{
    p_data->udatni01_cccd = 0;
}

static ble_err_t server_profile_init(uint8_t host_id)
{
    ble_err_t status;
    ble_info_link1_t *p_profile_info;

    status = BLE_ERR_OK;
    p_profile_info = (ble_info_link1_t *)ble_app_link_info[host_id].profile_info;

    // set link's state
    ble_app_link_info[host_id].state = STATE_STANDBY;

    do
    {
        // GAP Related
        // -------------------------------------
        status = ble_svcs_gaps_init(host_id, BLE_GATT_ROLE_SERVER, &(p_profile_info->svcs_info_gaps), NULL);
        if (status != BLE_ERR_OK)
        {
            break;
        }

        // GATT Related
        // -------------------------------------
        status = ble_svcs_gatts_init(host_id, BLE_GATT_ROLE_SERVER, &(p_profile_info->svcs_info_gatts), NULL);
        if (status != BLE_ERR_OK)
        {
            break;
        }

        // DIS Related
        // -------------------------------------
        status = ble_svcs_dis_init(host_id, BLE_GATT_ROLE_SERVER, &(p_profile_info->svcs_info_dis), ble_svcs_dis_evt_handler);
        if (status != BLE_ERR_OK)
        {
            break;
        }

        // TRSPS Related
        // -------------------------------------
        status = ble_svcs_trsps_init(host_id, BLE_GATT_ROLE_SERVER, &(p_profile_info->svcs_info_trsps), ble_svcs_trsps_evt_handler);
        if (status != BLE_ERR_OK)
        {
            break;
        }

        svcs_gatts_data_init(&p_profile_info->svcs_info_gatts.server_info.data);
        svcs_trsps_data_init(&p_profile_info->svcs_info_trsps.server_info.data);

    } while (0);

    return status;
}

static ble_err_t client_profile_init(uint8_t host_id)
{
    ble_err_t status;
    ble_info_link1_t *p_profile_info;

    status = BLE_ERR_OK;
    p_profile_info = (ble_info_link1_t *)ble_app_link_info[host_id].profile_info;

    // set link's state
    ble_app_link_info[host_id].state = STATE_STANDBY;

    do
    {
        // GAP Related
        // -------------------------------------
        status = ble_svcs_gaps_init(host_id, BLE_GATT_ROLE_CLIENT, &(p_profile_info->svcs_info_gaps), NULL);
        if (status != BLE_ERR_OK)
        {
            break;
        }

        // GATT Related
        // -------------------------------------
        status = ble_svcs_gatts_init(host_id, BLE_GATT_ROLE_CLIENT, &(p_profile_info->svcs_info_gatts), NULL);
        if (status != BLE_ERR_OK)
        {
            break;
        }

        // DIS Related
        // -------------------------------------
        status = ble_svcs_dis_init(host_id, BLE_GATT_ROLE_CLIENT, &(p_profile_info->svcs_info_dis), ble_svcs_dis_evt_handler);
        if (status != BLE_ERR_OK)
        {
            break;
        }

        // TRSPS Related
        // -------------------------------------
        status = ble_svcs_trsps_init(host_id, BLE_GATT_ROLE_CLIENT, &(p_profile_info->svcs_info_trsps), ble_svcs_trsps_evt_handler);
        if (status != BLE_ERR_OK)
        {
            break;
        }

        svcs_gatts_data_init(&p_profile_info->svcs_info_gatts.client_info.data);
        svcs_trsps_data_init(&p_profile_info->svcs_info_trsps.client_info.data);

    } while (0);

    return status;
}

static ble_err_t ble_init(void)
{
    uint8_t i;
    ble_err_t status;

    status = BLE_ERR_OK;
    do
    {
        atcmd_init(&ble_atcmd);

        //Init the ble param
        status = atcmd_ble_param_init(&ble_atcmd);
        if (status != BLE_ERR_OK)
        {
            break;
        }

        for (i = 0; i < MAX_CONN_NO_APP; i++)
        {
            // BLE Server profile init
            status = server_profile_init(i);
            if (status != BLE_ERR_OK)
            {
                break;
            }

            // BLE Client profile init
            status = client_profile_init(i);
            if (status != BLE_ERR_OK)
            {
                break;
            }
        }
    } while (0);

    // register shell command
    extern sh_cmd_t g_cli_cmd_atplus;
    shell_register_cmd(&g_cli_cmd_atplus);

    return status;
}

/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/
void uart_rx_data_handle(uint8_t *data, uint8_t length)
{
    atcmd_uart_handle(&ble_atcmd, data, length);
#if(configUSE_TICKLESS_IDLE==1)
    sleep_cli_console();
    Lpm_Low_Power_Unmask(LOW_POWER_MASK_BIT_TASK_BLE_APP);
#endif
}

void app_init(void)
{
    // banner
    printf("---------------------------------------------------\n");
    printf("  BLE AT command demo: start...\n");
    printf("---------------------------------------------------\n");
    printf("***************************************************\n");
    printf("BLE Host id List\n");
    printf("Role : Central, Host id = %d\n", APP_TRSP_C_HOST_ID);
    printf("Role : Peripheral, Host id = %d\n", APP_TRSP_P_HOST_ID);
    printf("***************************************************\n");

    // BLE Stack init
    gt_app_cfg.pf_evt_indication = app_evt_indication_cb;
    task_hci_init();
    ble_host_stack_init(&gt_app_cfg);
    printf("BLE stack initial...\n");

    // application task
    g_app_msg_q = xQueueCreate(APP_QUEUE_SIZE, sizeof(app_queue_t));
    xTaskCreate((TaskFunction_t)app_main_task, "app", APP_STACK_SIZE, NULL, TASK_PRIORITY_APP, NULL);

    semaphore_cb = xSemaphoreCreateCounting(BLE_APP_CB_QUEUE_SIZE, BLE_APP_CB_QUEUE_SIZE);
    semaphore_isr = xSemaphoreCreateCounting(APP_ISR_QUEUE_SIZE, APP_ISR_QUEUE_SIZE);
    semaphore_app = xSemaphoreCreateCounting(APP_REQ_QUEUE_SIZE, APP_REQ_QUEUE_SIZE);
}
