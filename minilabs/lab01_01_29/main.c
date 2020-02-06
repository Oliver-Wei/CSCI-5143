#ifndef F_CPU
#define F_CPU 16000000UL
#endif

// Comment this to remove "heartbeat" and led flash in PCINT
//#define DEBUG 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "leds.h"
#include "buttons.h"


/****************************************************************************
   ALL INITIALIZATION
****************************************************************************/
void initialize_system(void)
{
	initialize_leds();
	light_show();
  initialize_buttons();
}

/****************************************************************************
   MAIN
****************************************************************************/

int main(void) {
  // This prevents the need to reset after flashing
  USBCON = 0;

	initialize_system();

  // Set up to fire ISR upon button A and C activity
  SetUpButton(&_button_A);
  SetUpButton(&_button_C);

  //*************************************************************//
  //*******         THE CYCLIC CONTROL LOOP            **********//
  //*************************************************************//

  // FILL THIS IN TO BLINK LEDS at the requested frequency given the
  // current state of the system (i.e. if a button was pressed or released)
  // Even if you know how to set up a timer interrupt, do not use it
  // for this assignment. You may use _delay_ms()

  // without keyword volatile, the compiler optimizes away count
  volatile uint32_t count = 0;
  volatile uint32_t time = 0;

  sei();
  while(1) {

    #ifdef DEBUG
    // might be useful to see a heartbeat if in debug mode
    
    ++count;
    if (count == 50000) {
      TOGGLE_BIT(*(&_yellow)->port, _yellow.pin);
      count = 0;
    }
    
    #endif

    // Delay 250 ms for button A second release response
    // Delay 250*5 ms for button C second release response
    if ((button_A_release_time % 3 == 2)||(button_C_release_time % 3 == 2)){
      for (int i=0; i<5; i++){
        fn_release_A();
        _delay_ms(250);
      }
      fn_release_C();
    }

    /*
    // Delay 250 ms for button A second release response
    // Delay 983 ms for button C second release response
    if ((button_A_release_time % 3 == 2)||(button_C_release_time % 3 == 2)){
      if (time % 250 == 0) fn_release_A();
      if (time % 983 == 0) fn_release_C();
      if (time % (250*983) == 0) time = 0;
      _delay_ms(1);
      ++time;
    }
    */

    
  } /* end while(1) loop */
} /* end main() */

    
