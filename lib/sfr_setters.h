/*
 * File: sfr_setters.h
 * Author: Ted Salmon <tass2001@gmail.com>
 * Description:
 *     Set various special function registers
 */
#ifndef SFR_SETTERS_H
#define SFR_SETTERS_H
void SetI2CMAEV(unsigned index, unsigned value);
void SetI2CSLEV(unsigned index, unsigned value);
void SetSPIIE(unsigned index, unsigned value);
void SetSPITXIE(unsigned index, unsigned value);
void SetSPIRXIE(unsigned index, unsigned value);
void SetTIMERIE(unsigned index, unsigned value);
void SetTIMERIF(unsigned index, unsigned value);
void SetTIMERIP(unsigned index, unsigned value);
void SetUARTRXIE(unsigned index, unsigned value);
void SetUARTRXIF(unsigned index, unsigned value);
void SetUARTRXIP(unsigned index, unsigned value);
void SetUARTTXIE(unsigned index, unsigned value);
void SetUARTTXIF(unsigned index, unsigned value);
void SetUARTTXIP(unsigned index, unsigned value);
#endif /* SFR_SETTERS_H */
