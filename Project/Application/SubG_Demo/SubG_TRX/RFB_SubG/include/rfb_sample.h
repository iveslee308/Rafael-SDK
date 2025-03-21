/**
 * @file rfb_sample.h
 * @author
 * @date
 * @brief Brief single line description use for indexing
 *
 * More detailed description can go here
 *
 *
 * @see http://
 */
#ifndef _RFB_SAMPLE_H_
#define _RFB_SAMPLE_H_
/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include "radio.h"

/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
#define PRBS9_CONTENT                                                                                           \
    {                                                                                                           \
            0xFF, 0xC1, 0xFB, 0xE8, 0x4C, 0x90, 0x72, 0x8B,                                                     \
            0xE7, 0xB3, 0x51, 0x89, 0x63, 0xAB, 0x23, 0x23,                                                     \
            0x02, 0x84, 0x18, 0x72, 0xAA, 0x61, 0x2F, 0x3B,                                                     \
            0x51, 0xA8, 0xE5, 0x37, 0x49, 0xFB, 0xC9, 0xCA,                                                     \
            0x0C, 0x18, 0x53, 0x2C, 0xFD, 0x45, 0xE3, 0x9A,                                                     \
            0xE6, 0xF1, 0x5D, 0xB0, 0xB6, 0x1B, 0xB4, 0xBE,                                                     \
            0x2A, 0x50, 0xEA, 0xE9, 0x0E, 0x9C, 0x4B, 0x5E,                                                     \
            0x57, 0x24, 0xCC, 0xA1, 0xB7, 0x59, 0xB8, 0x87,                                                     \
            0xFF, 0xE0, 0x7D, 0x74, 0x26, 0x48, 0xB9, 0xC5,                                                     \
            0xF3, 0xD9, 0xA8, 0xC4, 0xB1, 0xD5, 0x91, 0x11,                                                     \
            0x01, 0x42, 0x0C, 0x39, 0xD5, 0xB0, 0x97, 0x9D,                                                     \
            0x28, 0xD4, 0xF2, 0x9B, 0xA4, 0xFD, 0x64, 0x65,                                                     \
            0x06, 0x8C, 0x29, 0x96, 0xFE, 0xA2, 0x71, 0x4D,                                                     \
            0xF3, 0xF8, 0x2E, 0x58, 0xDB, 0x0D, 0x5A, 0x5F,                                                     \
            0x15, 0x28, 0xF5, 0x74, 0x07, 0xCE, 0x25, 0xAF,                                                     \
            0x2B, 0x12, 0xE6, 0xD0, 0xDB, 0x2C, 0xDC, 0xC3,                                                     \
            0x7F, 0xF0, 0x3E, 0x3A, 0x13, 0xA4, 0xDC, 0xE2,                                                     \
            0xF9, 0x6C, 0x54, 0xE2, 0xD8, 0xEA, 0xC8, 0x88,                                                     \
            0x00, 0x21, 0x86, 0x9C, 0x6A, 0xD8, 0xCB, 0x4E,                                                     \
            0x14, 0x6A, 0xF9, 0x4D, 0xD2, 0x7E, 0xB2, 0x32,                                                     \
            0x03, 0xC6, 0x14, 0x4B, 0x7F, 0xD1, 0xB8, 0xA6,                                                     \
            0x79, 0x7C, 0x17, 0xAC, 0xED, 0x06, 0xAD, 0xAF,                                                     \
            0x0A, 0x94, 0x7A, 0xBA, 0x03, 0xE7, 0x92, 0xD7,                                                     \
            0x15, 0x09, 0x73, 0xE8, 0x6D, 0x16, 0xEE, 0xE1,                                                     \
            0x3F, 0x78, 0x1F, 0x9D, 0x09, 0x52, 0x6E, 0xF1,                                                     \
            0x7C, 0x36, 0x2A, 0x71, 0x6C, 0x75, 0x64, 0x44,                                                     \
            0x80, 0x10, 0x43, 0x4E, 0x35, 0xEC, 0x65, 0x27,                                                     \
            0x0A, 0xB5, 0xFC, 0x26, 0x69, 0x3F, 0x59, 0x99,                                                     \
            0x01, 0x63, 0x8A, 0xA5, 0xBF, 0x68, 0x5C, 0xD3,                                                     \
            0x3C, 0xBE, 0x0B, 0xD6, 0x76, 0x83, 0xD6, 0x57,                                                     \
            0x05, 0x4A, 0x3D, 0xDD, 0x81, 0x73, 0xC9, 0xEB,                                                     \
            0x8A, 0x84, 0x39, 0xF4, 0x36, 0x0B, 0xF7                                                            \
    }

/**************************************************************************************************
*    TYPEDEFS
*************************************************************************************************/
typedef uint8_t RFB_PCI_TEST_CASE;
#define RFB_PCI_BURST_TX_TEST         ((RFB_PCI_TEST_CASE)0x01)
#define RFB_PCI_SLEEP_TX_TEST         ((RFB_PCI_TEST_CASE)0x02)
#define RFB_PCI_RX_TEST               ((RFB_PCI_TEST_CASE)0x03)
/**************************************************************************************************
 *    Global Prototypes
 *************************************************************************************************/
void rfb_sample_init(uint8_t RfbPciTestCase);
void rfb_sample_entry(uint8_t rfb_pci_test_case);
#endif

