// Cricket 10F322
// based on MPLAB development board code
// Brian Wilson
// 2015-Jun-25

#include "device_initialize.h"

uint16_t adc; // define variable - unsigned integer

void pause(int count) {
    int a,b;
    for (a=0; a<count; a++) {
        for (b=0; b<10; b++) {
        // dodoodoodo
        }
    }
}

// LED's
#define RED     LATAbits.LATA1
#define GREEN   LATAbits.LATA0

// Blink the LED's.
// Both will be OFF when we're done.
void blink(int count) {
    RED = 1;
    GREEN = 1;
    pause(count);
    
    RED = 0;
    GREEN = 0;
    pause(count);
}

int i;
int cyclecount = 10;
temperature_result_t t_old = 0, t_new = 0;

// Main application
void main(void)
{
    SYSTEM_Initialize();    // Initialize everything

    TRISAbits.TRISA0 = 0;   // Set Channel RA0 (GREEN) as output
    TRISAbits.TRISA1 = 0;   // Set Channel RA1 (RED) as output
    
     // blink a few times quickly to let us know we did a reset
    for (i=0; i<5; i++) {
        blink(5000);
    }
        
    //TMR2_StartTimer();      // Start Timer2 for PWM operation
    //PWM_LoadDutyValue(50*4); // some reasonable value
    TEMPERATURE_Start();
    NCO_Start(); // This will preempt RA1 (RED) so it connects to CWG output.

    while (1) {
        // LED D2 Brightness control
        /*multiply duty by 4 in order to scale value for better PWM resolution
          8 bit ADC output is scaled to get a better resolution on 10-bit PWM*/
        
        // waits for conversion!
        //adc =  ADC_GetConversion (channel_AN2) * 4; // function call to read value from Potentiometer
        //PWM_LoadDutyValue (adc); // call function to write value of "duty" to the PWM duty cycle
        
        //adc = ADC_GetConversion (channel_AN2);
        //PR2 = adc;
        
        // note that if you write to TMR2 instead of PR2 then the pre and post scalers
        // are cleared back to 1:1
        
        // Track temperature. 
        t_new = TEMPERATURE_Get();
        if (t_new > t_old) {
            // Temp UP so go RED
            //RED = 1;
            GREEN = 0;
        } else if (t_new < t_old) {
            // Temp DOWN so go GREEN
            //RED = 0;
            GREEN = 1;
        } else { 
            // NO CHANGE
            cyclecount--;
            // no change for 10 cycles -- show temp value on 2 LED's !
            if (cyclecount <= 0) {
                int b;
                uint8_t t = t_new;
                for (b = 0; b < 8; b++) {
                    // MARK
                    //RED = 1;
                    if (t & 0x80) { //show highest bit
                        GREEN = 1;
                    } else {
                        GREEN = 0;
                    }
                    pause(8000);
                    // SPACE
                    //RED = 0;
                    GREEN = 0;
                    pause(7000);
                    t <<= 1; // shift to the left
                }
                cyclecount = 10;
            }
        }
        pause(4000);    // wait a little while before checking again.
        t_old = t_new;
    }
}
// End of File
