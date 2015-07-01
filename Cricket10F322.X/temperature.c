#include <xc.h>
#include "temperature.h"

#define FVREN 0x80    // enable fixed voltage reference
#define ADFVR 0x00    // AD fixed voltage reference output is OFF
#define TSEN  0x20    // enable temp sensor
#define TSRNG 0x20    // temp range = high for 5v operation (test fixture)
//#define TSRNG 0x00  //            = low for 1.8-3.3v operation (button battery)


// Sets up the register(s) for the temperature indicator.
void TEMPERATURE_Initialize(void) {

    // This conflicts with the ADC POT reading code...
    
    // 0x99 = 10011001 = 100 110 0 1
    // ADCS = 100 = FOsc/4
    // CHS  = 110 = Temperature indicator
    // GO   = 0   = conversion ready
    // ADON = 1   = ON
            
    ADCON = 0x99;         // ADON enabled; CHS TEMPERATURE;
}

// Starts the temperature indicator module
void TEMPERATURE_Start(void) {
    // enable TSEN bit in fixed voltage reference (FVR)
    FVRCON = FVREN | TSEN | TSRNG | ADFVR; 
}

// Returns the current 8 bit reading from the temperature indicator.
temperature_result_t TEMPERATURE_Get(void) {
    return ADC_GetConversion(channel_TEMP);
}
