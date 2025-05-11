# Task 1.6 Makefile
MCU      = atmega328p
F_CPU    = 12000000UL
CC       = avr-gcc
OBJCOPY  = avr-objcopy
CFLAGS   = -Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU)

all: task1.6.hex

task1.6.elf: task1.6.c
	$(CC) $(CFLAGS) -o $@ $<

task1.6.hex: task1.6.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: task1.6.hex
	avrdude -p m328p -c gpio -e -U flash:w:task1.6.hex

clean:
	rm -f *.elf *.hex
