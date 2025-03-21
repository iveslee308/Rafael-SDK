###########################################################################
#  !!! This file is Auto-Generated By Embedded IDE, Don't modify it !!!
###########################################################################

# eide version
EIDE_VER = 2

# current target
CUR_TARGET := Target 1

# current compiler
COMPILER_TYPE := GCC

# include folders
INCLUDE_FOLDERS += ../config
INCLUDE_FOLDERS += ../../../../Library/CMSIS/Include
INCLUDE_FOLDERS += ../../../../Library/RT58x/Peripheral/Include
INCLUDE_FOLDERS += ../../../../Library/RT58x/PHY/include
INCLUDE_FOLDERS += ../../../../Library/RT58x/include
INCLUDE_FOLDERS += .eide/deps

# library search folders

# c source files
C_SOURCES += ../../../../Library/RT58x/Device/system_cm3_mcu.c
C_SOURCES += ../../../../Library/RT58x/Peripheral/dma.c
C_SOURCES += ../../../../Library/RT58x/Peripheral/flashctl.c
C_SOURCES += ../../../../Library/RT58x/Peripheral/gpio.c
C_SOURCES += ../../../../Library/RT58x/Peripheral/mp_sector.c
C_SOURCES += ../../../../Library/RT58x/Peripheral/qspi.c
C_SOURCES += ../../../../Library/RT58x/Peripheral/retarget_drv_keil.c
C_SOURCES += ../../../../Library/RT58x/Peripheral/rtc.c
C_SOURCES += ../../../../Library/RT58x/Peripheral/sysctrl.c
C_SOURCES += ../../../../Library/RT58x/Peripheral/sysfun.c
C_SOURCES += ../../../../Library/RT58x/Peripheral/timer.c
C_SOURCES += ../../../../Library/RT58x/Peripheral/uart_drv.c
C_SOURCES += ../main.c

# cpp source files

# asm source files
ASM_SOURCES += ../../../../Library/RT58x/Device/GCC/gcc_startup_cm3_mcu.S

# object files
OBJ_SOURCES += ../../../../Middleware/Prebuild/lib_rf_mcu.a

# macro defines
DEFINES += DEBUG