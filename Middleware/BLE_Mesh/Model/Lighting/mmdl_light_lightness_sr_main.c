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
#include "mmdl_light_lightness_common.h"
#include "mmdl_light_lightness_sr.h"
#include "mmdl_gen_level_sr.h"
#include "mmdl_gen_onoff_sr.h"
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
static void mmdl_light_lightness_get(mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model);
static void mmdl_light_lightness_set(uint32_t ack_flag, mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model);
static void mmdl_light_lightness_linear_get(mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model);
static void mmdl_light_lightness_linear_set(uint32_t ack_flag, mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model);
static void mmdl_light_lightness_last_get(mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model);
static void mmdl_light_lightness_default_get(mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model);
static void mmdl_light_lightness_default_set(uint32_t ack_flag, mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model);
static void mmdl_light_lightness_range_get(mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model);
static void mmdl_light_lightness_range_set(uint32_t ack_flag, mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model);
static void mmdl_light_lightness_send_status(uint16_t opcode, uint32_t paramter_size,  mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model);
static void mmdl_light_lightness_update_extend(ble_mesh_element_param_t *p_current_element, ble_mesh_model_param_t *p_parent_model, ble_mesh_model_param_t *p_current_child_model);


void mmdl_light_lightness_sr_init(ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model)
{

}

void mmdl_light_lightness_sr_handler(mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model, uint8_t is_broadcast)
{

    /* store the data in private variables */
    ttl = 4;

    /* process the message by different opcode */
    switch (LE2BE16(p_evt_msg->opcode))
    {
    case MMDL_LIGHT_LIGHTNESS_GET_OPCODE:
        mmdl_light_lightness_get(p_evt_msg, p_element, p_model);
        break;

    case MMDL_LIGHT_LIGHTNESS_SET_OPCODE:
        mmdl_light_lightness_set(TRUE, p_evt_msg, p_element, p_model);
        break;

    case MMDL_LIGHT_LIGHTNESS_SET_NO_ACK_OPCODE:
        mmdl_light_lightness_set(FALSE, p_evt_msg, p_element, p_model);
        break;

    case MMDL_LIGHT_LIGHTNESS_LINEAR_GET_OPCODE:
        mmdl_light_lightness_linear_get(p_evt_msg, p_element, p_model);
        break;

    case MMDL_LIGHT_LIGHTNESS_LINEAR_SET_OPCODE:
        mmdl_light_lightness_linear_set(TRUE, p_evt_msg, p_element, p_model);
        break;

    case MMDL_LIGHT_LIGHTNESS_LINEAR_SET_NO_ACK_OPCODE:
        mmdl_light_lightness_linear_set(FALSE, p_evt_msg, p_element, p_model);
        break;

    case MMDL_LIGHT_LIGHTNESS_LAST_GET_OPCODE:
        mmdl_light_lightness_last_get(p_evt_msg, p_element, p_model);
        break;

    case MMDL_LIGHT_LIGHTNESS_DEFAULT_GET_OPCODE:
        mmdl_light_lightness_default_get(p_evt_msg, p_element, p_model);
        break;

    case MMDL_LIGHT_LIGHTNESS_RANGE_GET_OPCODE:
        mmdl_light_lightness_range_get(p_evt_msg, p_element, p_model);
        break;

    case MMDL_LIGHT_LIGHTNESS_DEFAULT_SET_OPCODE:
        mmdl_light_lightness_default_set(TRUE, p_evt_msg, p_element, p_model);
        break;

    case MMDL_LIGHT_LIGHTNESS_DEFAULT_SET_NO_ACK_OPCODE:
        mmdl_light_lightness_default_set(FALSE, p_evt_msg, p_element, p_model);
        break;

    case MMDL_LIGHT_LIGHTNESS_RANGE_SET_OPCODE:
        mmdl_light_lightness_range_set(TRUE, p_evt_msg, p_element, p_model);
        break;

    case MMDL_LIGHT_LIGHTNESS_RANGE_SET_NO_ACK_OPCODE:
        mmdl_light_lightness_range_set(FALSE, p_evt_msg, p_element, p_model);
        break;
    }

}

void mmdl_light_lightness_ex_cb(ble_mesh_element_param_t *p_current_element, ble_mesh_model_param_t *p_child_model, general_parameter_t p)
{
    ble_mesh_model_param_t   *p_parent_model;
    light_lightness_state_t  *p_light_lightness_state;
    uint32_t                 state_changed = 0;

    do
    {
        /* search my parent model by model id */
        if (!search_model(p_current_element, MMDL_LIGHT_LIGHTNESS_SR_MDL_ID, &p_parent_model))
        {
            break;
        }
        p_light_lightness_state = (light_lightness_state_t *)p_parent_model->p_state_var;

        //process the message from different extend model by model_id
        switch (p_child_model->model_id)
        {
        case MMDL_GEN_ONOFF_SR_MDL_ID:
        {
            if (p.u8 == 0) //set device off, level -> 0
            {
                p_light_lightness_state->lightness_last = p_light_lightness_state->lightness_actual;
                p_light_lightness_state->lightness_actual = 0;
            }
            else //det device on, level -> last
            {
                p_light_lightness_state->lightness_actual = p_light_lightness_state->lightness_last;
                p_light_lightness_state->lightness_last = 0;
            }
        }
        break;

        case MMDL_GEN_LEVEL_SR_MDL_ID:
        {
            p_light_lightness_state->lightness_last = p_light_lightness_state->lightness_actual;
            p_light_lightness_state->lightness_actual = p.u16 + 32768;
        }
        break;
        }
        /* TODO: have to identify the state change situation */
        state_changed = 1;

        /* process all children models */
        if (state_changed)
        {
            mmdl_light_lightness_update_extend(p_current_element, p_parent_model, p_child_model);


            if (p_parent_model->is_extended)
            {
                general_parameter_t  call_back_param;
                // this model is extended, call upper model to process
                call_back_param.u16 = p_light_lightness_state->lightness_actual;
                ((ex_call_back)(p_parent_model->p_upper_call_back))(p_current_element, p_parent_model, call_back_param);
            }
            else if (p_parent_model->p_user_call_back != NULL)
            {
                ((model_lightness_user_call_back)(p_parent_model->p_user_call_back))(p_current_element->element_address, p_light_lightness_state->lightness_actual);

            }
            /* publish the state due to state changed */
            mmdl_light_lightness_publish_state(p_current_element, p_parent_model);

        }



    } while (0);
}


void mmdl_light_lightness_scene_set(ble_mesh_element_param_t *p_current_element)
{
    light_lightness_state_t  *p_light_lightness_state;
    ble_mesh_model_param_t   *p_child_model;
    ble_mesh_model_param_t   *p_parent_model;

    if (search_model(p_current_element, MMDL_LIGHT_LIGHTNESS_SR_MDL_ID, &p_parent_model))
    {
        p_light_lightness_state = (light_lightness_state_t *)p_parent_model->p_state_var;

        /* search for the knowing extended model from spec in ths element */
        if (search_model(p_current_element, MMDL_GEN_ONOFF_SR_MDL_ID, &p_child_model))
        {
            uint8_t onoff_state;

            onoff_state = (p_light_lightness_state->lightness_actual > 0) ? 1 : 0;
            mmdl_generic_onoff_extend_set(p_current_element, p_child_model, onoff_state);
        }

        if (search_model(p_current_element, MMDL_GEN_LEVEL_SR_MDL_ID, &p_child_model))
        {
            int16_t level_state;

            level_state = p_light_lightness_state->lightness_actual - 32768;
            mmdl_generic_level_extend_set(p_current_element, p_child_model, level_state);
        }
    }


}

static void mmdl_light_lightness_get(mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model)
{
    mmdl_light_lightness_send_status(MMDL_LIGHT_LIGHTNESS_STATUS_OPCODE,
                                     sizeof(light_lightness_status_msg_t),
                                     p_evt_msg, p_element, p_model);
}

static void mmdl_light_lightness_set(uint32_t ack_flag, mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model)
{
    light_lightness_state_t    *p_light_lightness_state = (light_lightness_state_t *)p_model->p_state_var;
    light_lightness_set_msg_t  *p_msg = (light_lightness_set_msg_t *)p_evt_msg->parameter;
    uint32_t                   state_changed = 0;

    /*TODO process the tid */

    if (p_light_lightness_state->lightness_actual != p_msg->lightness)
    {
        state_changed = 1;
        p_light_lightness_state->lightness_last = p_light_lightness_state->lightness_actual;
        p_light_lightness_state->lightness_actual = p_msg->lightness;
    }

    if (ack_flag)
    {
        mmdl_light_lightness_send_status(MMDL_LIGHT_LIGHTNESS_STATUS_OPCODE,
                                         sizeof(light_lightness_status_msg_t),
                                         p_evt_msg, p_element, p_model);
    }

    if (state_changed)
    {
        mmdl_light_lightness_update_extend(p_element, p_model, p_model);
    }


    if (state_changed && p_model->is_extended)
    {
        general_parameter_t  call_back_param;
        // this model is extended, call upper model to process
        call_back_param.u16 = p_light_lightness_state->lightness_actual;
        ((ex_call_back)(p_model->p_upper_call_back))(p_element, p_model, call_back_param);
    }
    else if (state_changed)
    {
        mmdl_light_lightness_publish_state(p_element, p_model);
    }
}

static void mmdl_light_lightness_linear_get(mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model)
{
    mmdl_light_lightness_send_status(MMDL_LIGHT_LIGHTNESS_LINEAR_STATUS_OPCODE,
                                     sizeof(light_lightness_linear_status_msg_t),
                                     p_evt_msg, p_element, p_model);

}

static void mmdl_light_lightness_linear_set(uint32_t ack_flag, mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model)
{
    light_lightness_state_t  *p_light_lightness_state = (light_lightness_state_t *)p_model->p_state_var;
    uint32_t                 state_changed = 0;

    p_light_lightness_state->lightness_last = p_light_lightness_state->lightness_actual;
    //    float linear_value = 2548;
    //    float actual = 65535 * sqrt(linear_value / 65535 );
    //    p_light_lightness_state->lightness_actual = (uint16_t)actual;
    if (ack_flag)
    {
        mmdl_light_lightness_send_status(MMDL_LIGHT_LIGHTNESS_LINEAR_STATUS_OPCODE,
                                         sizeof(light_lightness_linear_status_msg_t),
                                         p_evt_msg, p_element, p_model);
    }

    if (state_changed)
    {
        mmdl_light_lightness_update_extend(p_element, p_model, p_model);
    }

    if (state_changed && p_model->is_extended)
    {
        general_parameter_t  call_back_param;
        // this model is extended, call upper model to process
        call_back_param.u16 = p_light_lightness_state->lightness_target;
        ((ex_call_back)(p_model->p_upper_call_back))(p_element, p_model, call_back_param);
    }
    else if (state_changed)
    {
        mmdl_light_lightness_publish_state(p_element, p_model);
    }
}

static void mmdl_light_lightness_last_get(mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model)
{
    mmdl_light_lightness_send_status(MMDL_LIGHT_LIGHTNESS_LAST_STATUS_OPCODE,
                                     sizeof(light_lightness_last_status_msg_t),
                                     p_evt_msg, p_element, p_model);

}

static void mmdl_light_lightness_default_get(mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model)
{
    mmdl_light_lightness_send_status(MMDL_LIGHT_LIGHTNESS_DEFAULT_STATUS_OPCODE,
                                     sizeof(light_lightness_default_status_msg_t),
                                     p_evt_msg, p_element, p_model);

}

static void mmdl_light_lightness_default_set(uint32_t ack_flag, mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model)
{
    light_lightness_state_t               *p_light_lightness_state = (light_lightness_state_t *)p_model->p_state_var;
    light_lightness_default_status_msg_t  *p_light_lightness_default_status_msg = (light_lightness_default_status_msg_t *)p_evt_msg->parameter;

    p_light_lightness_state->lightness_default = p_light_lightness_default_status_msg->lightness;

    if (ack_flag)
    {
        mmdl_light_lightness_send_status(MMDL_LIGHT_LIGHTNESS_DEFAULT_STATUS_OPCODE,
                                         sizeof(light_lightness_default_status_msg_t),
                                         p_evt_msg, p_element, p_model);
    }
}

static void mmdl_light_lightness_range_get(mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model)
{
    mmdl_light_lightness_send_status(MMDL_LIGHT_LIGHTNESS_RANGE_STATUS_OPCODE,
                                     sizeof(light_lightness_range_status_msg_t),
                                     p_evt_msg, p_element, p_model);

}

static void mmdl_light_lightness_range_set(uint32_t ack_flag, mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model)
{
    light_lightness_state_t          *p_light_lightness_state = (light_lightness_state_t *)p_model->p_state_var;
    light_lightness_range_set_msg_t  *p_light_lightness_range_set_msg = (light_lightness_range_set_msg_t *)p_evt_msg->parameter;

    p_light_lightness_state->lightness_range_min = p_light_lightness_range_set_msg->range_min;
    p_light_lightness_state->lightness_range_max = p_light_lightness_range_set_msg->range_max;

    if (ack_flag)
    {
        mmdl_light_lightness_send_status(MMDL_LIGHT_LIGHTNESS_RANGE_STATUS_OPCODE,
                                         sizeof(light_lightness_range_status_msg_t),
                                         p_evt_msg, p_element, p_model);
    }
}

static void mmdl_light_lightness_send_status(uint16_t opcode, uint32_t paramter_size, mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model)
{
    mesh_app_mdl_evt_msg_req_t  *pt_req;
    mesh_tlv_t                  *pt_tlv;
    light_lightness_state_t     *p_light_lightness_state = (light_lightness_state_t *)p_model->p_state_var;


    do
    {

        pt_tlv = pvPortMalloc(sizeof(mesh_tlv_t) + sizeof(mesh_app_mdl_evt_msg_req_t) + paramter_size);
        if (pt_tlv == NULL)
        {
            break;
        }
        pt_tlv->type = TYPE_MESH_APP_MDL_EVT_MSG_REQ;
        pt_tlv->length = sizeof(mesh_app_mdl_evt_msg_req_t) + paramter_size;

        pt_req = (mesh_app_mdl_evt_msg_req_t *)pt_tlv->value;

        pt_req->dst_addr = p_evt_msg->src_addr;
        pt_req->src_addr = p_element->element_address;
        pt_req->appkey_index = p_evt_msg->appkey_index;
        pt_req->ttl = ttl;
        pt_req->parameter_len = paramter_size;

        switch (opcode)
        {
        case MMDL_LIGHT_LIGHTNESS_STATUS_OPCODE:
        {
            light_lightness_status_msg_t *p_msg = (light_lightness_status_msg_t *)pt_req->parameter;
            pt_req->opcode_invalid = 0;
            pt_req->opcode = BE2LE16(MMDL_LIGHT_LIGHTNESS_STATUS_OPCODE);
            p_msg->present_lightness = p_light_lightness_state->lightness_actual;
            p_msg->target_lightness = p_light_lightness_state->lightness_target;
            p_msg->remaining_time = 0;
        }
        break;

        case MMDL_LIGHT_LIGHTNESS_LINEAR_STATUS_OPCODE:
        {
            light_lightness_linear_status_msg_t *p_msg = (light_lightness_linear_status_msg_t *)pt_req->parameter;
            pt_req->opcode_invalid = 0;
            pt_req->opcode = BE2LE16(MMDL_LIGHT_LIGHTNESS_LINEAR_STATUS_OPCODE);
            p_msg->present_lightness = p_light_lightness_state->lightness_linear;
            p_msg->target_lightness = p_light_lightness_state->lightness_target;
            p_msg->remaining_time = 0;
        }
        break;

        case MMDL_LIGHT_LIGHTNESS_LAST_STATUS_OPCODE:
        {
            light_lightness_last_status_msg_t *p_msg = (light_lightness_last_status_msg_t *)pt_req->parameter;
            pt_req->opcode_invalid = 0;
            pt_req->opcode = BE2LE16(MMDL_LIGHT_LIGHTNESS_LAST_STATUS_OPCODE);
            p_msg->lightness = p_light_lightness_state->lightness_last;
        }
        break;

        case MMDL_LIGHT_LIGHTNESS_DEFAULT_STATUS_OPCODE:
        {
            light_lightness_default_status_msg_t *p_msg = (light_lightness_default_status_msg_t *)pt_req->parameter;
            pt_req->opcode_invalid = 0;
            pt_req->opcode = BE2LE16(MMDL_LIGHT_LIGHTNESS_DEFAULT_STATUS_OPCODE);
            p_msg->lightness = p_light_lightness_state->lightness_default;
        }
        break;

        case MMDL_LIGHT_LIGHTNESS_RANGE_STATUS_OPCODE:
        {
            light_lightness_range_status_msg_t *p_msg = (light_lightness_range_status_msg_t *)pt_req->parameter;
            pt_req->opcode_invalid = 0;
            pt_req->opcode = BE2LE16(MMDL_LIGHT_LIGHTNESS_RANGE_STATUS_OPCODE);
            p_msg->status_code = 0;
            p_msg->range_min = p_light_lightness_state->lightness_range_min;
            p_msg->range_max = p_light_lightness_state->lightness_range_max;
        }
        }

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

static void mmdl_light_lightness_update_extend(ble_mesh_element_param_t *p_current_element, ble_mesh_model_param_t *p_parent_model, ble_mesh_model_param_t *p_current_child_model)
{
    light_lightness_state_t  *p_light_lightness_state = (light_lightness_state_t *)p_parent_model->p_state_var;
    ble_mesh_model_param_t   *p_child_model;
    uint32_t                 model_id = p_current_child_model->model_id;

    /* search for the knowing extended model from spec in ths element */
    if (search_model(p_current_element, MMDL_GEN_ONOFF_SR_MDL_ID, &p_child_model) && model_id != MMDL_GEN_ONOFF_SR_MDL_ID)
    {
        uint8_t onoff_state;

        onoff_state = (p_light_lightness_state->lightness_actual > 0) ? 1 : 0;
        mmdl_generic_onoff_extend_set(p_current_element, p_child_model, onoff_state);
    }

    if (search_model(p_current_element, MMDL_GEN_LEVEL_SR_MDL_ID, &p_child_model) && model_id != MMDL_GEN_LEVEL_SR_MDL_ID)
    {
        int16_t level_state;

        level_state = p_light_lightness_state->lightness_actual - 32768;
        mmdl_generic_level_extend_set(p_current_element, p_child_model, level_state);
    }

}

void mmdl_light_lightness_extend_set(ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model, uint16_t lightness_level_state)
{
    light_lightness_state_t      *p_light_lightness_state = (light_lightness_state_t *)p_model->p_state_var;

    do
    {
        /* if state not changed, break it */
        p_light_lightness_state->lightness_actual = lightness_level_state;

        /* TODO: update linear level */

    } while (0);
}

void mmdl_light_lightness_publish_state(ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model)
{
    mesh_app_mdl_evt_msg_req_t  *pt_req;
    mesh_tlv_t                  *pt_tlv = NULL;
    light_lightness_state_t     *p_light_lightness_state = (light_lightness_state_t *)p_model->p_state_var;
    uint16_t                    element_address = p_element->element_address;
    uint16_t                    publish_address = p_model->p_publish_info->address;
    uint8_t                     ttl = p_model->p_publish_info->publish_ttl;
    uint32_t                    parameter_size = sizeof(light_lightness_linear_status_msg_t);

    do
    {
        if (publish_address == MESH_UNASSIGNED_ADDR)
        {
            /* the publish address is not valid, ignore command */
            break;
        }

        pt_tlv = pvPortMalloc(sizeof(mesh_tlv_t) + sizeof(mesh_app_mdl_evt_msg_req_t) + parameter_size);
        if (pt_tlv == NULL)
        {
            break;
        }
        pt_tlv->type = TYPE_MESH_APP_MDL_EVT_MSG_REQ;
        pt_tlv->length = sizeof(mesh_app_mdl_evt_msg_req_t) + parameter_size;

        pt_req = (mesh_app_mdl_evt_msg_req_t *)pt_tlv->value;

        pt_req->dst_addr = publish_address;
        pt_req->src_addr = element_address;
        pt_req->ttl = ttl;
        pt_req->parameter_len = parameter_size;

        light_lightness_status_msg_t *p_msg = (light_lightness_status_msg_t *)pt_req->parameter;
        pt_req->opcode_invalid = 0;
        pt_req->opcode = BE2LE16(MMDL_LIGHT_LIGHTNESS_STATUS_OPCODE);
        p_msg->present_lightness = p_light_lightness_state->lightness_actual;
        p_msg->target_lightness = p_light_lightness_state->lightness_target;
        p_msg->remaining_time = 0;

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

