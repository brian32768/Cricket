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

void NCO_Set(uint8_t hbyte, uint8_t lbyte);

#endif	/* NCO_H */

