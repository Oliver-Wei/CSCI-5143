#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "timers.h"
#include "leds.h"

// int period = 100;
/* Set up a timer using ctc mode.
 * parameter [in] timer_num : 0 (8-bit) or 1,3 (16-bit)
 * parameter [in] prescaler : 1,8,64, ... as indicated in datasheet options
 * parameter [in] ms_period : desired period of ISR
 * parameter [out] : -1 for error in setting up. 1 otherwise.
 */
int SetUpTimerCTC( int timer_num, int prescaler, int ms_period ) {
  if ((0 != timer_num) && ((1 != timer_num) && (3 != timer_num))) {
    return ERROR;
  }

  // determine the match value for the desired period given prescaler
  // @TODO check that this is an integer value -- if not give warning.
  // ********************************************************************* //
  // ******** MODIFY THIS LINE. Use the input parameters.  *************** //
  // ********************************************************************* //
  // 65536
  uint32_t match32 = ms_period*16000/prescaler; // 16000/64

  // Check that the match fits in its counter register (8-bit or 16-bit)
  if ((0 == timer_num) && (match32 > 255)) {
    return -1;
  } else if (match32 > 65535) {
    return ERROR;
  }

  // Get the clock-select bits for the chosen prescaler
  char CSbits = GetCSBits(prescaler);

  switch(timer_num) {
    case(0) : return SetUpTimer_0(CSbits, (uint8_t) match32); break;
    case(1) : return SetUpTimer_1(CSbits, (uint16_t) match32); break;
    case(3) : return SetUpTimer_3(CSbits, (uint16_t) match32); break;
    default : return ERROR;
  }
  return 1;
}

int SetUpTimerPWM( int timer_num, int prescaler, int ms_period ) {
  if ((0 != timer_num) && ((1 != timer_num) && (3 != timer_num))) {
    return ERROR;
  }

  // determine the match value for the desired period given prescaler
  // @TODO check that this is an integer value -- if not give warning.
  // ********************************************************************* //
  // ******** MODIFY THIS LINE. Use the input parameters.  *************** //
  // ********************************************************************* //
  // 62500
  uint32_t top32 = 16000000/prescaler/ms_period;

  // Check that the match fits in its counter register (8-bit or 16-bit)
  if ((0 == timer_num) && (top32 > 255)) {
    return -1;
  } else if (top32 > 65535) {
    return ERROR;
  }

  // Get the clock-select bits for the chosen prescaler
  char CSbits = GetCSBits(prescaler);

  switch(timer_num) {
    case(0) : return SetUpTimer_0(CSbits, (uint8_t) top32); break;
    case(1) : return SetUpTimer_1(CSbits, (uint16_t) top32); break;
    case(3) : return SetUpTimer_3(CSbits, (uint16_t) top32); break;
    default : return ERROR;
  }
  return 1;
}

int SetUpTimer_0(char CSbits, uint8_t match) {

  // Clear the Control Registers for Timer 0
  TCCR0A = 0;
  TCCR0B = 0;

  // CTC mode is 2 = 010  
  TCCR0A |= (1 << WGM01);
  // Clock Select Bits are bits 00000XXX in TCCR0B   
  TCCR0B |= CSbits;
  // CTC uses OCR0A as top / match   match
  OCR0A = match;
  // Enable the Interrupt on the OCR0A match   
  TIMSK0 |= (1<<OCIE0A);
  return 1;
}


// int SetUpTimer_1(char CSbits, uint16_t match) {
//   // ********      FILL THIS IN **************************//
//   // Clear the Control Registers for Timer 1
//   TCCR1A = 0;
//   TCCR1B = 0;

//   // CTC mode is 4 = 100
//   TCCR1B |= (1 << WGM12);
//   // Clock Select Bits are bits 00000XXX in TCCR1B   
//   TCCR1B |= CSbits;
//   // CTC uses OCR1A as top / match   match
//   OCR1A = match;
//   // Enable the Interrupt on the OCR1A match   
//   TIMSK1 |= (1<<OCIE1A);
//   return 1;
// }

int SetUpTimer_1(char CSbits, uint16_t top) {
  // ********      FILL THIS IN **************************//
  // Clear the Control Registers for Timer 1
  TCCR1A = 0;
  TCCR1B = 0;
  
  // PWM mode is 14 = 1110
  TCCR1A |= (1 << WGM11);
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << WGM13);
  // Clock Select Bits are bits 00000XXX in TCCR1B   
  TCCR1B |= CSbits;
  // COM bits for OCnA/B (COMnA/B:10) PWM : typically want "10"
  TCCR1A |= (1 << COM1B1);
  // Set ICR (TOP) to control frequency
  ICR1 = top;
  // Set OCR1B for 50% duty cycle
  OCR1B = top/2;
  // Enable the Interrupt on the OCR1A match   
  TIMSK1 |= (1<<OCIE1A);
  return 1;
}

// int SetUpTimer_3(char CSbits, uint16_t top) {
//   // ********      FILL THIS IN **************************//
//   // Clear the Control Registers for Timer 3
//   TCCR3A = 0;
//   TCCR3B = 0;

//   // PWM mode is 14 = 1110
//   TCCR3A |= (1 << WGM31);
//   TCCR3B |= (1 << WGM32);
//   TCCR3B |= (1 << WGM33);
//   // Clock Select Bits are bits 00000XXX in TCCR3B   
//   TCCR3B |= CSbits;
//   // COM bits for OCnA/B (COMnA/B:10) PWM : typically want "10"
//   TCCR3A |= (1 << COM3A1);
//   // Set ICR (TOP) to control frequency
//   ICR3 = top;
//   // Set OCR3A for duty cycle
//   OCR3A = top/2;
//   // Enable the Interrupt on the OCR3A match   
//   TIMSK3 |= (1<<OCIE3A);
//   return 1;
// }

int SetUpTimer_3(char CSbits, uint16_t match) {
  // ********      FILL THIS IN **************************//
  // Clear the Control Registers for Timer 3
  TCCR3A = 0;
  TCCR3B = 0;

  // CTC mode is 4 = 100 
  TCCR3B |= (1 << WGM32);
  // Clock Select Bits are bits 00000XXX in TCCR3B   
  TCCR3B |= CSbits;
  // CTC uses OCR3A as top / match   match
  OCR3A = match;
  // Enable the Interrupt on the OCR3A match   
  TIMSK3 |= (1<<OCIE3A);
  return 1;
}



char GetCSBits(int prescaler) {
  // homegrown dictionary.
  // key: prescaler. value: clock select bits value
  switch(prescaler) {
    case(1) : return 1; break;
    case(8) : return 2; break;
    case(64) : return 3; break;
    case(256) : return 4; break;
    case(1024) : return 5; break;
    default : return 0;
  }
}
