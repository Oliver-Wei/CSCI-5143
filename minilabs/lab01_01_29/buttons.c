#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "buttons.h"
#include "leds.h"

// comment this line out when you are done debugging
//#define DEBUG


void EmptyFunction() {}

void initialize_buttons() {

  // Declare the data structures for the buttons.
  INIT_BUTTONA;
  INIT_BUTTONC;

  // initialize the button function pointers with a function that does nothing.
  // This way, you can always have a call to all of these functions in your
  // code, even if the system you are designing doesn't use some button .
  fn_release_A = EmptyFunction;
  fn_press_A = EmptyFunction;
  fn_release_C = EmptyFunction;
  fn_press_C = EmptyFunction;
}

/* Set up either of the two buttons on the board.
* parameter [in] IO_struct button : for A or C as defined above
* parameter [out] : -1 for error in setting up. 1 otherwise.
*/
int SetUpButton(IO_struct * button) {

  // Configure the data direction to input
  CLEAR_BIT(*button->ddr, button->pin);
  // ***** What about CONFIG_INPUT(*button->ddr, button->pin) ???
  // Enable Button pull-up resistor
  SET_BIT(*button->port, button->pin);

  // PCICR: Pin Change Interrupt Control Register
  // PCIE0: Pin Change Interrupt Enable Bit:
  //    Any change on any enabled PCINT7..0 can fire ISR.
  PCICR |= (1 << PCIE0);

  // PCMSK0: Pin Change Mask for Interrupt0, which is for all pins 0 through 7
  // Enable interrupts on Button A (PCINT3) and Button C (PCINT0)
  if (button->pin == BUTTONA) {
    PCMSK0 |= (1 << PCINT3);
  }
  if (button->pin == BUTTONC) {
    PCMSK0 |= (1 << PCINT0);
  }

  pinb_previous = PINB & BUTTON_MASK;
  return 0;
}

void green_led_on(void){
  CLEAR_BIT(*(&_green)->port, _green.pin);
}

void blink_green(void){
  TOGGLE_BIT(*(&_green)->port, _green.pin);
}

void green_led_off(void){
  SET_BIT(*(&_green)->port, _green.pin);
}

void yellow_led_on(void){
  SET_BIT(*(&_yellow)->port, _yellow.pin);
}

void blink_yellow(void){
  TOGGLE_BIT(*(&_yellow)->port, _yellow.pin);
}

void yellow_led_off(void){
  CLEAR_BIT(*(&_yellow)->port, _yellow.pin);
}

/* Set up callback functions for either of two buttons on the board.
* parameter [in] IO_struct button : for A or C as defined above
* parameter [in] release : 1 (true) = call function on release
* parameter [in] callback : function to be called when event happens
* parameter [out] : -1 for error in setting up. 1 otherwise.
*/
int SetUpButtonAction(IO_struct * button, int release, void(*callback)(void)) {
  // Set up for the callback function to be used when button event happens
  if (button->pin == BUTTONA) {
    if (release) {
      fn_release_A = callback;
    } else {
      fn_press_A = callback;
    }
  }
  if (button->pin == BUTTONC) {
    if (release) {
      fn_release_C = callback;
    } else {
      fn_press_C = callback;
    }
  }
}

ISR(PCINT0_vect) {
  
  #ifdef DEBUG
  // If debugging, it will be good to flash an led to know that you made
  // it into the ISR. Remeber that red and buttonC share the port, so don't
  // use the red led. If you soldered your headers, you can also set up
  // an LED in general IO to give yourself more options.
  TOGGLE_BIT(*(&_green)->port, _green.pin);
  _delay_ms(150);
  TOGGLE_BIT(*(&_green)->port, _green.pin);
  #endif
  
  // read port B, mask for only button A and C bits
  uint8_t pinb_now = (PINB & BUTTON_MASK);

  // check that pin change persists. ignore if not.
  _delay_ms(1);
  if (pinb_now ^ (PINB & BUTTON_MASK)) {
    return;
  }

  // record which button(s) changed state from last interrupt
  // remember that pinb_now holds state for only button A and C
  uint8_t pinb_change = (pinb_now ^ pinb_previous);

  // ************************************************************/
  // ***********    COMPLETE THIS PART BELOW ********************/
  // ************************************************************/
  // Determine if Button A has changed since the last time
  // if it has changed, determine how it changed state (press or release)
  // then have the system react appropriately according to requirements.
  // pinb_change pinb_now
  if (pinb_change == (1<<BUTTONA)){  
    if ((pinb_now & (1<<BUTTONA)) != 0){
      ++button_A_release_time;
      if (button_A_release_time % 3 == 1) {SetUpButtonAction(&_button_A, 1, green_led_on); fn_release_A();}
      else if (button_A_release_time % 3 == 2) SetUpButtonAction(&_button_A, 1, blink_green);
      else if (button_A_release_time % 3 == 0) {SetUpButtonAction(&_button_A, 1, green_led_off); fn_release_A(); button_A_release_time = 0;}
      
    }
  }
  // Determine if Button C has changed since the last time
  // if it has changed, determine how it changed state (press or release)
  // then have the system react appropriately according to requirements.
  // Determine if Button C has changed since the last time
  
  if (pinb_change == (1<<BUTTONC)){  
    if ((pinb_now & (1<<BUTTONC)) != 0){
      ++button_C_release_time;
      if (button_C_release_time % 3 == 1) {SetUpButtonAction(&_button_C, 1, yellow_led_on); fn_release_C();}
      else if (button_C_release_time % 3 == 2) SetUpButtonAction(&_button_C, 1, blink_yellow);
      else if (button_C_release_time % 3 == 0) {SetUpButtonAction(&_button_C, 1, yellow_led_off); fn_release_C(); button_C_release_time = 0;}
    }
  }
  
  // You may make generic code using the fn_<state>_<Button> functions,
  // but you do not have to. You can put the code here in the ISR.
  pinb_previous = pinb_now;
}

