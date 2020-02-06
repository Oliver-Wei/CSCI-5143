#ifndef LEDS_H_
#define LEDS_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <util/delay.h>
#include <inttypes.h>

#include "common.h"

#define INIT_YELLOW _yellow = (IO_struct) { &DDRC, &PORTC, 7, &PINC }; \
  DDRC |= (1<<DDC7);
#define INIT_GREEN  _green = (IO_struct) { &DDRD, &PORTD, 5, &PIND }; \
  DDRD |= (1<<DDD5);
#define INIT_RED _red = (IO_struct) { &DDRB, &PORTB, 0, &PINB }; \
  DDRB |= (1<<DDB0);
#define INIT_RED_PE6 _red_pe6 = (IO_struct) { &DDRE, &PORTE, 6, &PINE }; \
  DDRE |= (1<<DDE6);
#define INIT_RED_PB4 _red_pb4 = (IO_struct) { &DDRB, &PORTB, 4, &PINB }; \
  DDRB |= (1<<DDB4);
#define INIT_YELLOW_PD6 _yellow_pd6 = (IO_struct) { &DDRD, &PORTD, 6, &PIND }; \
  DDRD |= (1<<DDD6);
#define INIT_GREEN_PB6 _green_pb6 = (IO_struct) { &DDRB, &PORTB, 6, &PINB }; \
  DDRB |= (1<<DDB6);

IO_struct _yellow;
IO_struct _red;
IO_struct _green;
IO_struct _red_pe6;
IO_struct _red_pb4;
IO_struct _yellow_pd6;
IO_struct _green_pb6;

/* initialize all the on-board leds by calling the macros defined above
 * for initializing the individual on-board leds
 */
void initialize_leds();

/* Flash the designated on-board led for 250ms on, then 250ms off.
 * Assumes led is initialized */
void flash_led(IO_struct * color, int inverted);

/* Flash all the leds for a sanity check light show */
void light_show();

void toggle_red();

void toggle_yellow();

void toggle_green();

#endif
