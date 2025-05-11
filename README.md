```markdown
# Embedded Software Lab – Lab 1 Tasks (1.5, 1.6, 1.7)

This repository contains the implementations for **Task 1.5**, **Task 1.6**, and **Task 1.7** of the Embedded Software Lab,
targeting the ATmega328P (Gertboard) with the AVR-GCC toolchain and a Raspberry Pi host.
Each task builds on the previous one to demonstrate GPIO control, UART-driven configuration, and serial debug output.

---

## 🔧 Build & Flash

Each task subfolder contains its own `Makefile`. From inside that folder (or root if you placed the `Makefile` there), run:

```bash
make         # builds <task>.hex
make flash   # flashes <task>.hex to the ATmega328P via avrdude
````

**Makefile variables** (adjustable in each Makefile):

* `MCU      = atmega328p`
* `F_CPU    = 12000000UL`
* `CC       = avr-gcc`
* `OBJCOPY  = avr-objcopy`
* `CFLAGS   = -Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU)`

**Flashing command**:

```bash
avrdude -p m328p -c gpio -e -U flash:w:<task>.hex
```

---

## 🛠 Platform & Tools

* **Target MCU:** ATMega328P (3.3V, 12 MHz)
* **Dev Platform:** Raspberry Pi with Gertboard
* **Programming Interface:** GPIO-based SPI via `avrdude`
* **UART Interface:** GPIO 14 (TXD), GPIO 15 (RXD)
* **Toolchain:**

  * `avr-gcc` – AVR cross-compiler
  * `avr-objcopy` – Convert ELF to HEX
  * `avrdude` – Upload HEX via GPIO
  * `minicom` – Serial terminal

---

## 📋 Tasks Overview

### Task 1.5: Sequential LED Control

* **Objective**
  Light twelve LEDs (D1–D12) one at a time in a forward then reverse sequence.
* **GPIO Setup**

  * `DDRB = 0x3F` → PB0–PB5 drive LEDs D1–D6
  * `DDRD = 0xFC` → PD2–PD7 drive LEDs D7–D12
* **Phases**

  1. Light PD2→PD7 in ascending order
  2. Light PB0→PB5 in ascending order
  3. Light PB5→PB0 in descending order
  4. Light PD7→PD2 in descending order
* **Timing**
  Fixed 200 ms delay between each step using `_delay_ms(200)`.

---

### Task 1.6: UART-Controlled Blink Speed

* **Objective**
  Same LED sequence as Task 1.5, but allow the user to change the blink delay at runtime via UART commands.
* **UART Initialization**

  * Baud rate: 9600 bps
  * UBRR0 = 77 for F\_CPU=12 MHz
  * 8 data bits, no parity, 1 stop bit
* **Command Handling**

  * Receive `'1'` → set blink delay to 200 ms
  * Receive `'2'` → set blink delay to 1000 ms
  * Receive `'3'` → set blink delay to 2000 ms
  * Invalid inputs are ignored
* **Safe Delay**
  Implements `wait_delay(ms)` to split long delays into 100 ms chunks for reliability.

---

### Task 1.7: Serial Debug Output

* **Objective**
  Extend Task 1.6 by echoing received UART commands and printing the currently selected blink speed to the serial console.
* **Debug Messages**

  * Upon reception: print `Received: <char>`
  * After adjusting speed: print `Speed = <value>ms`
* **LED Logic**
  Identical to Task 1.6, preserving dynamic speed control.


```
```
