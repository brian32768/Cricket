//
// Numerically controlled oscillator module
//

#include <xc.h>
#include "nco.h"

void NCO_Initialize(void) {
    NCO1CONbits.N1EN = 1; // Enable NCO
    NCO1CONbits.N1OE = 1; // Enable NCO output on RA2

    // Set clock for NCO.
    NCO1CLKbits.N1PWS = 0; // PWS only meaningful in pulse wid mode
    NCO1CLKbits.N1CKS = 1; // Clock source is FOsc
    //NCO1CLKbits.N1CKS = 2; // Clock source is 16 MHz
}

void NCO_Start(void) {  
    // NCO increment registers
    //NCO1INCL =  64; // 122 Hz
    //NCO1INCH =   0;
    NCO_Set(2,0);
}

void NCO_Stop(void) {
    NCO_Set(0, 0);
}

void NCO_Set(uint8_t hbyte, uint8_t lbyte) {
    // NB ALWAYS LOAD H FIRST else it is ignored. Values are latched on L
    NCO1INCH = hbyte;
    NCO1INCL = lbyte;
}


