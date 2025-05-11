#include <avr/io.h>          // Include AVR register definitions
#include <util/delay.h>      // Include delay function

#define F_CPU 12000000UL     // Define the CPU frequency as 12 MHz for _delay_ms()

int main(void) {
    // Configure direction of data ports:
    DDRB = 0x3F;             // Set PB0–PB5 as output (binary 0011 1111), used for LEDs D1–D6
    DDRD = 0xFC;             // Set PD2–PD7 as output (binary 1111 1100), used for LEDs D7–D12

    while (1) {
        // ===== Phase 1: Turn on LEDs connected to PORTD (PD2–PD7) one by one =====
        for (uint8_t i = 2; i <= 7; i++) {
            PORTD = (1 << i);  // Set bit i high on PORTD (turn on one LED)
            PORTB = 0;         // Make sure all PORTB LEDs are off
            _delay_ms(200);    // Wait 200 milliseconds
        }

        // ===== Phase 2: Turn on LEDs connected to PORTB (PB0–PB5) one by one =====
        for (uint8_t i = 0; i < 6; i++) {
            PORTB = (1 << i);  // Set bit i high on PORTB (bitwise left shift)
            PORTD = 0;         // Make sure all PORTD LEDs are off
            _delay_ms(200);    // Wait 200 milliseconds
        }

        // ===== Phase 3: Turn off LEDs on PORTB in reverse order (PB5 → PB0) =====
        for (int8_t i = 5; i >= 0; i--) {
            PORTB = (1 << i);  // Light the LED briefly before moving to next
            PORTD = 0;
            _delay_ms(200);
        }

        // ===== Phase 4: Turn off LEDs on PORTD in reverse order (PD7 → PD2) =====
        for (int8_t i = 7; i >= 2; i--) {
            PORTD = (1 << i);  // Light the LED briefly in reverse order
            PORTB = 0;
            _delay_ms(200);
        }
    }
}