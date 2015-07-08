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
#define RED     LATAbits.LATA0
#define GREEN   LATAbits.LATA1




// Patterns I'd like to hear
//
// BRIGHT
//
//    peep peep .................................... peep ........
//
//    peep peep peep ............ peep peep peep .............
//
//    peep ... peep ... peep  (LOUD)
//
// DARK

int green_led = 0;
int duty_cycle = 3;

void interrupt handle_interrupts() {
    // is this a timer2 interrupt?
    if (TMR2IE && PIR1bits.TMR2IF) {
        // Make duty cycle 25%
        if (green_led < duty_cycle) {
            //GREEN = 0;
            NCO_Disable();
            green_led++;
        } else {
            //GREEN = 1;
            NCO_Enable();
            green_led = 0;
        }
        
        PIR1bits.TMR2IF = 0;
    }
}

// Blink the LED's.
// Both will be OFF when we're done.
void blink(int count) {
    //RED = 1;
    GREEN = 1;
    pause(count);
    
    //RED = 0;
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

    TRISAbits.TRISA0 = 0;   // Set Channel RA0 (RED) as output
    TRISAbits.TRISA1 = 0;   // Set Channel RA1 (GREEN) as output
    TRISAbits.TRISA2 = 0;   // Set Channel RA2 (NCO) as output
    
    // blink a few times quickly to let us know we did a reset
    //for (i=0; i<5; i++) { blink(5000); }

    Timer2_Enable();
    //PWM_LoadDutyValue(50*4); // some reasonable value
    //TEMPERATURE_Start();
    NCO_Enable();
    pause(1000);

    TRISAbits.TRISA0 = 1;   // Set Channel RA0 A/D as photo sensor input
    
    Enable_Interrupts();    

    while (1) {
        // Track amount of light
        t_new = ADC_GetConversion (channel_AN0);
                
        // As light increases, Ropt goes down, so Vopt goes up. I think.
        if (t_new > 128) {
            // Too much light, I am scared!
            Timer2_Disable();
        } else {
            Timer2_Enable();
        }
        
        if (t_new > 80) {
            // stretch out duty cycle when it's bright
            duty_cycle = 6;
            CWG_Disable(); // Get quieter by holding RA1 low.
        } else if (t_new > 64) {
            // stretch out duty cycle when it's bright
            duty_cycle = 4;
            CWG_Enable();  // Get louder by putting complement of RA0 on RA1.
        } else {
            duty_cycle = 2;
            CWG_Enable();  // Get louder by putting complement of RA0 on RA1.
        }
        
        // Track temperature. 
        //t_new = TEMPERATURE_Get();
        
        if (t_new > t_old) {
            // Temp UP so go RED
            //RED = 1;
            //GREEN = 0;
            //NCO_Set(0,50);
            
        } else if (t_new < t_old) {
            // Temp DOWN so go GREEN
            //RED = 0;
            //GREEN = 1;
            //NCO_Set(0, 25);
            
        } else { 
            NCO_Set(1, (255 - t_new));
            Timer2_SetPeriod(t_new);
            
//            for (i=0; i<5; i++) {
//    //            NCO_Set(255);
//                blink(1000);
//      //          NCO_Set(0);
//                //blink(1000);
//            }
//            // NO CHANGE
//            cyclecount--;
//            // no change for 10 cycles -- show temp value on 2 LED's !
//            if (cyclecount <= 0) {
//                int b;
//                uint8_t t = t_new;
//                for (b = 0; b < 8; b++) {
//                    // MARK
//                    //RED = 1;
//                    if (t & 0x80) { //show highest bit
//                        GREEN = 1;
//                    } else {
//                        GREEN = 0;
//                    }
//                    pause(8000);
//                    // SPACE
//                    //RED = 0;
//                    GREEN = 0;
//                    pause(7000);
//                    t <<= 1; // shift to the left
//                }
//                cyclecount = 10;
//            }
        }
        pause(1000);
        t_old = t_new;
    }
}
// End of File
