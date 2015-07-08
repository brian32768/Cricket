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
    
    // ----------------
    // Set up complementary waveform generator to track NCO.
    // See docs page 137
    // Step 1. 
    TRISAbits.TRISA0 = 1;   // Set CWG1A as input. (See docs page 68)
    TRISAbits.TRISA1 = 1;   // Set CWG1B as input. (See docs page 68)
    // Step 2.
    CWG1CON0bits.G1EN = 0; // Disable CWG module
    // Step 3. Set dead band registers to 0 to bypass dead-band generation.
    CWG1DBR = 0;
    CWG1DBF = 0;
    // Step 4. 
    CWG1CON2bits.G1ASDSCLC1 = 0; // LC1OUT has no effect on shutdown
    CWG1CON2bits.G1ASDSFLT  = 0; // CWG1FLT input has no effect on shutdown
    //   Set shutdown states
    CWG1CON1bits.G1ASDLA = 0x01; // Tristate CWG1A output when in shutdown.
    CWG1CON1bits.G1ASDLB = 0x02; // Drive CWG1B output low when in shutdown.
    //   Set enable and clear auto-restart enable
    CWG1CON2bits.G1ASE   = 1;
    CWG1CON2bits.G1ARSEN = 0;
    // Step 5. Select input source
    CWG1CON1bits.G1IS    = 0x02; // Source is N1OUT
    // Step 6. 
    CWG1CON0bits.G1CS = 0; // Set clock source to FOsc
    //   Set output polarities
    CWG1CON0bits.G1POLA = 0; // Polarity A is normal
    CWG1CON0bits.G1POLB = 1; // Polarity B is normal
    //   Enable CWG1B output (pin RA1)
    CWG1CON0bits.G1OEA  = 0; // Disable CWG1A output on RA0
    CWG1CON0bits.G1OEB  = 1; // Enable CWG1B output on RA1
    // Step 7.
    CWG1CON0bits.G1EN = 1; // Enable CWG module
    // Step 8.
    TRISAbits.TRISA0 = 0;   // Set CWG1A as output. (See docs page 68)
    TRISAbits.TRISA1 = 0;   // Set CWG1B as output. (See docs page 68)

    // Step 9. Clear G1ASE to start CWG 
    CWG1CON2bits.G1ASE   = 0;
}

// Enable NCO output on RA2
void NCO_Enable(void) {
    NCO1CONbits.N1EN = 1; // Enable NCO
//    NCO1CONbits.N1OE = 1; 
//    TRISAbits.TRISA2 = 0;   // Set NCO1 as output. (See docs page 68)
//    TRISAbits.TRISA1 = 0;   // Set CWG1B as output. (See docs page 68)
}

// Disable NCO output on RA2
void NCO_Disable(void) {
    NCO1CONbits.N1EN = 0; // Disable NCO
//    NCO1CONbits.N1OE = 0; // Disable NCO output
//    TRISAbits.TRISA2 = 1;   // Set NCO1 as input. (See docs page 68)
//    TRISAbits.TRISA1 = 1;   // Set CWG1B as output. (See docs page 68)
}

// Enable CWG to make the output LOUDER.
void CWG_Enable(void) {
    CWG1CON0bits.G1EN = 1; // Enable CWG module
}

// Disable CWG to make the output softer.
void CWG_Disable(void) {
    CWG1CON0bits.G1EN = 0; // Disable CWG module
}

void NCO_Set(uint8_t hbyte, uint8_t lbyte) {
    // NB ALWAYS LOAD H FIRST else it is ignored. Values are latched on L
    NCO1INCH = hbyte;
    NCO1INCL = lbyte;
}


