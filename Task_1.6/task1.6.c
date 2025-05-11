/*
 * task1.6.c
 * Task 1.6: Change LED blink speed via UART input
 * Target: ATmega328P @ 12 MHz on Gertboard
 */

#define F_CPU 12000000UL  // CPU clock frequency for _delay_ms()
#include <avr/io.h>       // AVR I/O register definitions
#include <util/delay.h>   // _delay_ms()

/**
 * Initialize UART at 9600 baud.
 * UBRR0 = (F_CPU / (16 * BAUD)) - 1
 * For 12 MHz and 9600 baud, UBRR0 = 77.
 */
void uart_init(void) {
    uint16_t ubrr = 77;
    UBRR0H = (ubrr >> 8) & 0xFF;              // High byte of baud rate
    UBRR0L = ubrr & 0xFF;                     // Low byte of baud rate
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);     // Enable RX and TX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);   // 8 data bits, 1 stop bit
}

/**
 * Blocking receive of one character over UART.
 * Waits until RXC0 (Receive Complete) flag is set.
 */
char uart_receive(void) {
    while (!(UCSR0A & (1 << RXC0))) {
        // spin until a byte arrives
    }
    return UDR0;  // Read received byte
}

/**
 * Delay wrapper that safely handles delays longer than _delay_ms() limit.
 * Splits the total ms into 100 ms chunks.
 */
void wait_delay(uint16_t ms) {
    while (ms >= 100) {
        _delay_ms(100);
        ms -= 100;
    }
    if (ms > 0) {
        _delay_ms(ms);
    }
}

int main(void) {
    // ---- Configure LED pins as outputs ----
    DDRB = 0x3F;  // PB0–PB5 (bits 0–5) for LEDs D1–D6
    DDRD = 0xFC;  // PD2–PD7 (bits 2–7) for LEDs D7–D12

    uart_init();           // Set up UART
    uint16_t delay_ms = 200; // Default blink delay: 200 ms

    while (1) {
        // ---- Check for UART input to adjust speed ----
        if (UCSR0A & (1 << RXC0)) {  // If a byte has been received
            char c = uart_receive(); // Read it
            if (c == '1') {
                delay_ms = 200;      // 200 ms
            } else if (c == '2') {
                delay_ms = 1000;     // 1 s
            } else if (c == '3') {
                delay_ms = 2000;     // 2 s
            }
            // Invalid inputs are simply ignored
        }

        // ---- Forward LED sequence ----
        // Light PD2→PD7 one by one
        for (uint8_t i = 2; i <= 7; ++i) {
            PORTD = (1 << i);  // Turn on one PD pin
            PORTB = 0;         // Ensure PB LEDs are off
            wait_delay(delay_ms);
        }
        // Light PB0→PB5 one by one
        for (uint8_t i = 0; i < 6; ++i) {
            PORTB = (1 << i);  // Turn on one PB pin
            PORTD = 0;         // Ensure PD LEDs are off
            wait_delay(delay_ms);
        }

        // ---- Reverse LED sequence ----
        // Light PB5→PB0
        for (int8_t i = 5; i >= 0; --i) {
            PORTB = (1 << i);
            PORTD = 0;
            wait_delay(delay_ms);
        }
        // Light PD7→PD2
        for (int8_t i = 7; i >= 2; --i) {
            PORTD = (1 << i);
            PORTB = 0;
            wait_delay(delay_ms);
        }
    }

    return 0;  // never reached
}