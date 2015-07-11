// Cricket 10F322
// based on MPLAB development board code
// Brian Wilson
// 2015-Jun-25

#include "device_initialize.h"

uint16_t adc; // define variable - unsigned integer


// Just hang around a little while
void pause(int count) {
    int a,b;
    for (a=0; a<count; a++) {
        for (b=0; b<10; b++) {
        
        }
    }
}

// Patterns I'd like to hear
//
// BRIGHT
//    really bright is quiet
//    peep peep .................................... peep ........
//    peep peep peep ............ peep peep peep .............
//    peep ... peep ... peep  (LOUDEST)
// DARK

int duty_count = 0;
int duty_cycle = 3;

void interrupt handle_interrupts() {
    // is this a timer2 interrupt?
    if (TMR2IE && PIR1bits.TMR2IF) {
        if (duty_count < duty_cycle) {
            NCO_Disable();
            duty_count++;
        } else {
            NCO_Enable();
            duty_count = 0;
        }
        
        PIR1bits.TMR2IF = 0;
    }
}

int i,j;
int cyclecount = 10;
temperature_result_t t_old = 0, t_new = 0;

#define TESTING_SENSOR 0

// Pitch settings
#define HIGH_PITCH  2100  // 4k
#define MED_PITCH   1050  // 1900 Hz
#define LOW_PITCH    500  // 950 Hz

#define BASE_PITCH   800

// Sensor settings
#define TOO_BRIGHT   192
#define BRIGHT       128
#define SHADY         64

// Timer2 period settings
#define T2_BASE       128

// Main application
void main(void)
{
    SYSTEM_Initialize();  // Initialize everything

    TRISAbits.TRISA0 = 0; // Set Channel RA0 (photo sensor) as output during init
    TRISAbits.TRISA1 = 0; // Set Channel RA1 (CWG) as output
    TRISAbits.TRISA2 = 0; // Set Channel RA2 (NCO) as output

    NCO_Enable(); // Turn on main oscillator
    CWG_Enable(); // Turn on complementary waveform
    
    // Pitch sweep at startup
    for (i = LOW_PITCH; i<HIGH_PITCH; i += 25) {
        NCO_SetPitch(i);
        pause(45);
    }

#if TESTING_SENSOR
    Timer2_Disable();       // only emit continuous tones in test mode
#else
    Timer2_SetPeriod(T2_BASE);
    Timer2_Enable();        // chirp when enabled instead of continuous tone
#endif
    ADC_Enable();           // Enable photo sensor input
    //TEMPERATURE_Start();
    
    Enable_Interrupts();    

    while (1) {
        // Track amount of light
        t_new = ADC_GetConversion(channel_AN0);
        // Note -- more light = lower number 
        
        // Track temperature. 
        //t_new = TEMPERATURE_Get();
        
#if TESTING_SENSOR
            // Trying to find 2 good break points 
        if (t_new > TOO_BRIGHT) {
            NCO_Disable();
            CWG_Disable();
        } else if (t_new > BRIGHT) {
            NCO_Enable();
            NCO_SetPitch(HIGH_PITCH);
            CWG_Disable();
        } else if (t_new > SHADY) {
            NCO_Enable();
            NCO_SetPitch(MED_PITCH);
            CWG_Enable();
        } else { // DARK
            NCO_Enable();
            NCO_SetPitch(LOW_PITCH);
            CWG_Enable();
        }
        CWG_Enable(); // loud is acceptable
#else
        // As light increases, Ropt goes down, so Vopt goes up. I think.
            if (t_new > TOO_BRIGHT) {
                // Too much light, I am scared!
                Timer2_Disable(); // continuous tone
                NCO_SetPitch(HIGH_PITCH); pause(1000); // I am scared peep
                NCO_Disable(); // Turn off oscillator output
                pause(3000); // wait longer before responding to changes
                Timer2_Enable(); // chirp when enabled instead of continuous tone
            } else {
                NCO_Enable();

                if (t_new > BRIGHT) {
                    // stretch out duty cycle when it's bright
                    duty_cycle = 256;
                    CWG_Disable(); // Get quieter by holding CWG low.
                    Timer2_SetPeriod(T2_BASE); // how long speaker is on
                } else if (t_new > SHADY) {
                    // stretch out duty cycle when it's bright
                    duty_cycle = 128;
                    CWG_Enable();  // Get louder by putting complement of RA0 on RA1.
                    Timer2_SetPeriod(T2_BASE - t_new); // how long speaker is on
                } else 
                {
                    duty_cycle = 64;
                    CWG_Enable();  // Get louder by putting complement of RA0 on RA1.
                    Timer2_SetPeriod(T2_BASE - t_new); // how long speaker is on
                }

    //        if (t_new > t_old) {
    //            NCO_Set(2, 50);
    //            
    //        } else if (t_new < t_old) {
    //            NCO_Set(2, 25);

                // pitch drops when it gets brighter
                NCO_SetPitch(BASE_PITCH + 255 - t_new);
            }
#endif // TESTING_SENSOR

        t_old = t_new;
        pause(300); // wait before responding to changes
    }
}
// End of File
