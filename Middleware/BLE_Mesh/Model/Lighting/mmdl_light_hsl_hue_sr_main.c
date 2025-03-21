/**
 * @file mmdl_light_control_sr_main.c
 * @author Joshua Jean (joshua.jean@rafaelmicro.com)
 * @brief
 * @version 0.1
 * @date 2022-07-13
 *
 * @copyright Copyright (c) 2022
 *
 */

//=============================================================================
//                Include
//=============================================================================
#include <math.h>
#include "sys_arch.h"
#include "mmdl_common.h"
#include "mmdl_light_hsl_common.h"
#include "mmdl_light_hsl_sr.h"
#include "mmdl_gen_level_sr.h"

//=============================================================================
//                Private Definitions of const value
//=============================================================================



//=============================================================================
//                Private Global Variables
//=============================================================================

/* store the useful data pointer for futher using */
static uint8_t                      ttl = 4;

//=============================================================================
//                Private Functions Declaration
//=============================================================================
static void mmdl_light_hsl_hue_get(mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model);
static void mmdl_light_hsl_hue_set(uint32_t ack_flag, mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model);
static void mmdl_light_hsl_hue_send_status(mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model);
static void mmdl_light_hsl_hue_update_extend(ble_mesh_element_param_t *p_current_element, ble_mesh_model_param_t *p_parent_model, ble_mesh_model_param_t *p_current_child_model);

void mmdl_light_hsl_hue_sr_init(ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model)
{

}

void mmdl_light_hsl_hue_sr_handler(mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model, uint8_t is_broadcast)
{

    /* store the data in private variables */
    ttl = 4;

    /* process the message by different opcode */
    switch (LE2BE16(p_evt_msg->opcode))
    {
    case MMDL_LIGHT_HSL_HUE_GET_OPCODE:
        mmdl_light_hsl_hue_get(p_evt_msg, p_element, p_model);
        break;

    case MMDL_LIGHT_HSL_HUE_SET_OPCODE:
        mmdl_light_hsl_hue_set(TRUE, p_evt_msg, p_element, p_model);
        break;

    case MMDL_LIGHT_HSL_HUE_SET_NO_ACK_OPCODE:
        mmdl_light_hsl_hue_set(FALSE, p_evt_msg, p_element, p_model);
        break;

    }
}

void mmdl_light_hsl_hue_ex_cb(ble_mesh_element_param_t *p_current_element, ble_mesh_model_param_t *p_child_model, general_parameter_t p)
{
    ble_mesh_model_param_t  *p_parent_model;
    light_hsl_state_t       *p_light_hsl_state;
    uint32_t                state_changed = 0;

    do
    {
        /* search my parent model by model id */
        if (!search_model(p_current_element, MMDL_LIGHT_HSL_HUE_SR_MDL_ID, &p_parent_model))
        {
            break;
        }

        p_light_hsl_state = (light_hsl_state_t *)p_parent_model->p_state_var;

        //process the message from different extend model by model_id
        switch (p_child_model->model_id)
        {
        case MMDL_GEN_LEVEL_SR_MDL_ID:
        {
            p_light_hsl_state->target_hsl_hue_state = p.u16 + 32768;
        }
        break;
        }

        state_changed = 1;

        /* process all children models */
        /* according to the spec, this model has only one extended model "generic level" */
        mmdl_light_hsl_hue_update_extend(p_current_element, p_parent_model, p_child_model);

        if (state_changed)
        {
            /* publish the state due to state changed */
            mmdl_light_hsl_hue_publish_state(p_current_element, p_parent_model);
        }

    } while (0);
}

void mmdl_light_hsl_hue_scene_set(ble_mesh_element_param_t *p_current_element)
{
    light_hsl_state_t       *p_light_hsl_state;
    ble_mesh_model_param_t  *p_child_model;
    ble_mesh_model_param_t  *p_parent_model;

    /* search for the knowing extended model from spec in ths element */

    if (search_model(p_current_element, MMDL_LIGHT_HSL_HUE_SR_MDL_ID, &p_parent_model))
    {
        p_light_hsl_state = (light_hsl_state_t *)p_parent_model->p_state_var;
        if (search_model(p_current_element, MMDL_GEN_LEVEL_SR_MDL_ID, &p_child_model))
        {
            int16_t level_state;

            level_state = p_light_hsl_state->hsl_hue_state - 32768;
            mmdl_generic_level_extend_set(p_current_element, p_child_model, level_state);
        }
    }
}

static void mmdl_light_hsl_hue_get(mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model)
{
    mmdl_light_hsl_hue_send_status(p_evt_msg, p_element, p_model);
}

static void mmdl_light_hsl_hue_set(uint32_t ack_flag, mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model)
{
    light_hsl_hue_set_msg_t  *p_msg = (light_hsl_hue_set_msg_t *)p_evt_msg->parameter;
    light_hsl_state_t        *p_light_hsl_state = (light_hsl_state_t *)p_model->p_state_var;
    uint32_t                 state_changed = 0;

    do
    {
        //check the state change
        if (p_light_hsl_state->hsl_hue_state == p_msg->hue)
        {
            break;
        }

        //if state changed, check is the extended or not
        p_light_hsl_state->hsl_hue_state = p_msg->hue;
        p_light_hsl_state->target_hsl_hue_state = p_msg->hue;

        state_changed = 1;

    } while (0);

    if (ack_flag)
    {
        mmdl_light_hsl_hue_send_status(p_evt_msg, p_element, p_model);
    }

    /* publish the state if publish is avaliable */
    if (state_changed)
    {
        mmdl_light_hsl_hue_update_extend(p_element, p_model, p_model);
        mmdl_light_hsl_hue_publish_state(p_element, p_model);
    }

}


static void mmdl_light_hsl_hue_send_status(mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model)
{
    mesh_app_mdl_evt_msg_req_t  *pt_req;
    mesh_tlv_t                  *pt_tlv;
    light_hsl_state_t           *p_light_hsl_state   = (light_hsl_state_t *)p_model->p_state_var;
    light_hsl_hue_status_msg_t  *p_light_hsl_hue_status;


    do
    {
        pt_tlv = pvPortMalloc(sizeof(mesh_tlv_t) + sizeof(mesh_app_mdl_evt_msg_req_t) + sizeof(light_hsl_hue_status_msg_t));
        if (pt_tlv == NULL)
        {
            break;
        }

        pt_tlv->type = TYPE_MESH_APP_MDL_EVT_MSG_REQ;
        pt_tlv->length = sizeof(mesh_app_mdl_evt_msg_req_t) + sizeof(light_hsl_hue_status_msg_t);

        pt_req = (mesh_app_mdl_evt_msg_req_t *)pt_tlv->value;

        pt_req->dst_addr = p_evt_msg->src_addr;
        pt_req->src_addr = p_element->element_address;
        pt_req->appkey_index = p_evt_msg->appkey_index;
        pt_req->ttl = ttl;

        pt_req->opcode_invalid = 0;
        pt_req->opcode = BE2LE16(MMDL_LIGHT_HSL_HUE_STATUS_OPCODE);
        pt_req->parameter_len = sizeof(light_hsl_hue_status_msg_t);
        p_light_hsl_hue_status = (light_hsl_hue_status_msg_t *)pt_req->parameter;
        p_light_hsl_hue_status->present_hue = p_light_hsl_state->hsl_hue_state;
        p_light_hsl_hue_status->target_hue = p_light_hsl_state->target_hsl_hue_state;
        /* TODO: remaining time must be processed */
        p_light_hsl_hue_status->remaining_time = 0;

        if (mesh_evt_msg_sendto(pt_tlv) != 0)
        {
            info_color(LOG_RED, "Send msg to mesh stack fail\n");
            break;
        }
    } while (0);

    if (pt_tlv)
    {
        vPortFree(pt_tlv);
    }

}


static void mmdl_light_hsl_hue_update_extend(ble_mesh_element_param_t *p_current_element, ble_mesh_model_param_t *p_parent_model, ble_mesh_model_param_t *p_current_child_model)
{
    light_hsl_state_t       *p_light_hsl_state = (light_hsl_state_t *)p_parent_model->p_state_var;
    ble_mesh_model_param_t  *p_child_model;
    uint32_t                 model_id = p_current_child_model->model_id;

    /* search for the knowing extended model from spec in ths element */

    if (search_model(p_current_element, MMDL_GEN_LEVEL_SR_MDL_ID, &p_child_model) && model_id != MMDL_GEN_LEVEL_SR_MDL_ID)
    {
        int16_t level_state;

        level_state = p_light_hsl_state->hsl_hue_state - 32768;
        mmdl_generic_level_extend_set(p_current_element, p_child_model, level_state);
    }

}

void mmdl_light_hsl_hue_publish_state(ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model)
{
    mesh_app_mdl_evt_msg_req_t  *pt_req;
    mesh_tlv_t                  *pt_tlv = NULL;;
    light_hsl_state_t           *p_light_hsl_state   = (light_hsl_state_t *)p_model->p_state_var;
    uint16_t                    element_address = p_element->element_address;
    uint16_t                    publish_address = p_model->p_publish_info->address;
    uint8_t                     ttl = p_model->p_publish_info->publish_ttl;
    light_hsl_hue_status_msg_t  *p_light_hsl_hue_status;

    do
    {
        if (publish_address == MESH_UNASSIGNED_ADDR)
        {
            /* the publish address is not valid, ignore command */
            break;
        }

        pt_tlv = pvPortMalloc(sizeof(mesh_tlv_t) + sizeof(mesh_app_mdl_evt_msg_req_t) + sizeof(light_hsl_hue_status_msg_t));
        if (pt_tlv == NULL)
        {
            break;
        }

        pt_tlv->type = TYPE_MESH_APP_MDL_EVT_MSG_REQ;
        pt_tlv->length = sizeof(mesh_app_mdl_evt_msg_req_t) + sizeof(light_hsl_hue_status_msg_t);

        pt_req = (mesh_app_mdl_evt_msg_req_t *)pt_tlv->value;

        pt_req->dst_addr = publish_address;
        pt_req->src_addr = element_address;
        pt_req->ttl = ttl;

        pt_req->opcode_invalid = 0;
        pt_req->opcode = BE2LE16(MMDL_LIGHT_HSL_HUE_STATUS_OPCODE);
        pt_req->parameter_len = sizeof(light_hsl_hue_status_msg_t);
        p_light_hsl_hue_status = (light_hsl_hue_status_msg_t *)pt_req->parameter;
        p_light_hsl_hue_status->present_hue = p_light_hsl_state->hsl_hue_state;
        p_light_hsl_hue_status->target_hue = p_light_hsl_state->target_hsl_hue_state;
        /* TODO: remaining time must be processed */
        p_light_hsl_hue_status->remaining_time = 0;

        if (mesh_evt_msg_sendto(pt_tlv) != 0)
        {
            info_color(LOG_RED, "Send msg to mesh stack fail\n");
            break;
        }
    } while (0);

    if (pt_tlv)
    {
        vPortFree(pt_tlv);
    }

}
