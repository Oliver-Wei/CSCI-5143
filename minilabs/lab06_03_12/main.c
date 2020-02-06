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
#include "motor.h"

#define TOP_4kHz 3999

extern volatile int16_t global_counts_m2;
extern volatile int8_t global_error_m2;

int Pr = 2248; // Position reference (goal input from the user);
int last_error = 2248;

int max_duty = TOP_4kHz/10.0*9.0;
int min_duty = 0;

int max_T = 2248;
int min_T = 100;

int T;

// int release_time0 = 500;

// volatile uint32_t ms_timer0 = 0;
volatile uint32_t ms_timer1 = 0;

/****************************************************************************
   ALL INITIALIZATION
****************************************************************************/
void initialize_system(void)
{
	initialize_leds();
	light_show();
  initialize_buttons();
}

// void motor_Backward_360(void){
//   OnMotor2();
//   global_counts_m2 = 0;
//   while (global_counts_m2 < 2248) {
//     motorBackward();
//   }
//   _delay_ms(100);
//   OffMotor2();
// }

// void motor_Forward_360(void){
//   OnMotor2();
//   global_counts_m2 = 0;
//   while (global_counts_m2 > 0){
//     motorForward();
//   }
//   _delay_ms(100);
//   OffMotor2();
// }

void PD_control(int kP, int kD, int dt) {
    int Pm = global_counts_m2; // Position measured (from the encoder reading in the ISR);
    int error_change = (Pr-Pm) - last_error;
    last_error = (Pr-Pm);
    // D = error_change / dt;
    // T = kP(Pr-Pm) + kD*(D);
    T = kP*(Pr - Pm);
    // if (T> max_T) OCR1B = max_duty;
    // else if (T < min_T) OCR1B = min_duty;
    // else{
    OCR1B = T;
    // } 
    // if (T > 0){
    //     // direction = backward;
    //     if (T> max_T) OCR1B = max_duty;
    //     else if (T < min_T) OCR1B = min_duty;
    //     // else OCR1B  += T*scale;
    //     else{
    //       OCR1B = T;
    //     } 
    // }
    // else {
    //     // direction = forward;
    //     if (T < -max_T) OCR1B = max_duty;
    //     else if (T > -min_T) OCR1B = min_duty;
    //     else {
    //       OCR1B  += T;
    //     }
    // }
}

/****************************************************************************
   MAIN
****************************************************************************/

int main(void) {
  // This prevents the need to reset after flashing
  USBCON = 0;

  initialize_system();
  // SetUpButton(&_button_A);
  // SetUpButton(&_button_C);

  // SetUpButtonAction(&_button_A, 1, motor_Backward_360);
  // SetUpButtonAction(&_button_C, 1, motor_Forward_360);

  setupEncoder();
  setupMotor2();


  // Do some setup for the timers.
  // SetUpTimerPWM(1,256,2);
  //*************************************************************//
  //*******         THE CYCLIC CONTROL LOOP            **********//
  //*************************************************************//

  // FILL THIS IN TO BLINK LED at the requested frequency using system time.

  // without keyword volatile, the compiler optimizes away count
  // DDRB |= (1 << DDB6);
  // PORTB |= (1 << PB6);
  sei();
  // OffMotor2();
  while(1) {
    // while (global_counts_m2 < 2248) {
    //   OCR1B = TOP_4kHz / 10.0 * 3.0;
    //   motorBackward();
    // }
    // _delay_ms(100);
    // while (global_counts_m2 > 0){
      
    //   motorForward();
    // }
    // _delay_ms(100);
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

ISR(TIMER1_COMPB_vect) {
  ++ms_timer1;
  PD_control(0.5,0,0);
  // if (ms_timer1%200 == 0) OCR1B = TOP_4kHz / 10.0 * 9.0;
  // if (ms_timer1%200 == 100) OCR1B = TOP_4kHz / 10.0 * 3.0;
}

