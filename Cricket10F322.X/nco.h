//
// Numerically controlled oscillator module
//

#ifndef NCO_H
#define	NCO_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>


void NCO_Initialize(void);

void NCO_Enable(void);
void NCO_Disable(void);

void NCO_SetHL(uint8_t high, uint8_t low);
void NCO_SetPitch(int pitch);

void CWG_Enable(void);
void CWG_Disable(void);

#endif	/* NCO_H */

