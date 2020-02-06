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
int ms_period3 = 100;

volatile uint32_t ms_period = 2;
volatile uint32_t duty = 20;

/****************************************************************************
   ALL INITIALIZATION
****************************************************************************/
void initialize_system(void)
{
	initialize_leds();
	light_show();
  initialize_buttons();
}

void change_toggle_rate(void)
{
  ms_period = ms_period*2;
  if (ms_period > 16) ms_period = 2;
  SetUpTimerPWM(1,256,ms_period);
}

void change_brightness(void)
{
  duty += 10;
  if (duty > 100) duty = 10;
  OCR3A = 16000000/256/ms_period3/100*duty;
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

  // Do some setup for the timers.
  // SetUpTimerPWM(1,256,ms_period);
  
  //*************************************************************//
  //*******         THE CYCLIC CONTROL LOOP            **********//
  //*************************************************************//

  // FILL THIS IN TO BLINK LED at the requested frequency using system time.
  SetUpTimerPWM(1,256,ms_period);
  SetUpTimerPWM(3,256,ms_period3);

  // Set up buttion A and C activity
  SetUpButtonAction(&_button_A, 1, change_toggle_rate);
  SetUpButtonAction(&_button_C, 1, change_brightness);

  // Enable PB6 and PC6
  DDRB |= (1 << DDB6);
  PORTB |= (1 << PB6);
  DDRC |= (1 << DDC6);
  PORTC |= (1 << PC6);

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

