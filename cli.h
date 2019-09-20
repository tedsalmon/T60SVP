/*
 * File: cli.h
 * Author: Ted Salmon <tass2001@gmail.com>
 * Description:
 *     Implement a CLI to pass commands to the device
 */
#ifndef CLI_H
#define CLI_H
#define _ADDED_C_LIB 1
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mappings.h"
#include "lib/char_queue.h"
#include "lib/i2c.h"
#include "lib/timer.h"
#include "lib/uart.h"
#include "lib/utils.h"

// Banner timeout is in seconds
#define CLI_BANNER_TIMEOUT 300
#define CLI_MSG_END_CHAR 0x0D
#define CLI_MSG_DELIMETER 0x20
#define CLI_MSG_DELETE_CHAR 0x7F
/**
 * CLI_t
 *     Description:
 *         This object defines our CLI object
 *     Fields:
 *         UART_t *uart - A pointer to the UART module object
 *         uint16_t lastChar - The last character
 */
typedef struct CLI_t {
    UART_t *uart;
    uint16_t lastChar;
    uint32_t lastRxTimestamp;
} CLI_t;
CLI_t CLIInit(UART_t *);
void CLIProcess(CLI_t *);
#endif /* CLI_H */
