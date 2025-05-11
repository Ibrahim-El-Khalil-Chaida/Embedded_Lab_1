MCU=atmega328p
F_CPU=12000000UL
CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU)

all: task1.7.hex

task1.7.elf: *.c
	$(CC) $(CFLAGS) -o $@ $<

task1.7.hex: task1.7.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: task1.7.hex
	sudo avrdude -p m328p -c gpio -e -U flash:w:task1.7.hex

clean:
	rm -f *.elf *.hex
