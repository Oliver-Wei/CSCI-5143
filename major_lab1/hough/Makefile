PORT ?= /dev/tty.usbmodem1411
MCU=atmega32u4
CFLAGS= -g -Wall -mcall-prologues -mmcu=$(MCU) -O1 -v
LDFLAGS= -Wl,-gc-sections -Wl,-relax
CC=avr-gcc
TARGET=main
OBJECT_FILES=main.o hough.o

all: $(TARGET).hex

clean:
	rm -f *.o *.hex

%.hex: %.obj
	avr-objcopy -R .eeprom -O ihex $< $@

%.obj: $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@

program: $(TARGET).hex
	avrdude -p $(MCU) -c avr109 -P $(PORT) -U flash:w:$(TARGET).hex
