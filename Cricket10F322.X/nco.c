//
// Numerically controlled oscillator module
//

#include <xc.h>
#include "nco.h"

#define NxEN  0x80    // enable NCO
#define NxOE  0x00    // disable output on RA2 (we'll get output on CWG output which is RA1)
#define NxPOL 0x00    // non-inverted output


void NCO_Initialize(void) {
    // Set clock for NCO.
    NCO1CLK = 0xE1;
}

void NCO_Start(void) {

  NCO1CON = NxEN | NxOE | NxPOL;
  
}

