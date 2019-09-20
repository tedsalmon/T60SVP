/*
 * IO Mappings for the PIC24
 */
#ifndef IO_MAPPING_H
#define IO_MAPPING_H
#define SYSTEM_UART_MODULE 1
#define SYSTEM_UART_RX_PRIORITY 3
#define SYSTEM_UART_TX_PRIORITY 4
#define SYSTEM_UART_RX_PIN 10
#define SYSTEM_UART_TX_PIN 17

#define I2C1_SDA_MODE TRISAbits.TRISA15
#define I2C1_SDA_DC ODCAbits.ODCA15
#define I2C1_SDA LATAbits.LATA15
#define I2C1_SDA_STATUS PORTAbits.RA15

#define I2C1_SCL_MODE TRISAbits.TRISA14
#define I2C1_SCL_DC ODCAbits.ODCA14
#define I2C1_SCL LATAbits.LATA14
#define I2C1_SCL_STATUS PORTAbits.RA14

#define ON_LED_MODE TRISAbits.TRISA7
#define ON_LED LATAbits.LATA7
#endif /* IO_MAPPING_H */
