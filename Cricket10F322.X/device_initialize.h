#ifndef DEVICE_INITIALIZE_H
#define	DEVICE_INITIALIZE_H
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "adc.h"
#include "pwm.h"
#include "tmr2.h"
#include "temperature.h"
#include "nco.h"

//    Initializes the device
void SYSTEM_Initialize(void);

// Initializes the oscillator
void OSCILLATOR_Initialize(void);

void Enable_Interrupts(void);

#endif	/* DEVICE_INITIALIZE_H */

