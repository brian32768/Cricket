//
//  Read from A/D input channel
//

#include <xc.h>
#include "adc.h"

void ADC_Initialize(void)
{

//  This code is set up to read from the POT on the 10F322 dev board
//  It's not currently compatible with my temperature code.

    // The meaning of ADCON    
    // 0x89 = 10001001
    // ADCS = 100 = FOsc/4
    // CHS  = 010 = AN2
    // GO   = 0   = conversion ready
    // ADON = 1   = ON
            
    ADCON = 0x89;         // ADON enabled; CHS AN2;

    TRISAbits.TRISA2 = 1; // POT1 pin (RA2) as input
    ANSELAbits.ANSA2 = 1; // configure (RA2) as analog input

}


adc_result_t ADC_GetConversion(adc_channel_t channel)
{
    // Select the A/D channel
    ADCONbits.CHS = channel;

    // Turn on the ADC module
    ADCONbits.ADON = 1;

    // Start the conversion
    ADCONbits.GO_nDONE = 1;

    // Wait for the conversion to finish
    while (ADCONbits.GO_nDONE);

    // Conversion finished, return the result
    return ADRES;
}

// End of File


