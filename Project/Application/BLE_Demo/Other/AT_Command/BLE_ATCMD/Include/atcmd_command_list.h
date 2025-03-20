#ifndef _CMD_LIST_H_
#define _CMD_LIST_H_

#include "atcmd_command.h"
#include "ble_gap.h"
#include "atcmd_helper.h"
#include "ble_advertising.h"
#include "ble_scan.h"
#include "ble_profile.h"
#include "ble_security_manager.h"

// PUBLIC FUNCTION DECLARE
void cmd_list_init(void);
bool cmd_assign(atcmd_item_t *item);
bool high_level_cmd_check(char *cmd_str);
void cmd_list_print(void);

// ALL COMMAND LIST

//Other
extern cmd_info_t reset;
extern cmd_info_t help;
extern cmd_info_t role;
extern cmd_info_t all_param;
extern cmd_info_t sleep;
extern cmd_info_t wake_up;
extern cmd_info_t accept_list_size;
extern cmd_info_t add_accept_list;
extern cmd_info_t clear_accept_list;
extern cmd_info_t remove_accept_list;

//Advertising
extern cmd_info_t enable_adv;
extern cmd_info_t disable_adv;
extern cmd_info_t adv_interval;
extern cmd_info_t adv_channel_map;
extern cmd_info_t adv_type;
extern cmd_info_t adv_data;
extern cmd_info_t scan_rsp;

//Connect
extern cmd_info_t disconnect;
extern cmd_info_t con_interval;
extern cmd_info_t con_latency;
extern cmd_info_t con_timeout;
extern cmd_info_t con_param;

//Create_Connect
extern cmd_info_t create_connect;
extern cmd_info_t cancel_create_connect;
extern cmd_info_t create_connect_interval;
extern cmd_info_t create_connect_latency;
extern cmd_info_t create_connect_timeout;

//Scanning
extern cmd_info_t enable_scan;
extern cmd_info_t disable_scan;
extern cmd_info_t scan_type;
extern cmd_info_t scan_interval;
extern cmd_info_t scan_window;

//Address
extern cmd_info_t dev_addr;
extern cmd_info_t dev_addr_type;
extern cmd_info_t default_addr;

//Master
extern cmd_info_t exchange_mtu_size;
extern cmd_info_t read;
extern cmd_info_t write;
extern cmd_info_t write_without_rsp;
extern cmd_info_t read_cccd;
extern cmd_info_t write_cccd;

//Connect_Other
extern cmd_info_t data_length;
extern cmd_info_t preferred_data_length;
extern cmd_info_t preferred_mtu_size;
extern cmd_info_t read_mtu_size;
extern cmd_info_t phy;
extern cmd_info_t read_phy;
extern cmd_info_t read_rssi;
extern cmd_info_t security_request;
extern cmd_info_t bonding_flag;
extern cmd_info_t restore_cccd;
extern cmd_info_t init_bonding_info;

//Slave
extern cmd_info_t gap_appearance;
extern cmd_info_t gap_device_name;
extern cmd_info_t gap_pre_con_param;
extern cmd_info_t notify;
extern cmd_info_t indicate;
extern cmd_info_t read_rsp;

#endif //_CMD_LIST_H_
