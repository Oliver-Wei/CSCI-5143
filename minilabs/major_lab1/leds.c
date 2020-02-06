#include "leds.h"

void initialize_leds() {
  INIT_YELLOW;
  INIT_GREEN;
  INIT_RED;
  INIT_RED_PE6;
  INIT_RED_PB4;
  INIT_YELLOW_PD6;
  INIT_GREEN_PB6;
}

// Syntax for using struct to access registers inspired by
// http://www.avrfreaks.net/forum/io-ports-mapped-struct
void flash_led(IO_struct * color, int inverted) {
  if (!inverted) {
    SET_BIT(*color->port, color->pin);
  } else {
    CLEAR_BIT(*color->port, color->pin);
  }
  _delay_ms(200);
  TOGGLE_BIT(*color->port, color->pin);
  _delay_ms(200);
}

void light_show() {
  // Flash them all to ensure they are working.
  int i;
  for (i = 0; i < 2; i++) {
		flash_led(&_yellow, 0);
		flash_led(&_red, 1);
		flash_led(&_green, 1);
  }
}

void toggle_red(){
  TOGGLE_BIT(*(&_red_pb4)->port, _red_pb4.pin);
}

void toggle_yellow(){
  TOGGLE_BIT(*(&_yellow_pd6)->port, _yellow_pd6.pin);
}

void toggle_green(){
  TOGGLE_BIT(*(&_green_pb6)->port, _green_pb6.pin);
}

