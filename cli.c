/*
 * File:   cli.c
 * Author: Ted Salmon <tass2001@gmail.com>
 * Description:
 *     Implement a CLI to pass commands to the device
 */
#include "cli.h"

char convTable[256] = {
    ' ', ' ', '1', '2', '3', '4', '5', '6',    '7', '8', '9', '0', '-', '=', ' ', ' ', 
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',    'O', 'P', ' ', '$', ' ', ' ', 'A', 'S', 
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ';',    '\'','`', '\\','\\','Z', 'X', 'C', 'V', 
    'B', 'N', 'M', ',', '.', '/', '"', '*',    ' ', ' ', ' ', ' ', 'f', 'f', 'f', 'f',
    
    'f', 'f', 'f', 'f', 'f', ' ', ' ', '7',    '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', ' ', ' ', '>', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
};

char convTableAlternate[256] = {
    ' ', ' ', ' ', ' ', '1', '1', '2', '3',    '3', '3', '4', '4', '5', '5', '6', '6', 
    '7', '7', '8', '8', '9', '9', '0', '0',    '-', '-', '=', '=', ' ', ' ', ' ', ' ',
    'Q', 'Q', 'W', 'W', 'E', 'E', 'R', 'R',    'T', 'T', 'Y', 'Y', 'U', 'U', 'I', 'I',
    'O', 'O', 'P', 'P', ' ', ' ', '$', '$',    ' ', ' ', ' ', ' ', 'A', 'A', 'S', 'S',
    
    'D', 'D', 'F', 'F', 'G', 'G', 'H', 'H',    'J', 'J', 'K', 'K', 'L', 'L', ';', ';',
    '\'','\'','`', '`', '\\','\\','\\','\\',   'Z', 'Z', 'X', 'X', 'C', 'C', 'V', 'V',
    'B', 'B', 'N', 'N', 'M', 'M', ',', ',',    '.', '.', '/', '/', '"', '"', '*', '*',
    ' ', ' ', ' ', ' ', ' ', ' ', 'f', 'f',    'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f',
    
    'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f',    'f', 'f', ' ', ' ', ' ', ' ', '7', '7',
    '8', '8', '9', '9', '-', '-', '4', '4',    '5', '5', '6', '6', '+', '+', '1', '1',
    '2', '2', '3', '3', '0', '0', '.', '.',    ' ', ' ', ' ', ' ', '>', '>', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
};

/**
 * CLIInit()
 *     Description:
 *         Initialize our CLI object
 *     Params:
 *         UART_t *uart - A pointer to the UART module object
 *     Returns:
 *         void
 */
CLI_t CLIInit(UART_t *uart)
{
    CLI_t cli;
    cli.uart = uart;
    cli.lastChar = 0;
    cli.lastRxTimestamp = 0;
    return cli;
}

/**
 * CLIProcess()
 *     Description:
 *         Read the RX queue and process the messages into meaningful data
 *     Params:
 *         UART_t *uart - A pointer to the UART module object
 *     Returns:
 *         void
 */
void CLIProcess(CLI_t *cli)
{
    while (cli->lastChar != cli->uart->rxQueue.writeCursor) {
        unsigned char nextChar = CharQueueGet(&cli->uart->rxQueue, cli->lastChar);
        UARTSendChar(cli->uart, nextChar);
        if (cli->lastChar >= (CHAR_QUEUE_SIZE - 1)) {
            cli->lastChar = 0;
        } else {
            cli->lastChar++;
        }
    }
    // Check for the backspace character
    uint16_t backspaceLegnth = CharQueueSeek(&cli->uart->rxQueue, CLI_MSG_DELETE_CHAR);
    if (backspaceLegnth > 0) {
        if (cli->lastChar < 2) {
            cli->lastChar = CHAR_QUEUE_SIZE - (3 - cli->lastChar);
        } else {
            cli->lastChar = cli->lastChar - 2;
        }
        // Remove the backspace character
        CharQueueRemoveLast(&cli->uart->rxQueue);
        // Send the "back one" character, space character and then back one again
        if (cli->uart->rxQueue.size > 0) {
            UARTSendChar(cli->uart, '\b');
            UARTSendChar(cli->uart, ' ');
            UARTSendChar(cli->uart, '\b');
        }
        // Remove the character before it
        CharQueueRemoveLast(&cli->uart->rxQueue);
    }
    uint16_t messageLength = CharQueueSeek(&cli->uart->rxQueue, CLI_MSG_END_CHAR);
    if (messageLength > 0) {
        // Send a newline to keep the CLI pretty
        UARTSendChar(cli->uart, 0x0A);
        char msg[messageLength];
        uint16_t i;
        uint8_t delimCount = 1;
        for (i = 0; i < messageLength; i++) {
            char c = CharQueueNext(&cli->uart->rxQueue);
            if (c == CLI_MSG_DELIMETER) {
                delimCount++;
            }
            if (c != CLI_MSG_END_CHAR) {
                msg[i] = c;
            } else {
                // 0x0D delimits messages, so we change it to a null
                // terminator instead
                msg[i] = '\0';
            }
        }
        uint8_t cmdSuccess = 1;
        if (messageLength > 1) {
            // Copy the message, since strtok adds a null terminator after the first
            // occurrence of the delimiter, it will not cause issues with string
            // functions
            char tmpMsg[messageLength];
            strcpy(tmpMsg, msg);
            char *msgBuf[delimCount];
            char *p = strtok(tmpMsg, " ");
            i = 0;
            while (p != NULL) {
                msgBuf[i++] = p;
                p = strtok(NULL, " ");
            }
            if (UtilsStricmp(msgBuf[0], "REBOOT") == 0) {
                __asm__ volatile ("reset");
            } else if (UtilsStricmp(msgBuf[0], "SCAN") == 0) {
                unsigned char addr = 0x00;
                while (addr < 255) {
                    int8_t status = I2CPoll(addr);
                    if (status == 0x00) {
                        LogDebug("%02X Responded to Poll", addr);
                    }
                    addr++;
                }
            } else if (UtilsStricmp(msgBuf[0], "PASSWORD") == 0) {
                uint8_t i = 0;
                int8_t status;
                unsigned char password[8];
                unsigned char buffer;
                while (i < 8) {
                    status = I2CRead(0x57, 0x38 + i, &buffer);
                    LogDebug("Password[%d]: %02x", i, buffer);
                    password[i] = convTable[buffer];
                    i++;
                }
                password[7] = '\0';
                LogDebug("Got Password: '%s'", password);
            } else if (UtilsStricmp(msgBuf[0], "HELP") == 0 || UtilsStricmp(msgBuf[0], "?") == 0) {
                LogRaw("Available Commands:\r\n");
                LogRaw("    REBOOT - Reboot the device\r\n");
                LogRaw("    SCAN - Scan the I2C bus for slaves\r\n");
                LogRaw("    PASSWORD - Pull the SVP from the 4th slave address of the EEPROM (Password starts at 0x338)\r\n");
            } else {
                cmdSuccess = 0;
            }
            if (cmdSuccess == 0) {
                LogRaw("Command not found. Try HELP or ?\r\n# ");
            } else {
                LogRaw("OK\r\n# ");
            }
        } else {
            if (((TimerGetMillis() - cli->lastRxTimestamp) / 1000) > CLI_BANNER_TIMEOUT ||
                cli->lastRxTimestamp == 0
            ) {
                LogRaw("~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
                LogRaw("T60SVP Firmware: 1.0.0.0\r\n");
                LogRaw("Try HELP or ?\r\n");
                LogRaw("~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
            }
            LogRaw("# ");

        }
        cli->lastRxTimestamp = TimerGetMillis();
    }
}
