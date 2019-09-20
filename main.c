/*
 * File: main.c
 * Author: Ted Salmon <tass2001@gmail.com>
 * Description:
 *     The main loop for our PIC24FJ
 */
#include <stdlib.h>
#include <stdio.h>
#include <xc.h>
#include "sysconfig.h"
#include "mappings.h"
#include "cli.h"
#include "lib/log.h"
#include "lib/i2c.h"
#include "lib/timer.h"
#include "lib/uart.h"

int main(void)
{
    // Set all used ports to digital mode
    ANSB = 0;
    ANSD = 0;
    ANSE = 0;
    ANSF = 0;
    ANSG = 0;

    // Set the I/O ports to outputs
    ON_LED_MODE = 0;

    // Initialize the system UART first, since we needed it for debug
    struct UART_t systemUart = UARTInit(
        SYSTEM_UART_MODULE,
        SYSTEM_UART_RX_PIN,
        SYSTEM_UART_TX_PIN,
        SYSTEM_UART_RX_PRIORITY,
        SYSTEM_UART_TX_PRIORITY,
        UART_BAUD_115200,
        UART_PARITY_NONE
    );

    // All UART handler registrations need to be done at
    // this level to maintain a global scope
    UARTAddModuleHandler(&systemUart);
    LogMessage("", "**** T60SVP ****");

    struct CLI_t cli = CLIInit(&systemUart);

    ON_LED = 1;
    TimerInit();
    I2CInit();

    // Process events
    while (1) {
        TimerProcessScheduledTasks();
        CLIProcess(&cli);
    }

    return 0;
}

// Trap Catches
void __attribute__ ((__interrupt__, auto_psv)) _OscillatorFail(void)
{
    // Clear the trap flag
    INTCON1bits.OSCFAIL = 0;
    ON_LED = 0;
    while (1);
}

void __attribute__ ((__interrupt__, auto_psv)) _AddressError(void)
{
    // Clear the trap flag
    INTCON1bits.ADDRERR = 0;
    ON_LED = 0;
    while (1);
}


void __attribute__ ((__interrupt__, auto_psv)) _StackError(void)
{
    // Clear the trap flag
    INTCON1bits.STKERR = 0;
    ON_LED = 0;
    while (1);
}

void __attribute__ ((__interrupt__, auto_psv)) _MathError(void)
{
    // Clear the trap flag
    INTCON1bits.MATHERR = 0;
    ON_LED = 0;
    while (1);
}

void __attribute__ ((__interrupt__, auto_psv)) _NVMError(void)
{
    ON_LED = 0;
    while (1);
}

void __attribute__ ((__interrupt__, auto_psv)) _GeneralError(void)
{
    ON_LED = 0;
    while (1);
}
