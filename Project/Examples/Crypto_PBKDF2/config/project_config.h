/**************************************************************************//**
 * @file     project_config.h
 * @version
 * @brief   define project config
 *
 * @copyright
 ******************************************************************************/

#include "chip_define.h"

#ifndef ___PROJECT_CONFIG_H__
#define ___PROJECT_CONFIG_H__

#define MODULE_ENABLE(module)     (module > 0)


#define SET_SYS_CLK    SYS_CLK_64MHZ

/*
 * If system support multi-tasking, some hardware need mutex to protect
 */
#define SUPPORT_MULTITASKING               0

#if (CHIP_VERSION >= RT58X_MPA)

#endif

/*enable this option, UART1 will support CTS/RTS pin for flow control*/
#define SUPPORT_UART1_FLOWCNTL             0


/*Support AES  */
#define CRYPTO_AES_ENABLE                  1

/*Support ECC SECP192R1/P192 curve */
#define CRYPTO_SECP192R1_ENABLE            1

/*Support ECC SECP256R1/P256 curve */
#define CRYPTO_SECP256R1_ENABLE            1

/*Support ECC SECT163R2/B163 curve */
#define CRYPTO_SECT163R2_ENABLE            1



#if (CHIP_VERSION >= RT58X_MPA)
/*C25519 only supported by MP IC*/
#define CRYPTO_C25519_ENABLE               1

#endif

#endif
