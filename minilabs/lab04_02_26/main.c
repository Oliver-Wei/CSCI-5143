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

volatile uint32_t ms_timer0 = 0;

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

  // Do some setup for the timers.
  SetUpTimerPWM(1,256,2);
  //*************************************************************//
  //*******         THE CYCLIC CONTROL LOOP            **********//
  //*************************************************************//

  // FILL THIS IN TO BLINK LED at the requested frequency using system time.

  // without keyword volatile, the compiler optimizes away count
  DDRB |= (1 << DDB6);
  PORTB |= (1 << PB6);
  sei();
  while(1) {

  } /* end while(1) loop */
} /* end main() */


// ********************* PUT YOUR TIMER ISRs HERE *************** //
// ISR(TIMER0_COMPA_vect) {
//   ++ms_timer0;
//   if (ms_timer0 == release_time0){
//     fRelease = 1;
//     ms_timer0 = 0;
//   }
// }

