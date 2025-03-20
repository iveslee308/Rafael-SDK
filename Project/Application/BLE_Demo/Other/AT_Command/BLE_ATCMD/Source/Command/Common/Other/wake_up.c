#include "atcmd_command_list.h"

// PRIVATE FUNCTION DECLARE
static void wake_up_init(cmd_info_t *this);
static ble_err_t set_cmd(atcmd_item_t *item);
static void test_cmd(atcmd_item_t *item);

// PUBLIC VARIABLE DECLARE
cmd_info_t wake_up =
{
    .cmd_name = "+WAKEUP",
    .description = "wake up",
    .init = wake_up_init
};

// PRIVATE FUNCTION IMPLEMENT
static void wake_up_init(cmd_info_t *this)
{
    cmd_info_init(this);
    this->set_cmd = set_cmd;
    this->test_cmd = test_cmd;
}

static ble_err_t set_cmd(atcmd_item_t *item)
{
    Lpm_Low_Power_Mask(LOW_POWER_MASK_BIT_TASK_BLE_APP);
    item->status = AT_CMD_STATUS_OK;
    return BLE_ERR_OK;
}

static void test_cmd(atcmd_item_t *item)
{
    printf(
        "+WAKEUP\n"
        "  Set the device to go into Idle mode when it is not busy.\n"
    );
}
