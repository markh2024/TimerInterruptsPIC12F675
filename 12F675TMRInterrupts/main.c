#include <xc.h>

// PIC12F675 Configuration Bit Settings
#pragma config FOSC = INTRCIO    // Internal RC oscillator, GPIO on GP4 and GP5
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF       // Power-Up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // GP3/MCLR pin function select
#pragma config BOREN = OFF       // Brown-out Detect Enable bit (Disabled)
#pragma config CP = OFF          // Code Protection bit (Disabled)
#pragma config CPD = OFF         // Data Code Protection bit (Disabled)

#define _XTAL_FREQ 4000000       // Define clock frequency as 4 MHz
#define GP5_HIGH GPIO |= (1 << 5)  // Set GP5 high
#define GP5_LOW GPIO &= ~(1 << 5)  // Set GP5 low
#define GP3_LOW !(GPIO & (1 << 3)) // Check if GP3 is low

char m_array[10] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x10, 0x11 };
signed char m_counter = 9;       // Start at 9
unsigned char second_counter = 0;  // Counts up to 10 seconds

// Interrupt Service Routine (ISR)
void __interrupt() myISR(void) {
    if (T0IF) {
        TMR0 = 6;              // Reload Timer0 to overflow every 1 second
        T0IF = 0;              // Clear Timer0 interrupt flag

        second_counter++;       // Increment 1-second counter

        if (second_counter == 10) {   // Check if 10 seconds have passed
            second_counter = 0;       // Reset 10-second counter

            // Output current value from m_array to GPIO
            GPIO = m_array[m_counter];

            if (m_counter == 0) {        // If m_counter is 0
                GP5_HIGH;                // Set GP5 high
                INTCONbits.TMR0IE = 0;   // Disable Timer0 interrupt
            } else {
                m_counter--;             // Decrement m_counter
            }
        }
    }
}

// Initialize GPIO and peripherals
void InitCCT(void) {
    ANSEL  = 0x00;              // Set all ports as digital I/O
    ADCON0 = 0x00;              // Shut off the A/D Converter
    CMCON  = 0x07;              // Shut off the Comparator
    VRCON  = 0x00;              // Shut off the Voltage Reference
    TRISIO = 0b00001000;        // Set GP3 as input, GP5 and others as outputs
    OPTION_REG = 0b00000111;    // Prescaler 1:256 for Timer0
    TMR0 = 6;                   // Initialize Timer0 to generate 1-second interrupts
    GPIO = 0x00;                // Initialize all GPIOs to 0
}

// Check GP3 and reset conditions
void checkGP3(void) {
    // Check if GP3 is pulled low
    if (GP3_LOW) {             // If GP3 is low
        GP5_LOW;               // Set GP5 low
        m_counter = 9;         // Reset m_counter to 9
        INTCONbits.TMR0IE = 1; // Re-enable Timer0 interrupt
    }
}

void main(void) {
    InitCCT();                  // Initialize I/O and peripherals

    // Enable Timer0 interrupt and global interrupts
    INTCON = 0b10100000;        // GIE (Global Interrupt Enable) + TMR0IE (Timer0 Interrupt Enable)
    T0IF = 0;                   // Clear Timer0 interrupt flag

    while (1) {
        checkGP3();             // Continuously check the state of GP3
    }

    return;
}
