
#ifndef _TMR2_H
#define _TMR2_H

#include <stdint.h>
#include <stdbool.h>

void Timer2_Initialize(void);

void Timer2_Enable(void);

void Timer2_Disable(void);

void Timer2_SetPeriod(uint8_t);

#endif // _TMR2_H
