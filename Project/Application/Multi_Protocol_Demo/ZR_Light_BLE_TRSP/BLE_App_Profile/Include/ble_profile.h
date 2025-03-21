/**************************************************************************//**
 * @file  ble_profile.h
 * @brief Provide the declarations that for BLE Profile subsystem needed.
*****************************************************************************/

#ifndef _BLE_PROFILE_H_
#define _BLE_PROFILE_H_

#include "stdint.h"
#include "ble_common.h"
#include "ble_service_common.h"
#include "ble_service_dis.h"
#include "ble_service_gaps.h"
#include "ble_service_gatts.h"
#include "ble_service_trsps.h"

#include "ble_service_fotas.h"

/**************************************************************************
 * Profile Application GENERAL Public Definitions and Functions
 **************************************************************************/
/**
 * @details Here shows the general definitions of the application profile.
 *
**************************************************************************/
/** Define the maximum number of BLE GAPS link. */
#define MAX_NUM_CONN_GAPS      1

/** Define the maximum number of BLE GATTS link. */
#define MAX_NUM_CONN_GATTS     1

/** Define the maximum number of BLE DIS link. */
#define MAX_NUM_CONN_DIS       1

/** Define the maximum number of BLE TRSPS link. */
#define MAX_NUM_CONN_TRSPS     1

/** Define the maximum number of BLE FOTAS link. */
#define MAX_NUM_CONN_FOTAS     1

/**************************************************************************
 * Profile Application LINK Public Definitions and Functions
 **************************************************************************/

/** BLE Application Link 0 Profile Attribute Information Structure.*/
typedef struct ble_info_link0_s
{
    ble_svcs_gaps_info_t      svcs_info_gaps;      /**< GAPS information (server). */
    ble_svcs_gatts_info_t     svcs_info_gatts;     /**< GATTS information (server). */
    ble_svcs_dis_info_t       svcs_info_dis;       /**< DIS information (server). */
    ble_svcs_trsps_info_t     svcs_info_trsps;     /**< TRSPS information (server). */
    ble_svcs_fotas_info_t     svcs_info_fotas;     /**< FOTAS information (server). */
} ble_info_link0_t;

typedef struct ble_app_link_info_s
{
    ble_gap_role_t   gap_role;    /**< GAP role. */
    uint8_t          state;       /**< Current state. */
    void             *profile_info;  /**< Link profile information. */
} ble_app_link_info_t;


/**************************************************************************
 * Global Functions
 **************************************************************************/


/**************************************************************************
 * Extern Definitions
 **************************************************************************/

/** Extern maximum Number of Host Connection Link Definition. */
extern const uint8_t                              max_num_conn_host;

/** Extern BLE Connection Links Definition. */
extern const ble_att_role_by_id_t                 att_db_link[];

/** Extern BLE Connection Link Parameter Table Definition. */
extern const ble_att_db_mapping_by_id_t           att_db_mapping[];

/** Extern BLE Connection Link Mapping Size Definition. */
extern const ble_att_db_mapping_by_id_size_t      att_db_mapping_size[];

/** Extern BLE application links information which is ordered by host id. */
extern ble_app_link_info_t                        ble_app_link_info[];

#endif //_BLE_PROFILE_H_

