
#include <stdio.h>
#include <string.h>
#include "cm3_mcu.h"

#include "project_config.h"

#include "uart_drv.h"
#include "retarget.h"

#include "crypto.h"
#include "sha256.h"
#include "rf_mcu_ahb.h"

int main(void);

void SetClockFreq(void);

/*
 * Remark: UART_BAUDRATE_115200 is not 115200...Please don't use 115200 directly
 * Please use macro define  UART_BAUDRATE_XXXXXX
 */

#define PRINTF_BAUDRATE      UART_BAUDRATE_115200

#define GPIO20  20
#define GPIO21  21

#define SUBSYSTEM_CFG_PMU_MODE              0x4B0
#define SUBSYSTEM_CFG_LDO_MODE_DISABLE      0x02
/************************************************************/

/*this is pin mux setting*/
void init_default_pin_mux(void)
{
    /*uart0 pinmux*/
    pin_set_mode(16, MODE_UART);     /*GPIO16 as UART0 TX*/
    pin_set_mode(17, MODE_UART);     /*GPIO17 as UART0 RX*/

    return;
}

void Comm_Subsystem_Disable_LDO_Mode(void)
{
    uint8_t reg_buf[4];

    RfMcu_MemoryGetAhb(SUBSYSTEM_CFG_PMU_MODE, reg_buf, 4);
    reg_buf[0] &= ~SUBSYSTEM_CFG_LDO_MODE_DISABLE;
    RfMcu_MemorySetAhb(SUBSYSTEM_CFG_PMU_MODE, reg_buf, 4);
}

void test_hmac_case1(void)
{
    const uint8_t key[20] =
    {
        0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
        0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b
    };

    const uint8_t data[8] =
    {
        0x48, 0x69, 0x20, 0x54, 0x68, 0x65, 0x72, 0x65
    };

    const uint8_t expected[32] =
    {
        0xb0, 0x34, 0x4c, 0x61, 0xd8, 0xdb, 0x38, 0x53, 0x5c, 0xa8, 0xaf, 0xce,
        0xaf, 0x0b, 0xf1, 0x2b, 0x88, 0x1d, 0xc2, 0x00, 0xc9, 0x83, 0x3d, 0xa7,
        0x26, 0xe9, 0x37, 0x6c, 0x2e, 0x32, 0xcf, 0xf7
    };

    uint8_t   output[32];

    hmac_sha256(key, sizeof(key), data, sizeof(data), output);

    if (memcmp(expected, output, SHA256_DIGEST_SIZE) != 0)
    {
        printf("Ooops!! check case1 \n");
    }
    else
    {
        printf("hmac case1 ok \n");
    }

    return;
}

void test_hmac_case2(void)
{
    const uint8_t key[4] =
    {
        0x4a, 0x65, 0x66, 0x65
    };

    const uint8_t data[28] =
    {
        0x77, 0x68, 0x61, 0x74, 0x20, 0x64, 0x6f, 0x20, 0x79, 0x61, 0x20, 0x77,
        0x61, 0x6e, 0x74, 0x20, 0x66, 0x6f, 0x72, 0x20, 0x6e, 0x6f, 0x74, 0x68,
        0x69, 0x6e, 0x67, 0x3f
    };

    const uint8_t expected[32] =
    {
        0x5b, 0xdc, 0xc1, 0x46, 0xbf, 0x60, 0x75, 0x4e, 0x6a, 0x04, 0x24, 0x26,
        0x08, 0x95, 0x75, 0xc7, 0x5a, 0x00, 0x3f, 0x08, 0x9d, 0x27, 0x39, 0x83,
        0x9d, 0xec, 0x58, 0xb9, 0x64, 0xec, 0x38, 0x43
    };

    uint8_t   output[32];

    hmac_sha256(key, sizeof(key), data, sizeof(data), output);

    if (memcmp(expected, output, SHA256_DIGEST_SIZE) != 0)
    {
        printf("Ooops!! check case2 \n");
    }
    else
    {
        printf("hmac case2 ok \n");
    }

    return;
}

void test_hmac_case3(void)
{
    const uint8_t key[20] =
    {
        0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
        0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa
    };

    const uint8_t data[50] =
    {
        0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
        0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
        0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
        0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
        0xdd, 0xdd
    };

    const uint8_t expected[32] =
    {
        0x77, 0x3e, 0xa9, 0x1e, 0x36, 0x80, 0x0e, 0x46, 0x85, 0x4d, 0xb8, 0xeb,
        0xd0, 0x91, 0x81, 0xa7, 0x29, 0x59, 0x09, 0x8b, 0x3e, 0xf8, 0xc1, 0x22,
        0xd9, 0x63, 0x55, 0x14, 0xce, 0xd5, 0x65, 0xfe
    };

    uint8_t   output[32];

    hmac_sha256(key, sizeof(key), data, sizeof(data), output);

    if (memcmp(expected, output, SHA256_DIGEST_SIZE) != 0)
    {
        printf("Ooops!! check case3\n");
    }
    else
    {
        printf("hmac case3 ok \n");
    }

    return;
}

void test_hmac_case4(void)
{
    const uint8_t key[25] =
    {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c,
        0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
        0x19
    };

    const uint8_t data[50] =
    {
        0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd,
        0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd,
        0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd,
        0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd,
        0xcd, 0xcd
    };

    const uint8_t expected[32] =
    {
        0x82, 0x55, 0x8a, 0x38, 0x9a, 0x44, 0x3c, 0x0e, 0xa4, 0xcc, 0x81, 0x98,
        0x99, 0xf2, 0x08, 0x3a, 0x85, 0xf0, 0xfa, 0xa3, 0xe5, 0x78, 0xf8, 0x07,
        0x7a, 0x2e, 0x3f, 0xf4, 0x67, 0x29, 0x66, 0x5b
    };

    uint8_t   output[32];

    hmac_sha256(key, sizeof(key), data, sizeof(data), output);

    if (memcmp(expected, output, SHA256_DIGEST_SIZE) != 0)
    {
        printf("Ooops!! check case4\n");
    }
    else
    {
        printf("hmac case4 ok \n");
    }

    return;
}

void test_hmac_case5(void)
{
    const uint8_t key[20] =
    {
        0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
        0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c
    };

    const uint8_t data[20] =
    {
        0x54, 0x65, 0x73, 0x74, 0x20, 0x57, 0x69, 0x74, 0x68, 0x20, 0x54, 0x72,
        0x75, 0x6e, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e
    };

    const uint8_t expected[32] =
    {
        0xa3, 0xb6, 0x16, 0x74, 0x73, 0x10, 0x0e, 0xe0, 0x6e, 0x0c, 0x79, 0x6c,
        0x29, 0x55, 0x55, 0x2b, 0xfa, 0x6f, 0x7c, 0x0a, 0x6a, 0x8a, 0xef, 0x8b,
        0x93, 0xf8, 0x60, 0xaa, 0xb0, 0xcd, 0x20, 0xc5
    };

    uint8_t   output[32];

    hmac_sha256(key, sizeof(key), data, sizeof(data), output);

    if (memcmp(expected, output, SHA256_DIGEST_SIZE) != 0)
    {
        printf("Ooops!! check case5\n");
    }
    else
    {
        printf("hmac case5 ok \n");
    }

    return;
}

/*
 * In RFC4231 test case 6 / case 7, key is 131 bytes, we "declare"
 * 132 byte for 4-bytes alignament space, but we will not use the last byte.
 */
const uint8_t case_6_7_key[132] =
{
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa
};

void test_hmac_case6(void)
{
    const uint8_t data[54] =
    {
        0x54, 0x65, 0x73, 0x74, 0x20, 0x55, 0x73, 0x69, 0x6e, 0x67, 0x20, 0x4c,
        0x61, 0x72, 0x67, 0x65, 0x72, 0x20, 0x54, 0x68, 0x61, 0x6e, 0x20, 0x42,
        0x6c, 0x6f, 0x63, 0x6b, 0x2d, 0x53, 0x69, 0x7a, 0x65, 0x20, 0x4b, 0x65,
        0x79, 0x20, 0x2d, 0x20, 0x48, 0x61, 0x73, 0x68, 0x20, 0x4b, 0x65, 0x79,
        0x20, 0x46, 0x69, 0x72, 0x73, 0x74
    };

    const uint8_t expected[32] =
    {
        0x60, 0xe4, 0x31, 0x59, 0x1e, 0xe0, 0xb6, 0x7f, 0x0d, 0x8a, 0x26, 0xaa,
        0xcb, 0xf5, 0xb7, 0x7f, 0x8e, 0x0b, 0xc6, 0x21, 0x37, 0x28, 0xc5, 0x14,
        0x05, 0x46, 0x04, 0x0f, 0x0e, 0xe3, 0x7f, 0x54
    };

    uint8_t   output[32];

    hmac_sha256(case_6_7_key, 131, data, sizeof(data), output);

    if (memcmp(expected, output, SHA256_DIGEST_SIZE) != 0)
    {
        printf("Ooops!! check case6\n");
    }
    else
    {
        printf("hmac case6 ok \n");
    }

    return;
}

const uint8_t case7_data[152] =
{
    0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x61, 0x20, 0x74, 0x65,
    0x73, 0x74, 0x20, 0x75, 0x73, 0x69, 0x6e, 0x67, 0x20, 0x61, 0x20, 0x6c,
    0x61, 0x72, 0x67, 0x65, 0x72, 0x20, 0x74, 0x68, 0x61, 0x6e, 0x20, 0x62,
    0x6c, 0x6f, 0x63, 0x6b, 0x2d, 0x73, 0x69, 0x7a, 0x65, 0x20, 0x6b, 0x65,
    0x79, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x61, 0x20, 0x6c, 0x61, 0x72, 0x67,
    0x65, 0x72, 0x20, 0x74, 0x68, 0x61, 0x6e, 0x20, 0x62, 0x6c, 0x6f, 0x63,
    0x6b, 0x2d, 0x73, 0x69, 0x7a, 0x65, 0x20, 0x64, 0x61, 0x74, 0x61, 0x2e,
    0x20, 0x54, 0x68, 0x65, 0x20, 0x6b, 0x65, 0x79, 0x20, 0x6e, 0x65, 0x65,
    0x64, 0x73, 0x20, 0x74, 0x6f, 0x20, 0x62, 0x65, 0x20, 0x68, 0x61, 0x73,
    0x68, 0x65, 0x64, 0x20, 0x62, 0x65, 0x66, 0x6f, 0x72, 0x65, 0x20, 0x62,
    0x65, 0x69, 0x6e, 0x67, 0x20, 0x75, 0x73, 0x65, 0x64, 0x20, 0x62, 0x79,
    0x20, 0x74, 0x68, 0x65, 0x20, 0x48, 0x4d, 0x41, 0x43, 0x20, 0x61, 0x6c,
    0x67, 0x6f, 0x72, 0x69, 0x74, 0x68, 0x6d, 0x2e
};

void test_hmac_case7(void)
{

    const uint8_t expected[32] =
    {
        0x9b, 0x09, 0xff, 0xa7, 0x1b, 0x94, 0x2f, 0xcb, 0x27, 0x63, 0x5f, 0xbc,
        0xd5, 0xb0, 0xe9, 0x44, 0xbf, 0xdc, 0x63, 0x64, 0x4f, 0x07, 0x13, 0x93,
        0x8a, 0x7f, 0x51, 0x53, 0x5c, 0x3a, 0x35, 0xe2
    };

    uint8_t   output[32];

    hmac_sha256(case_6_7_key, 131, case7_data, 152, output);

    if (memcmp(expected, output, SHA256_DIGEST_SIZE) != 0)
    {
        printf("Ooops!! check case7\n");
    }
    else
    {
        printf("hmac case7 ok \n");
    }

    return;

}

void test_hkdf_case1(void)
{
    /*RFC 5869 Test Case 1*/
    const uint8_t ikm[22] =
    {
        0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
        0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
        0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b
    };

    const uint8_t salt[13] =
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c
    };

    const uint8_t info[10] =
    {
        0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
        0xf8, 0xf9
    };

    const uint8_t expected_okm[42] =
    {
        0x3c, 0xb2, 0x5f, 0x25, 0xfa, 0xac, 0xd5, 0x7a,
        0x90, 0x43, 0x4f, 0x64, 0xd0, 0x36, 0x2f, 0x2a,
        0x2d, 0x2d, 0x0a, 0x90, 0xcf, 0x1a, 0x5a, 0x4c,
        0x5d, 0xb0, 0x2d, 0x56, 0xec, 0xc4, 0xc5, 0xbf,
        0x34, 0x00, 0x72, 0x08, 0xd5, 0xb8, 0x87, 0x18,
        0x58, 0x65
    };

    uint8_t  out_buf[64];      /*just allocate some large buffer... in fact, it only used 42 bytes*/
    uint32_t  out_length;
    uint32_t  status;

    out_length = 42;

    status = hkdf_sha256(out_buf, out_length,
                         ikm,  sizeof(ikm),  salt, sizeof(salt), info, sizeof(info));

    if (status != STATUS_SUCCESS)
    {
        /*almost impossible to here.*/
        printf("Oops... check erorr \n");
        while (1);
    }

    /*compare result*/

    if (memcmp( (void *)expected_okm, (void *) out_buf, out_length) == 0)
    {
        printf("Test HKDF RFC 5869 Test Case 1 OK \n");
    }
    else
    {
        /*almost impossible*/
        printf("Oops ... Test HKDF RFC 5869 Test Case 1 Error... Check Why\n");
        while (1);
    }
}

#define   TestCase2_ikm_length     80
#define   TestCase2_salt_length    80
#define   TestCase2_info_length    80

void test_hkdf_case2(void)
{

    /*
     *    For Keil V5.36.0 it will generate a strang warning L6096W
     * "...... is not null-terminated"
     *  To avoid this wanring, we add 0x00 in the end of array. We
     * also make the arrary to be 4 bytes alignment.
     *
     */


    /*RFC 5869 Test Case 2 --- it only need 80 bytes.*/
    const uint8_t ikm[84] =
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,
        0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
        0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
        0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
        0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45,
        0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x00, 0x00, 0x00, 0x00
    };



    /*Notice: it only need 80 bytes.*/
    const uint8_t salt[84] =
    {
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d,
        0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b,
        0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
        0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
        0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5,
        0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0x00, 0x00, 0x00, 0x00
    };

    /*Notice: it only need 80 bytes.*/
    const uint8_t info[84] =
    {
        0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd,
        0xbe, 0xbf, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb,
        0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9,
        0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
        0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5,
        0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff, 0x00, 0x00, 0x00, 0x00
    };

    /*Notice: RFC test case only need 82 bytes. we add two bytes 0x00 for avoid Keil warning*/
    const uint8_t expected_okm[84] =
    {
        0xB1, 0x1E, 0x39, 0x8D, 0xC8, 0x03, 0x27, 0xA1, 0xC8, 0xE7, 0xF7, 0x8C, 0x59, 0x6A, 0x49, 0x34,
        0x4F, 0x01, 0x2E, 0xDA, 0x2D, 0x4E, 0xFA, 0xD8, 0xA0, 0x50, 0xCC, 0x4C, 0x19, 0xAF, 0xA9, 0x7C,
        0x59, 0x04, 0x5A, 0x99, 0xCA, 0xC7, 0x82, 0x72, 0x71, 0xCB, 0x41, 0xC6, 0x5E, 0x59, 0x0E, 0x09,
        0xDA, 0x32, 0x75, 0x60, 0x0C, 0x2F, 0x09, 0xB8, 0x36, 0x77, 0x93, 0xA9, 0xAC, 0xA3, 0xDB, 0x71,
        0xCC, 0x30, 0xC5, 0x81, 0x79, 0xEC, 0x3E, 0x87, 0xC1, 0x4C, 0x01, 0xD5, 0xC1, 0xF3, 0x43, 0x4F,
        0x1D, 0x87, 0x00, 0x00
    };



    uint8_t   out_buf[128];      /*just allocate some large buffer... in fact, it only used 42 bytes*/
    uint32_t  out_length;
    uint32_t  status;

    out_length = 82;

    status = hkdf_sha256(out_buf, out_length,
                         ikm,  TestCase2_ikm_length,  salt, TestCase2_salt_length, info, TestCase2_info_length);

    if (status != STATUS_SUCCESS)
    {
        /*almost impossible to here.*/
        printf("Oops... check erorr \n");
        while (1);
    }

    /*compare result*/

    if (memcmp( (void *)expected_okm, (void *) out_buf, out_length) == 0)
    {
        printf("Test HKDF RFC 5869 Test Case 2 OK \n");
    }
    else
    {
        /*almost impossible*/
        printf("Oops ... Test  HKDF RFC 5869 Test Case 2 Error... Check Why\n");
        while (1);
    }

}


void test_hkdf_case3(void)
{
    /*RFC 5869 Test Case 3*/
    const uint8_t ikm[22] =
    {
        0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
        0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
        0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b
    };

    /*This case no salt, no info*/

    const uint8_t expected_okm[42] =
    {

        0x8D, 0xA4, 0xE7, 0x75, 0xA5, 0x63, 0xC1, 0x8F, 0x71, 0x5F, 0x80, 0x2A, 0x06, 0x3C, 0x5A, 0x31,
        0xB8, 0xA1, 0x1F, 0x5C, 0x5E, 0xE1, 0x87, 0x9E, 0xC3, 0x45, 0x4E, 0x5F, 0x3C, 0x73, 0x8D, 0x2D,
        0x9D, 0x20, 0x13, 0x95, 0xFA, 0xA4, 0xB6, 0x1A, 0x96, 0xC8
    };


    uint8_t   out_buf[128];      /*just allocate some large buffer... in fact, it only used 42 bytes*/
    uint32_t  out_length;
    uint32_t  status;

    out_length = 42;

    status = hkdf_sha256(out_buf, out_length,
                         ikm,  sizeof(ikm),  NULL, 0, NULL, 0);

    if (status != STATUS_SUCCESS)
    {
        /*almost impossible to here.*/
        printf("Oops... check erorr \n");
        while (1);
    }

    /*compare result*/

    if (memcmp( (void *)expected_okm, (void *) out_buf, out_length) == 0)
    {
        printf("Test HKDF RFC 5869 Test Case 3 OK \n");
    }
    else
    {
        /*almost impossible*/
        printf("Oops ... Test  HKDF RFC 5869 Test Case 3 Error... Check Why\n");
        while (1);
    }

}

int main(void)
{

    /*test not 4-bytes alignment case*/
    uint8_t        test[4], *ptr;



    /*Notice: SHA256 is big endian data, this is "abc" for sha256 value
     * --- RFC6234 page 96 test vector #1 for SHA256 */
    const uint8_t  exp_sha256_result[SHA256_DIGEST_SIZE] =
    {
        0xBA, 0x78, 0x16, 0xBF, 0x8F, 0x01, 0xCF, 0xEA,
        0x41, 0x41, 0x40, 0xDE, 0x5D, 0xAE, 0x22, 0x23,
        0xB0, 0x03, 0x61, 0xA3, 0x96, 0x17, 0x7A, 0x9C,
        0xB4, 0x10, 0xFF, 0x61, 0xF2, 0x00, 0x15, 0xAD
    };

    sha256_context sha_cnxt;
    uint8_t        sha256_digest[SHA256_DIGEST_SIZE];

    uint32_t       i;


    /*we should set pinmux here or in SystemInit */
    init_default_pin_mux();

    sha256_vector_init();

    /*init debug uart port for printf*/
    console_drv_init(PRINTF_BAUDRATE);

    Comm_Subsystem_Disable_LDO_Mode();//if don't load 569 FW, need to call the function.

    printf("Crypto SHA256 Related Test %s %s\n", __DATE__, __TIME__);

    for (i = 0; i < 4; i++)
    {
        test[i] = 0x61 + i;
    }

    ptr = test;

    sha256_init(&sha_cnxt);
    sha256_update(&sha_cnxt, ptr, 3);           //test sha256("abc")
    sha256_finish(&sha_cnxt, sha256_digest);

    for (i = 0; i < SHA256_DIGEST_SIZE; i++)
    {
        /*show sha256 value*/
        printf("%02X", sha256_digest[i]);
    }

    if (memcmp((uint8_t *) exp_sha256_result, sha256_digest, SHA256_DIGEST_SIZE) != 0)
    {
        /*code should not goto here. */
        printf("Oops, sha256(\"abc\") value is wrong \n");
        while (1);
    }

    printf("\nTest RFC6234 sha256 vector #1 successful \n");

    printf("\n\n");

    /*Please NOTICE: HMAC over SHA256 will need more than 200 bytes stack space*/
    /*Please see RFC RFC4231 test vector*/
    test_hmac_case1();
    test_hmac_case2();
    test_hmac_case3();
    test_hmac_case4();
    test_hmac_case5();
    test_hmac_case6();
    test_hmac_case7();

    printf("finish HMAC test\n");

    printf("\n\n");
    /*
     * test RFC5869 HMAC based HKDF.... (NIST 800-56C Section 4.2 Option2)
     * if you want to use SM2 KDF please see example Crypto_SM2
     *
     */
    test_hkdf_case1();

    test_hkdf_case2();

    test_hkdf_case3();

    printf("test sha256 related all successful \n");

    while (1);
}


void SetClockFreq(void)
{
    return;
}


