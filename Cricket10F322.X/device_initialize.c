

// Configuration Bits
#pragma config FOSC = INTOSC    // Oscillator Selection bits (INTOSC oscillator: CLKIN function disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)
#pragma config LPBOR = OFF      // Brown-out Reset Selection bits (BOR disabled)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)

#include "device_initialize.h"

void SYSTEM_Initialize(void)
{
    OSCILLATOR_Initialize();
    //PWM_Initialize();
    Timer2_Initialize();
    NCO_Initialize();
    //TEMPERATURE_Initialize();
}

void OSCILLATOR_Initialize(void)
{
    // IRCF INTOSC;
    //OSCCON = 0x60; // 8MHz clock
    OSCCON = 0x50; // 4Mhz clock
    //OSCCON = 0x01; // 250 kHz clock
}

void Enable_Interrupts(void) {
    // enable interrupts from TIMER 2
    PIE1bits.TMR2IE = 1;
    
    // Global
    INTCONbits.GIE = 1;
    
    // Peripherals
    INTCONbits.PEIE = 1;
}

// End of File


