// Cricket 10F322
// based on MPLAB development board code
// Brian Wilson
// 2015-Jun-25

#include "device_initialize.h"

#define MS 38
// This is calibrated to be about a millisecond per count
void tick(int count) {
    int a,b,c;
    for (a=0; a<count; a++) {
        for (b=0; b<MS; b++) {
            c = a + b; // prevent loop from being optimized away
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
int duty_cycle = 0;  // 1 is 50%
int slow_start = 0; // delay when going from too_bright state to noisy state

// Each time any interrupt happens, PIC jumps to this function.
// It has to check to see which interrupt happened and then do
// whatever is needed to handle that interrupt.
// Then it has to clear the interrupt flag and exit.
void interrupt handle_interrupts() {
    // is this a timer2 interrupt?
    if (TMR2IE && PIR1bits.TMR2IF) {
        if (duty_cycle == 0 || slow_start > 0) {
            NCO_Disable(); 
        } else if (duty_count < duty_cycle) {
            NCO_Disable(); // Turn off sound output
            duty_count++;
        } else {
            NCO_Enable();  // Turn on sound output
            duty_count = 0;
        }
        
        PIR1bits.TMR2IF = 0; // Clear the interrupt flag.
    }
}

// A finite state machine is used to track whether the cricket
// has been in a given state a long time. 
// If it has, then the cricket falls asleep. 
// When state changes then it wakes up again

enum states {
    startup = 0,
    too_bright = 1,
    bright = 2,
    shady = 3,
    dark = 4
} current_state = startup, new_state = startup;

int sleeping = 0; // either sleeping or not
#define SLEEP_COUNT 4 * 60 * 10 // time in 1/4 seconds to stay awake in one state
int sleep_timer = SLEEP_COUNT;

int i,j;
uint8_t photo = 0;

#define TESTING_SENSOR 0

// Pitch settings
#define HIGH_PITCH  2100  // 4k
#define MED_PITCH   1050  // 1900 Hz
#define LOW_PITCH    500  // 950 Hz

#define BASE_PITCH   800

// Photoresistor thresholds
#define BRIGHT       192
#define SHADY        128
#define DARK          64

// Timer2 period settings
#define T2_BASE      180

#define PITCH_SWEEP 1
#if PITCH_SWEEP
void pitch_sweep() {
    for (i = LOW_PITCH; i<HIGH_PITCH; i += 20) {
        NCO_SetPitch(i);
        tick(3);
    }
}
#endif

#define PLAY_NOTES 0
#if PLAY_NOTES
void play_note(int pitch, int duration) {
    NCO_SetPitch(pitch);
    tick(duration);
}
#endif

// Main application
void main(void)
{
    SYSTEM_Initialize();  // Initialize everything

    TRISAbits.TRISA0 = 0; // Set Channel RA0 (photo sensor) as output during init
    TRISAbits.TRISA1 = 0; // Set Channel RA1 (CWG) as output
    TRISAbits.TRISA2 = 0; // Set Channel RA2 (NCO) as output

    // calibrate the pause, going for one tick per millisecond
    // note I am using RA0 as output here so timers dont confuse things
#if defined(TEST_TICK)
    for (i = 0; i < 1000; i++) {
        LATAbits.LATA0 = 1; // on for 1 ms
        tick(1);
        LATAbits.LATA0 = 0; // off for 1 ms
        tick(1);
    }
#endif
    
    NCO_Enable(); // Turn on main oscillator
    CWG_Enable(); // Turn on complementary waveform
    
   
#if PITCH_SWEEP
    pitch_sweep();
    //pitch_sweep();
#endif
#if PLAY_NOTES
//    play_note(0,250);
//    play_note(500, 300);
    play_note(800, 500);
//    play_note(500, 400);
//6                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       play_note(800, 500);
    play_note(950, 600);
//    play_note(0,1000);
#endif
    
#if TESTING_SENSOR==2
    Timer2_Disable();       // only emit continuous tones in test mode
#else
    Timer2_SetPeriod(T2_BASE);
    Timer2_Enable();        // chirp when enabled instead of continuous tone
#endif
    ADC_Initialize();       // Turn on A/D converter
    ADC_Enable();           // Enable photo sensor input
    //TEMPERATURE_Start();
    
    Enable_Interrupts();
    
    while(1) {
        // Get amount of light from photoresistor
        photo = ADC_GetConversion(channel_AN0);
        
#if TESTING_SENSOR
        new_state = dark; // for testing force into a fixed state
#else
        if (photo > BRIGHT) {
            new_state = too_bright;
        } else if (photo > SHADY) {
            new_state = bright;
        } else if (photo > DARK) {
            new_state = shady;
        } else {
            new_state = dark;        
        }
#endif // TESTING_SENSOR

        if (!sleeping){
            
            duty_cycle = photo>>4 + slow_start;
            if (slow_start<10)
            switch (new_state) {
                case too_bright:
                default:
                    // Be quiet
                    duty_cycle = 0;
                    NCO_Disable();
                    CWG_Disable();
                    slow_start = 20;
                    break;
                case bright:
                    Timer2_SetPeriod(T2_BASE - 128 + photo>>2);
                    duty_cycle += 32;
                    NCO_Enable();
                    CWG_Enable(); // Get louder
                    break;
                case shady:
                    duty_cycle += 10;
                    NCO_Enable();
                    CWG_Enable();  // Get louder by putting complement of RA0 on RA1.
                    Timer2_SetPeriod(T2_BASE + photo); // varied period of duty cycle
                    break;
                case dark:
                    //duty_cycle += 2;
                    NCO_Enable();
                    CWG_Enable();  // Get louder by putting complement of RA0 on RA1.
                    Timer2_SetPeriod(T2_BASE + photo); // varies duty cycle period
                    break;
            }
        }
        
        if (slow_start)
            slow_start--;
        
        // pitch varies with lighting, darker = higher
        // As light increases, Ropt goes down, so Vopt goes up. I think.
        NCO_SetPitch(BASE_PITCH + (255 - photo)>>1);
        
        if (new_state != current_state) {
            
            sleeping = 0; // state change always wakes us up
            sleep_timer = SLEEP_COUNT; // reset the counter
            
            current_state = new_state;
            
        } else if (!sleeping) {
            // We are in a steady state, is it time to sleep yet???
            if (sleep_timer <= 0) {
                //pitch_sweep();
                //tick(500);
                sleeping = 1;
                duty_cycle = 0;
                NCO_Disable(); 
            } else {
                sleep_timer--;
            }
        }

        tick(100 + photo); // wait 1/4 a second or so before responding to changes
    }
}
// End of File
