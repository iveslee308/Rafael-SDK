###########################################################################
#  !!! This file is Auto-Generated By Embedded IDE, Don't modify it !!!
###########################################################################

# eide version
EIDE_VER = 2

# current target
CUR_TARGET := RT58x

# current compiler
COMPILER_TYPE := GCC

# include folders
INCLUDE_FOLDERS += ../../../../../../../Library/CMSIS/Include
INCLUDE_FOLDERS += ../../../../../../../Library/RT58x/Peripheral/Include
INCLUDE_FOLDERS += ../../../../../../../Library/RT58x/PHY/include
INCLUDE_FOLDERS += ../../../../../../../Library/RT58x/include
INCLUDE_FOLDERS += ../../../../../../../Library/RT58x/PHY/rt569mp/include
INCLUDE_FOLDERS += ../../../../../../../Middleware/RF_FW_Control_Task/Include
INCLUDE_FOLDERS += ../../../../../../../Middleware/RUCI/include
INCLUDE_FOLDERS += ../../../../../../../Middleware/Third_Party/FreeRTOS/Source/include
INCLUDE_FOLDERS += ../../../../../../../Middleware/Portable/Utility/include
INCLUDE_FOLDERS += ../../../../../../../Middleware/Portable/System/Include
INCLUDE_FOLDERS += ../../../../../../../Middleware/Portable/bsp/Include
INCLUDE_FOLDERS += ../../../../../../../Middleware/MemoryMgmt/Include
INCLUDE_FOLDERS += ../../../../../../../Middleware/BLE/BLE_Host/Include
INCLUDE_FOLDERS += ../../../../../../../Middleware/FOTA/Include
INCLUDE_FOLDERS += ../../../../../../../Middleware/BLE/BLE_Service/BLE_Service_Common/Include
INCLUDE_FOLDERS += ../../../../../../../Middleware/BLE/BLE_Service/GAPS/Include
INCLUDE_FOLDERS += ../../../../../../../Middleware/BLE/BLE_Service/DIS/Include
INCLUDE_FOLDERS += ../../../../../../../Middleware/BLE/BLE_Service/GATTS/Include
INCLUDE_FOLDERS += ../../../../../../../Middleware/BLE/BLE_Service/HIDS/Include
INCLUDE_FOLDERS += ../../BLE_App_Profile/Include
INCLUDE_FOLDERS += ../../Config
INCLUDE_FOLDERS += ../../Include
INCLUDE_FOLDERS += ../../../../../../../Middleware/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3

# library search folders

# c source files
C_SOURCES += ../../../../../../../Library/RT58x/Device/system_cm3_mcu.c
C_SOURCES += ../../../../../../../Library/RT58x/PHY/rf_tx_comp.c
C_SOURCES += ../../../../../../../Library/RT58x/PHY/rt569mp/rt569mp_fw.c
C_SOURCES += ../../../../../../../Library/RT58x/PHY/rt569mp/rt569mp_init.c
C_SOURCES += ../../../../../../../Library/RT58x/Peripheral/dma.c
C_SOURCES += ../../../../../../../Library/RT58x/Peripheral/flashctl.c
C_SOURCES += ../../../../../../../Library/RT58x/Peripheral/gpio.c
C_SOURCES += ../../../../../../../Library/RT58x/Peripheral/lpm.c
C_SOURCES += ../../../../../../../Library/RT58x/Peripheral/mp_sector.c
C_SOURCES += ../../../../../../../Library/RT58x/Peripheral/sadc.c
C_SOURCES += ../../../../../../../Library/RT58x/Peripheral/sysctrl.c
C_SOURCES += ../../../../../../../Library/RT58x/Peripheral/sysfun.c
C_SOURCES += ../../../../../../../Library/RT58x/Peripheral/timer.c
C_SOURCES += ../../../../../../../Library/RT58x/Peripheral/uart_drv.c
C_SOURCES += ../../../../../../../Library/RT58x/Peripheral/uart_retarget.c
C_SOURCES += ../../../../../../../Middleware/BLE/BLE_Host/ble_cmd_advertising.c
C_SOURCES += ../../../../../../../Middleware/BLE/BLE_Host/ble_cmd_att_gatt.c
C_SOURCES += ../../../../../../../Middleware/BLE/BLE_Host/ble_cmd_common.c
C_SOURCES += ../../../../../../../Middleware/BLE/BLE_Host/ble_cmd_gap.c
C_SOURCES += ../../../../../../../Middleware/BLE/BLE_Host/ble_cmd_privacy.c
C_SOURCES += ../../../../../../../Middleware/BLE/BLE_Host/ble_cmd_scan.c
C_SOURCES += ../../../../../../../Middleware/BLE/BLE_Host/ble_cmd_security_manager.c
C_SOURCES += ../../../../../../../Middleware/BLE/BLE_Host/ble_host_ref.c
C_SOURCES += ../../../../../../../Middleware/BLE/BLE_Service/Ble_Service_Common/ble_service_common.c
C_SOURCES += ../../../../../../../Middleware/BLE/BLE_Service/DIS/ble_service_dis.c
C_SOURCES += ../../../../../../../Middleware/BLE/BLE_Service/GAPS/ble_service_gaps.c
C_SOURCES += ../../../../../../../Middleware/BLE/BLE_Service/GATTS/ble_service_gatts.c
C_SOURCES += ../../../../../../../Middleware/BLE/BLE_Service/HIDS/ble_service_hids.c
C_SOURCES += ../../../../../../../Middleware/MemoryMgmt/mem_mgmt.c
C_SOURCES += ../../../../../../../Middleware/Portable/System/sys_arch.c
C_SOURCES += ../../../../../../../Middleware/Portable/System/sys_memory.c
C_SOURCES += ../../../../../../../Middleware/Portable/System/sys_printf.c
C_SOURCES += ../../../../../../../Middleware/Portable/System/sys_taskdbg.c
C_SOURCES += ../../../../../../../Middleware/Portable/System/sys_timer.c
C_SOURCES += ../../../../../../../Middleware/Portable/Utility/shell.c
C_SOURCES += ../../../../../../../Middleware/Portable/Utility/util.c
C_SOURCES += ../../../../../../../Middleware/Portable/Utility/util_bda.c
C_SOURCES += ../../../../../../../Middleware/Portable/Utility/util_bstream.c
C_SOURCES += ../../../../../../../Middleware/Portable/Utility/util_fcs.c
C_SOURCES += ../../../../../../../Middleware/Portable/Utility/util_list.c
C_SOURCES += ../../../../../../../Middleware/Portable/Utility/util_log.c
C_SOURCES += ../../../../../../../Middleware/Portable/Utility/util_printf.c
C_SOURCES += ../../../../../../../Middleware/Portable/Utility/util_queue.c
C_SOURCES += ../../../../../../../Middleware/Portable/Utility/util_string.c
C_SOURCES += ../../../../../../../Middleware/Portable/bsp/bsp.c
C_SOURCES += ../../../../../../../Middleware/Portable/bsp/bsp_button.c
C_SOURCES += ../../../../../../../Middleware/Portable/bsp/bsp_console.c
C_SOURCES += ../../../../../../../Middleware/Portable/bsp/bsp_led.c
C_SOURCES += ../../../../../../../Middleware/Portable/bsp/bsp_uart.c
C_SOURCES += ../../../../../../../Middleware/Third_Party/FreeRTOS/Source/list.c
C_SOURCES += ../../../../../../../Middleware/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3/port.c
C_SOURCES += ../../../../../../../Middleware/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
C_SOURCES += ../../../../../../../Middleware/Third_Party/FreeRTOS/Source/queue.c
C_SOURCES += ../../../../../../../Middleware/Third_Party/FreeRTOS/Source/tasks.c
C_SOURCES += ../../../../../../../Middleware/Third_Party/FreeRTOS/Source/timers.c
C_SOURCES += ../../BLE_App_Profile/ble_profile_app.c
C_SOURCES += ../../BLE_App_Profile/ble_profile_def.c
C_SOURCES += ../../ble_app.c
C_SOURCES += ../../main.c

# cpp source files

# asm source files
ASM_SOURCES += ../../../../../../../Library/RT58x/Device/GCC/gcc_startup_cm3_mcu.S

# object files
OBJ_SOURCES += ../../../../../../../Middleware/Prebuild/lib_ble_hci_task.a
OBJ_SOURCES += ../../../../../../../Middleware/Prebuild/lib_ble_host.a
OBJ_SOURCES += ../../../../../../../Middleware/Prebuild/lib_rf_mcu.a

# macro defines
DEFINES += CHIP_TYPE=RT581
DEFINES += CHIP_VERSION=RT58X_MPB
DEFINES += RF_FW_INCLUDE_BLE=TRUE
DEFINES += RF_FW_INCLUDE_PCI=FALSE
DEFINES += RF_FW_INCLUDE_MULTI_2P4G=FALSE
DEFINES += SUPPORT_BLE