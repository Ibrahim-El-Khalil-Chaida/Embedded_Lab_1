// Include the header file for I/O register definitions
#include <avr/io.h>

// Include the delay function definitions
#include <util/delay.h>

// Define the CPU clock frequency used by _delay_ms()
#define F_CPU 12000000UL  // 12 MHz clock speed

// ==================== UART Initialization ====================
void uart_init(void) {
    uint16_t ubrr = 77;              // Calculate UBRR value for 9600 baud with 12MHz clock(USART Baud Rate Register) 77=(F_cpu/ (16 * baud rate)) - 1 
    UBRR0H = (ubrr >> 8);            // Set the high byte of the UBRR register (8 bit LSB)
    UBRR0L = ubrr;                   // Set the low byte of the UBRR register (4 bit MSB)
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Enable both Receiver and Transmitter in UART
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Set frame format: 8 data bits, 1 stop bit, no parity
}

// ==================== UART Receive ====================
char uart_receive(void) {
    while (!(UCSR0A & (1 << RXC0))); // Wait until data is received (RXC0 flag is set) (USART Control and Status Register A)
    return UDR0;                     // Read the received byte from UART Data Register
}

// ==================== UART Transmit ====================
void uart_transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0))); // Wait until the buffer is empty (UDRE0 flag is set) 
    UDR0 = data;                      // Write the data byte to UART Data Register
}

// ==================== UART Print String ====================
void uart_print(const char* str) {
    while (*str) uart_transmit(*str++); // Transmit each character of the string until null terminator
}

// ==================== Delay Function Wrapper ====================
void wait_delay(uint16_t delay_ms) {
    for (uint16_t i = 0; i < delay_ms / 100; i++) {
        _delay_ms(100); // Delay in 100 millisecond chunks (safe with _delay_ms())
    }
}

// ==================== Main Function ====================
int main(void) {
    DDRB = 0x3F;  // Configure PB0 to PB5 as output (0b00111111) for LEDs D1-D6 (Data Direction Register for Port B)
    DDRD = 0xFC;  // Configure PD2 to PD7 as output (0b11111100) for LEDs D7-D12 (Data Direction Register for Port D)

    uart_init();  // Initialize UART communication

    uint16_t delay_ms = 200;  // Initial LED blink delay set to 200 milliseconds

    // Send a welcome message through UART terminal (minicom)
    uart_print("System ready. Send 1 (200ms), 2 (1s), or 3 (2s)\r\n");

    while (1) {
        // ===== UART INPUT HANDLING =====
        if (UCSR0A & (1 << RXC0)) {     // Check if a byte has been received via UART
            char c = uart_receive();    // Read the received byte

            uart_print("Received: ");   // Echo back the received value
            uart_transmit(c);
            uart_print("\r\n");

            // Change LED speed depending on the received input
            if (c == '1') {
                delay_ms = 200;
                uart_print("Speed set to 200ms\r\n");
            } else if (c == '2') {
                delay_ms = 1000;
                uart_print("Speed set to 1 second\r\n");
            } else if (c == '3') {
                delay_ms = 2000;
                uart_print("Speed set to 2 seconds\r\n");
            } else {
                uart_print("Invalid input. Use 1, 2, or 3.\r\n");
            }
        }

        // ===== LED SEQUENCE: ONWARD =====
        for (uint8_t i = 2; i <= 7; i++) {
            PORTD = (1 << i);  // Turn on one LED at PD2–PD7
            PORTB = 0;         // Ensure PORTB LEDs are OFF
            wait_delay(delay_ms);
        }

        for (uint8_t i = 0; i < 6; i++) {
            PORTB = (1 << i);  // Turn on one LED at PB0–PB5
            PORTD = 0;         // Ensure PORTD LEDs are OFF
            wait_delay(delay_ms);
        }

        // ===== LED SEQUENCE: REVERSE =====
        for (int8_t i = 5; i >= 0; i--) {
            PORTB = (1 << i);  // Turn on LEDs in reverse on PORTB
            PORTD = 0;
            wait_delay(delay_ms);
        }

        for (int8_t i = 7; i >= 2; i--) {
            PORTD = (1 << i);  // Turn on LEDs in reverse on PORTD
            PORTB = 0;
            wait_delay(delay_ms);
        }
    }
}
