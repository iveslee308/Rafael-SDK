/******************************************************************************
*
* @File         ruci_pci_zwave_cmd.h
* @Version
* $Revision: 6351
* $Date: 2023-11-17
* @Brief
* @Note
* Copyright (C) 2023 Rafael Microelectronics Inc. All rights reserved.
*
******************************************************************************/
#ifndef _RUCI_PCI_ZWAVE_CMD_H
#define _RUCI_PCI_ZWAVE_CMD_H

#include "ruci_head.h"

#if (RUCI_ENABLE_PCI)

/******************************************************************************
* DEFINES
*****************************************************************************/

#pragma pack(push)
#pragma pack(1)
#define RUCI_PCI_ZWAVE_CMD_HEADER 0x18

// RUCI: initiate_zwave --------------------------------------------------------
#define RUCI_INITIATE_ZWAVE                     RUCI_NUM_INITIATE_ZWAVE, ruci_elmt_type_initiate_zwave, ruci_elmt_num_initiate_zwave
#define RUCI_CODE_INITIATE_ZWAVE                0x01
#define RUCI_LEN_INITIATE_ZWAVE                 4
#define RUCI_NUM_INITIATE_ZWAVE                 4
#define RUCI_PARA_LEN_INITIATE_ZWAVE            1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_initiate_zwave[];
extern const uint8_t ruci_elmt_num_initiate_zwave[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_initiate_zwave_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         band_type;
} ruci_para_initiate_zwave_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_initiate_zwave_t) */
#define SET_RUCI_PARA_INITIATE_ZWAVE(msg, band_type_in)        \
        do{                                                                                                            \
        ((ruci_para_initiate_zwave_t *)msg)->ruci_header.u8                 = RUCI_PCI_ZWAVE_CMD_HEADER;              \
        ((ruci_para_initiate_zwave_t *)msg)->sub_header                     = RUCI_CODE_INITIATE_ZWAVE;               \
        ((ruci_para_initiate_zwave_t *)msg)->length                         = RUCI_PARA_LEN_INITIATE_ZWAVE;           \
        ((ruci_para_initiate_zwave_t *)msg)->band_type                      = band_type_in;                           \
        }while(0)

// RUCI: set_zwave_modem -------------------------------------------------------
#define RUCI_SET_ZWAVE_MODEM                    RUCI_NUM_SET_ZWAVE_MODEM, ruci_elmt_type_set_zwave_modem, ruci_elmt_num_set_zwave_modem
#define RUCI_CODE_SET_ZWAVE_MODEM               0x02
#define RUCI_LEN_SET_ZWAVE_MODEM                4
#define RUCI_NUM_SET_ZWAVE_MODEM                4
#define RUCI_PARA_LEN_SET_ZWAVE_MODEM           1
#if (RUCI_ENDIAN_INVERSE)
extern const uint8_t ruci_elmt_type_set_zwave_modem[];
extern const uint8_t ruci_elmt_num_set_zwave_modem[];
#endif /* RUCI_ENDIAN_INVERSE */
typedef struct ruci_para_set_zwave_modem_s
{
    ruci_head_t     ruci_header;
    uint8_t         sub_header;
    uint8_t         length;
    uint8_t         bandwidth;
} ruci_para_set_zwave_modem_t;

/* User should provide msg buffer is greater than sizeof(ruci_para_set_zwave_modem_t) */
#define SET_RUCI_PARA_SET_ZWAVE_MODEM(msg, bandwidth_in)        \
        do{                                                                                                            \
        ((ruci_para_set_zwave_modem_t *)msg)->ruci_header.u8                 = RUCI_PCI_ZWAVE_CMD_HEADER;              \
        ((ruci_para_set_zwave_modem_t *)msg)->sub_header                     = RUCI_CODE_SET_ZWAVE_MODEM;              \
        ((ruci_para_set_zwave_modem_t *)msg)->length                         = RUCI_PARA_LEN_SET_ZWAVE_MODEM;          \
        ((ruci_para_set_zwave_modem_t *)msg)->bandwidth                      = bandwidth_in;                           \
        }while(0)

#pragma pack(pop)
#endif /* RUCI_ENABLE_PCI */
#endif /* _RUCI_PCI_ZWAVE_CMD_H */
