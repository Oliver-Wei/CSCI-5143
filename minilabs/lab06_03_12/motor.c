#include "motor.h"


// comment out line if not debugging
#define DEBUG_PCINT

// For setting frequency of timer. Freq calculation based on TOP+1
#define TOP_4kHz 3999

/* PCINT for Encoder was ported from
*  PololuWheelEncoders.cpp
*/

#ifdef DEBUG_PCINT
	volatile uint32_t interrupt_counter = 0;
#endif

volatile int8_t global_m2a;
volatile int8_t global_m2b;

volatile int16_t global_last_m2a_val;
volatile int16_t global_last_m2b_val;

volatile int8_t global_error_m2;
volatile int16_t global_counts_m2 = 0;

// vvvv   FILL IN setuupMotor2   vvvv
// Motor 2 (as seen on AStar pinout) is on OC1B
// This means set up Timer 1 with a PWM mode
// Use OC1RB for the match to control duty cycle.
// This is just like your code to control brightness
void setupMotor2(void) {
	// CAUTION: Please do not run your motors at full speed
	// I advise that you set your duty cycle to 0 here.

	// For wherever you are controlling the speed/duty cycle ...
	// When you first start moving, start with a 20% duty cycle.
	// Put a cap at 60% duty cycle.
	//SetUpTimerPWM(1, 256, motorFrequency);
	// Revise Timer 1 for setting PWM
	// Clear the Control Registers for Timer 1
	TCCR1A = 0;
	TCCR1B = 0;

	// CTC mode is 14 = 1110
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << WGM13);

	TCCR1A |= (1 << COM1B1);

	// Clock Select Bits are bits 00000XXX in TCCR1B
	TCCR1B |= 4; //CSbits;

	// Set the Top as two times of match to manage the frequency
	ICR1 = TOP_4kHz; // The input parameter is calculated as Top
	// Set the duty cycle to be 60%
	OCR1B = TOP_4kHz / 10.0 * 6.0;

	DDRB |= 1<<DDB6;
	// Enable the Interrupt on the OCR1B match
	TIMSK1 |= (1<<OCIE1B);
}

void setupEncoder(void) {

	// Set the encoders as input
	clearBit( chA_control, chA_pin );
	clearBit( chB_control, chB_pin );

	// Enable the interrupts for the 2 encoder channels
	setBit( PCMSK0, chA_INT );
	setBit( PCMSK0, chB_INT );

	// enable PCINT interrupts
	setBit( PCICR, PCIE0 );

	// Powering the encoder through general I/O. This sets signal high to provide power to device.
	setBit( enc_power_control, enc_power_pin );
	setBit( enc_power_output , enc_power_pin );
}

ISR(PCINT0_vect){
	#ifdef DEBUG
		++interrupt_counter;
	#endif

	// Make a copy of the current reading from the encoders
	uint8_t tmpB = PINB;

	// Get value of each channel, making it either a 0 or 1 valued integer
	uint8_t m2a_val = (tmpB & (1 << chA_pin )) >> chA_pin;
	uint8_t m2b_val = (tmpB & (1 << chB_pin )) >> chB_pin;

	// Adding or subtracting counts is determined by how these change between interrupts
	int8_t plus_m2 = m2a_val ^ global_last_m2b_val;
	int8_t minus_m2 = m2b_val ^ global_last_m2a_val;

	// Add or subtract encoder count as appropriate
	if(plus_m2) { global_counts_m2 += 1; }  // 2248.86 (360 degrees)
	if(minus_m2) { global_counts_m2 -= 1; }

	// If both values changed, something went wrong - probably missed a reading
	if(m2a_val != global_last_m2a_val && m2b_val != global_last_m2b_val) {
		global_error_m2 = 1;
	}

	// Save for next interrupt
	global_last_m2a_val = m2a_val;
	global_last_m2b_val = m2b_val;

	// If trying to debug, flash an led so you know the PCINT ISR fired
	#ifdef DEBUG_PCINT
		if (0 == interrupt_counter%20 ) {
			toggleBit( PORTD, PORTD5 );
		}
	#endif
}
