/* 
 * File:   temperature.h
 * Author: bwilson
 *
 * Created on June 28, 2015, 11:35 AM
 */

#ifndef TEMPERATURE_H
#define	TEMPERATURE_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#include "adc.h"

typedef uint8_t temperature_result_t;

// Sets up the register(s) for the temperature indicator.
void TEMPERATURE_Initialize(void);

// Starts the temperature indicator module
void TEMPERATURE_Start(void);

// Returns the current 8 bit reading from the temperature indicator.
temperature_result_t TEMPERATURE_Get(void);

#endif	/* TEMPERATURE_H */

