/*
 * SPDX-FileCopyrightText: 2020 Efabless Corporation
 * SPDX-FileCopyrightText: 2022 Andrew Foote
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * SPDX-License-Identifier: Apache-2.0
 */

// This include is relative to $CARAVEL_PATH (see Makefile)
#include <defs.h>
#include <stub.c>

/*
    4FT4 Fibonnaci test:
        - Configures lower 8 user IO pins as outputs
        - Does a basic test of reading and writing from the start of RAM 0 over wishbone
        - Downloads a program to ROM 0 at offset 64 that generates fibonacci numbers
          in RAM 0 words 0 to 31
        - Waits for the final fibonacci number to arrive, then validates the contents
          of RAM 0 words 0 to 15
*/

#define reg_mprj_wb_base ((volatile uint32_t*)0x30000000)
#define reg_mprj_wb_base_b ((volatile uint8_t*)0x30000000)

#define ROM_BASE 0x0
#define RAM_BASE 0x10000

#define FAIL(n) \
    do { \
        reg_mprj_datal = 0xDEAD000 + (n << 16); \
    } while (1);

static uint32_t program[] = {
  // CLC
  0xf1,
  // FIM 0P 0
  0x20,
  0x00,
  // FIM 1P 0
  0x22,
  0x00,
  // FIM 2P 1
  0x24,
  0x01,
  // SRC 0P
  0x21,
  // LD 3
  0xa3,
  // WRM
  0xe0,
  // INC 1
  0x61,
  // SRC 0P
  0x21,
  // LD 2
  0xa2,
  // WRM
  0xe0,
  // INC 1
  0x61,
  // SRC 0P
  0x21,
  // LD 5
  0xa5,
  // WRM
  0xe0,
  // INC 1
  0x61,
  // SRC 0P
  0x21,
  // LD 4
  0xa4,
  // WRM
  0xe0,
  // INC 1
  0x61,
  // LDM 14
  0xde,
  // XCH 14
  0xbe,
  // LDM 10
  0xda,
  // XCH 15
  0xbf,
  // SRC 0P
  0x21,
  // CLC
  0xf1,
  // LD 3
  0xa3,
  // ADD 5
  0x85,
  // WRM
  0xe0,
  // XCH 7
  0xb7,
  // INC 1
  0x61,
  // SRC 0P
  0x21,
  // LD 2
  0xa2,
  // ADD 4
  0x84,
  // WRM
  0xe0,
  // XCH 6
  0xb6,
  // INC 1
  0x61,
  // SRC 0P
  0x21,
  // LD 5
  0xa5,
  // XCH 3
  0xb3,
  // LD 4
  0xa4,
  // XCH 2
  0xb2,
  // LD 7
  0xa7,
  // XCH 5
  0xb5,
  // LD 6
  0xa6,
  // XCH 4
  0xb4,
  // ISZ 15
  0x7f,
  0x5b,
  // LDM 8
  0xd8,
  // XCH 15
  0xbf,
  // INC 0
  0x60,
  // ISZ 14
  0x7e,
  0x5b,
  // LDM 7
  0xd7,
  // WMP
  0xe1
};

void main()
{
    /*
    IO Control Registers
    | DM     | VTRIP | SLOW  | AN_POL | AN_SEL | AN_EN | MOD_SEL | INP_DIS | HOLDH | OEB_N | MGMT_EN |
    | 3-bits | 1-bit | 1-bit | 1-bit  | 1-bit  | 1-bit | 1-bit   | 1-bit   | 1-bit | 1-bit | 1-bit   |
    Output: 0000_0110_0000_1110  (0x1808) = GPIO_MODE_USER_STD_OUTPUT
    | DM     | VTRIP | SLOW  | AN_POL | AN_SEL | AN_EN | MOD_SEL | INP_DIS | HOLDH | OEB_N | MGMT_EN |
    | 110    | 0     | 0     | 0      | 0      | 0     | 0       | 1       | 0     | 0     | 0       |


    Input: 0000_0001_0000_1111 (0x0402) = GPIO_MODE_USER_STD_INPUT_NOPULL
    | DM     | VTRIP | SLOW  | AN_POL | AN_SEL | AN_EN | MOD_SEL | INP_DIS | HOLDH | OEB_N | MGMT_EN |
    | 001    | 0     | 0     | 0      | 0      | 0     | 0       | 0       | 0     | 1     | 0       |
    */

    /* Set up the housekeeping SPI to be connected internally so    */
    /* that external pin changes don't affect it.            */

    reg_spi_enable = 1;
    reg_wb_enable = 1;
    // reg_spimaster_config = 0xa002;    // Enable, prescaler = 2,
                                        // connect to housekeeping SPI

    // Connect the housekeeping SPI to the SPI master
    // so that the CSB line is not left floating.  This allows
    // all of the GPIO pins to be used for user functions.

    reg_mprj_io_31 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_30 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_29 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_28 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_27 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_26 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_25 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_24 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_23 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_22 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_21 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_20 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_19 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_18 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_17 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_16 = GPIO_MODE_MGMT_STD_OUTPUT;

    reg_mprj_io_11 = GPIO_MODE_USER_STD_OUTPUT;
    reg_mprj_io_10 = GPIO_MODE_USER_STD_OUTPUT;
    reg_mprj_io_9 = GPIO_MODE_USER_STD_OUTPUT;
    reg_mprj_io_8 = GPIO_MODE_USER_STD_OUTPUT;
    reg_mprj_io_7 = GPIO_MODE_USER_STD_OUTPUT;
    reg_mprj_io_6 = GPIO_MODE_USER_STD_OUTPUT;
    reg_mprj_io_5 = GPIO_MODE_USER_STD_OUTPUT;
    reg_mprj_io_4 = GPIO_MODE_USER_STD_OUTPUT;

    reg_mprj_io_3 = GPIO_MODE_USER_STD_INPUT_PULLDOWN;
    reg_mprj_io_2 = GPIO_MODE_USER_STD_INPUT_PULLDOWN;
    reg_mprj_io_1 = GPIO_MODE_USER_STD_INPUT_PULLDOWN;
    reg_mprj_io_0 = GPIO_MODE_USER_STD_INPUT_PULLDOWN;

     /* Apply configuration */
    reg_mprj_xfer = 1;
    while (reg_mprj_xfer == 1);


    // LA0 as output for halt
    reg_la0_oenb = reg_la0_iena = 0xffffffff;
    // LA1 as output for reset
    reg_la1_oenb = reg_la1_iena = 0xffffffff;

    reg_la2_oenb = reg_la2_iena = 0x00000000;    // [95:64]

    reg_la0_data = 0x1;
    reg_la1_data = 0x1;

    for (int i = 0; i < 8; i++) {
        reg_la1_data = 0x1;
    }

    reg_la1_data = 0x0;

    // 4ft4 is still halted

    // Flag start of the test
    reg_mprj_datal = 0xAB600000;

    *((volatile uint32_t *)(reg_mprj_wb_base_b + RAM_BASE)) = 0xfefefefe;
    if ((*((volatile uint32_t *)(reg_mprj_wb_base_b + RAM_BASE))) != 0xe) {
        FAIL(1);
    }

    *((volatile uint32_t *)(reg_mprj_wb_base_b + RAM_BASE + 4)) = 0xeaeaeaea;
    if ((*((volatile uint32_t *)(reg_mprj_wb_base_b + RAM_BASE + 4))) != 0xa) {
        FAIL(2);
    }

    // Load the program into ROM
    for (int i = 0; i < sizeof(program)/sizeof(program[0]); i++) {
        uint32_t w = program[i];

        *(reg_mprj_wb_base + 64 + i) = w;
    }

    // Clear out the first 64 B to break the loop
    for (int i = 0; i < 64; i++) {
        *(reg_mprj_wb_base + i) = 0x0;
    }

    // unhalt 4ft4
    reg_la0_data = 0x0;

    /*
    !expect ram 0 reg 0: 0 0 1 0 1 0 2 0 3 0 5 0 8 0 d 0
    !expect ram 0 reg 1: 5 1 2 2 7 3 9 5 0 9 9 e 9 7 2 6
    */

    // wait for the last expected value to show up in 4ft4 ram
    while ((*((volatile uint32_t *)(reg_mprj_wb_base_b + RAM_BASE + 0x7c))) != 0x6) {
        reg_mprj_datal = 0xAB610000;
    }
    reg_mprj_datal = 0xAB620000;

    const uint8_t expectation[] = { 0, 0, 1, 0, 1, 0, 2, 0, 3, 0, 5, 0, 8, 0, 0xd, 0};
    uint16_t sum = 0;
    for (int i = 0; i < 16; i++) {
        uint8_t v = (*((volatile uint32_t *)(reg_mprj_wb_base_b + RAM_BASE + 4 * i)));
        reg_mprj_datal = (((uint32_t)v) << 16);
        sum += v;
    }

    if (sum != 33) {
        FAIL(0x100);
    }

    reg_mprj_datal = 0x13370000;
}
