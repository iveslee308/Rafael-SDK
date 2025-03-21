#ifndef __MMDL_TIME_SRV_H__
#define __MMDL_TIME_SRV_H__

#ifdef __cplusplus
extern "C" {
#endif


extern void mmdl_time_sr_handler(mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model, uint8_t is_broadcast);
extern void mmdl_time_setup_sr_handler(mesh_app_mdl_evt_msg_idc_t *p_evt_msg, ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model, uint8_t is_broadcast);
extern void mmdl_generic_power_onoff_ex_cb(ble_mesh_element_param_t *p_current_element, ble_mesh_model_param_t *p_child_model, general_parameter_t p);
extern void mmdl_generic_power_onoff_extend_set(ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model, uint8_t onopowerup_state);
extern void mmdl_generic_power_onoff_publish_state(ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model);
extern void mmdl_time_sr_init(ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model);
extern void mmdl_time_UTC_set_user(rtc_time_t current_time, int8_t time_zone, ble_mesh_model_param_t *p_model);
extern void mmdl_time_TAI_to_UTC_update(ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model);
extern void mmdl_time_UTC_to_TAI_update(ble_mesh_element_param_t *p_element, ble_mesh_model_param_t *p_model);

#ifdef __cplusplus
};
#endif

#endif /* __MMDL_TIME_SRV_H__*/
