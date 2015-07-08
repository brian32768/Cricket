// based on the sample code
//
// I am using timer 2 to set the on/off cycle for the cricket
// peep .... peep ... peep ...

#include <xc.h>
#include "tmr2.h"

void Timer2_Initialize(void)
{
    // Note when you write to TMR2 then the pre and post scalers
    // are cleared back to 1:1        

    TMR2 = 0x00; // Clear timer 2

    // with FOsc set to 4MHz
    
    // TMR2ON off; T2CKPS 1:1; T2OUTPS 1:1;
    // T2CON = 0x00;
    // PR2 = 0xFF;
    // gives me 3.9 kHz
 
    // TMR2ON off; T2CKPS 1:4; T2OUTPS 1:1;
    //T2CON = 0x01;
    // gives 1.9 kHz
    
//    T2CONbits.TOUTPS = 0x0F; // Postscaler = 1:16 -- only affects interrupts
    T2CONbits.TOUTPS = 0x09; // Postscaler = 1:10 -- only affects interrupts
    //T2CONbits.T2CKPS = 0x03; // Prescaler = 1:64
    T2CONbits.T2CKPS = 0x02; // Prescaler = 1:16
    
    T2CONbits.TMR2ON = 0x00; // Timer2 is OFF

    PR2  = 0xFF;
    
    // Timer 2 will count from 0 until it gets to PR2
    
    //so period = (1/ FOsc/4) / prescaler * PR2) 
    // f = 1/period

    // Clearing IF flag.
    PIR1bits.TMR2IF = 0;
}

void Timer2_Enable(void) {
    T2CONbits.TMR2ON = 1;
}

void Timer2_Disable(void) {
    T2CONbits.TMR2ON = 0;
}

void Timer2_SetPeriod(uint8_t p) {
    PR2 = p;
}




