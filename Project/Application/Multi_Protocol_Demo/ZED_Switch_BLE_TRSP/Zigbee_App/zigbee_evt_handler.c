/**
 * Copyright (c) 2021 All Rights Reserved.
 */
/** @file zigbee_evt_handler.c
 *
 * @author Rex
 * @version 0.1
 * @date 2021/12/23
 * @license
 * @description
 */

//=============================================================================
//                Include
//=============================================================================
/* OS Wrapper APIs*/
#include "sys_arch.h"

/* Utility Library APIs */
#include "util_printf.h"
#include "util_log.h"

/* ZigBee Stack Library APIs */
#include "zigbee_stack_api.h"
#include "zigbee_app.h"

#include "bsp_led.h"

//=============================================================================
//                Private Function Declaration
//=============================================================================
extern uint8_t long_addr[8];
static void _zdo_evt_start(sys_tlv_t *pt_tlv);
static void _zdo_evt_rejoin(sys_tlv_t *pt_tlv);
static void _zdo_evt_device_leave_idc(sys_tlv_t *pt_tlv);
static void(*zdo_evt_idc_func_list[])(sys_tlv_t *) =
{
    [ZIGBEE_EVT_TYPE_START_IDC - ZIGBEE_EVT_TYPE_START_IDC] = _zdo_evt_start,
    [ZIGBEE_EVT_TYPE_DEVICE_ANNCE_IDC - ZIGBEE_EVT_TYPE_START_IDC] = NULL,
    [ZIGBEE_EVT_TYPE_LEAVE_IDC - ZIGBEE_EVT_TYPE_START_IDC] = _zdo_evt_device_leave_idc,
    [ZIGBEE_EVT_TYPE_DEVICE_ASSOCIATED_IDC - ZIGBEE_EVT_TYPE_START_IDC] = NULL,
    [ZIGBEE_EVT_TYPE_REJOIN_FAILURE_IDC - ZIGBEE_EVT_TYPE_START_IDC] = _zdo_evt_rejoin,
    [ZIGBEE_EVT_TYPE_PANID_CONFLICT_IDC - ZIGBEE_EVT_TYPE_START_IDC] = NULL,
};

static void _zdo_evt_rejoin(sys_tlv_t *pt_tlv)
{
    if (zigbee_app_join_continue(APP_ZB_REJOIN) == TRUE)
    {
        info_color(LOG_RED, "Device do rejoin\n");
        zigbee_app_evt_change(APP_START_REJOIN_EVT, FALSE);
    }
    else
    {
        info_color(LOG_YELLOW, "Device suspend rejoin\n");
        bsp_led_Off(ZB_NWK_FINDING_INDICATOR);
    }
}
static void _zdo_evt_start(sys_tlv_t *pt_tlv)
{
    zigbee_nwk_start_idc_t *pt_start_idc = (zigbee_nwk_start_idc_t *)pt_tlv->value;

    if (pt_start_idc->status != 0)
    {

        if (zigbee_app_join_continue(APP_ZB_JOIN) == true)
        {
            info_color(LOG_RED, "Device do join\n");
            zigbee_app_evt_change(APP_NOT_JOINED_EVT, false);
        }
        else
        {
            info_color(LOG_YELLOW, "Device suspend join\n");
            bsp_led_Off(ZB_NWK_FINDING_INDICATOR);
        }
    }
    else
    {
        info_color(LOG_GREEN, "Device join success\n");
        info_color(LOG_GREEN, "PAN: %04X, ShortAddr: %04X, on channel: %02d, MAC: %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n",
                   pt_start_idc->panID, pt_start_idc->nwkAddr, pt_start_idc->channel,
                   pt_start_idc->ieee_addr[7], pt_start_idc->ieee_addr[6],
                   pt_start_idc->ieee_addr[5], pt_start_idc->ieee_addr[4],
                   pt_start_idc->ieee_addr[3], pt_start_idc->ieee_addr[2],
                   pt_start_idc->ieee_addr[1], pt_start_idc->ieee_addr[0]);
        zigbee_app_evt_change(APP_ZB_JOINED_EVT, false);
        zigbee_app_join_info_reset(false);
        //memcpy(long_addr, pt_start_idc->ieee_addr, 8);
    }
}

static void _zdo_evt_device_leave_idc(sys_tlv_t *pt_tlv)
{
    zigbee_zdo_device_leave_idc_t *pt_dev_leave;

    do
    {
        if (!pt_tlv)
        {
            break;
        }
        pt_dev_leave = (zigbee_zdo_device_leave_idc_t *)pt_tlv->value;
        info_color(LOG_RED, "Device Leave :\n");
        info_color(LOG_RED, "\tIEEE %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n",
                   pt_dev_leave->ieeeAddr[7], pt_dev_leave->ieeeAddr[6],
                   pt_dev_leave->ieeeAddr[5], pt_dev_leave->ieeeAddr[4],
                   pt_dev_leave->ieeeAddr[3], pt_dev_leave->ieeeAddr[2],
                   pt_dev_leave->ieeeAddr[1], pt_dev_leave->ieeeAddr[0]);
        info_color(LOG_RED, "\tShort address 0x%04x, rejoin %02X\n", pt_dev_leave->shortAddr, pt_dev_leave->rejoin);

        if (memcmp(pt_dev_leave->ieeeAddr, long_addr, 8) == 0)
        {
            __NVIC_SystemReset();
        }

    } while (0);
}

void zigbee_evt_handler(sys_tlv_t *pt_tlv)
{
    if ((pt_tlv->type >= ZIGBEE_EVT_TYPE_ZDO_START_IDC) &&
            (pt_tlv->type <= ZIGBEE_EVT_TYPE_ZDO_FINISH_IDC))
    {
        if (zdo_evt_idc_func_list[pt_tlv->type - ZIGBEE_EVT_TYPE_START_IDC])
        {
            zdo_evt_idc_func_list[pt_tlv->type - ZIGBEE_EVT_TYPE_START_IDC](pt_tlv);
        }
    }
}

