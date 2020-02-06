#ifdef VIRTUAL_SERIAL
#include <VirtualSerial.h>
#else
#warning VirtualSerial not defined, USB IO may not work
#define SetupHardware();
#define USB_Mainloop_Handler();
#endif

#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

void handleInput( char c );
void handleCommand( char *command );
void init_usb();
void adc_init();
uint16_t adc_read(uint8_t channel);


