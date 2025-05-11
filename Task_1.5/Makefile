# Task 1.5 Makefile
MCU      = atmega328p
F_CPU    = 12000000UL
CC       = avr-gcc
OBJCOPY  = avr-objcopy
CFLAGS   = -Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU)

# Default target builds HEX
all: task1.5.hex

# ELF from C source
task1.5.elf: task1.5.c
	$(CC) $(CFLAGS) -o $@ $<

# Intel HEX from ELF
task1.5.hex: task1.5.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Flash via avrdude (GPIO programmer)
flash: task1.5.hex
	avrdude -p m328p -c gpio -e -U flash:w:task1.5.hex

# Clean up
clean:
	rm -f *.elf *.hex
