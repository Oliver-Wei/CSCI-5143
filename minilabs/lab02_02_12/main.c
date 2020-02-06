#ifndef F_CPU
#define F_CPU 16000000UL
#endif

// Comment this to remove "heartbeat" and led flash in PCINT
//#define DEBUG_MAIN 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "common.h"
#include "leds.h"
#include "buttons.h"
#include "timers.h"

int release_time0 = 500;
int release_time1 = 250;

volatile uint32_t ms_timer0 = 0;
volatile uint32_t ms_timer1 = 0;
volatile uint32_t fRelease = 0;

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

  // You can pass a function pointer to implement button behavior.
  // Do this, or change the code in buttons.c to toggle the led
  // SetUpButtonAction(&_button_A, 1, xxx);
  SetUpButtonAction(&_button_A, 1, blink_green);

  // Do some setup for the timers.
  // SetUpTimerCTC(xxx);
  SetUpTimerCTC(0,64,1000);
  SetUpTimerCTC(1,64,1000);
  //SetUpTimerCTC(3,64,1000);
  //*************************************************************//
  //*******         THE CYCLIC CONTROL LOOP            **********//
  //*************************************************************//

  // FILL THIS IN TO BLINK LED at the requested frequency using system time.

  // without keyword volatile, the compiler optimizes away count
  volatile uint32_t count = 0;

  sei();
  while(1) {

    #ifdef DEBUG_MAIN
    // might be useful to see a heartbeat if in debug mode
    ++count;
    if (count == 50000) {
      TOGGLE_BIT(*(&_yellow)->port, _yellow.pin);
      count = 0;
    }
    #endif

    if (fRelease){
      TOGGLE_BIT(*(&_red)->port, _red.pin);
      fRelease = 0;
    }

  } /* end while(1) loop */
} /* end main() */


// ********************* PUT YOUR TIMER ISRs HERE *************** //
ISR(TIMER0_COMPA_vect) {
  ++ms_timer0;
  if (ms_timer0 == release_time0){
    fRelease = 1;
    ms_timer0 = 0;
  }
}

ISR(TIMER1_COMPA_vect) {
  ++ms_timer1;
  if (ms_timer1 >= release_time1) {
    TOGGLE_BIT(*(&_yellow)->port, _yellow.pin);
    ms_timer1 = 0;
  }
}
