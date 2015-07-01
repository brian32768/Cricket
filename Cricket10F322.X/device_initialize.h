/****************************************************************************
;*	Microchip Technology Inc. 2007                                      *
;*	Assembler version:  XC8 v1.34                                       *
;*	Filename:                                                           *
;*		device_initialize.h                                                       *
;*	Dependents:                                                         *
;*                                                                          *
;*	April 13, 2015                                                      *
;*      PIC10(L)32X Developmental Board                                     *
;*      Demonstration program:                                              *
;*          After applying power to the PIC10(L)F32X Development Board,     *
;*          LED (D1) will automatically turn on.  Turn POT1 clockwise       *
;*          to increase the brightness of LED (D2).  Press switch (SW1)     *
;*          to turn both LEDs D1 and D2 off, release switch (SW1) and       *
;*          LEDs D1 and D2 will turn on.                                    *
;*                                                                          *
;****************************************************************************/

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

#endif	/* DEVICE_INITIALIZE_H */
/**
 End of File
*/
