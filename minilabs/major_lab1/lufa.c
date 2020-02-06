#include "lufa.h"

#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <math.h>

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <util/delay.h>

extern uint64_t green_toggle_count;
extern uint64_t yellow_toggle_count;
extern uint64_t red_toggle_count;
extern char set_up_experiment;

#define menuString "z(ero) e(xperiment) g(o) p(rint) r(period)\n\r"

extern int experiment;
extern char in_ui_mode;

uint8_t clear_channel=(1<<REFS1) | (1<<REFS0) | (1<<ADLAR);

void init_usb(){
  SetupHardware();
}

void handleInput( char c ) {
  // WARNING: This uses a busy-wait, thus will block main loop until done
  const int COMMAND_BUFF_LEN = 16;
  int i;
  char command[COMMAND_BUFF_LEN+1];

  USB_Mainloop_Handler();

  // Get chars until end-of-line received or buffer full
  for (i = 0; i < COMMAND_BUFF_LEN; i++) {

    // first char rcd as input param. next char fetched at bottom of for-loop
    // if its backspace, reset index
    if (c == '\b') {
      i -= 2;
      printf("\b \b");
      continue;
    }
    // if newline, go back 1 to write over
    else if (c == '\n') {
      i--;
      continue;
    }
    // if return, we have complete input
    else if (c == '\r') {
      break;
    }

    // otherwise, we got a char - echo it back
    printf("%c", c);
    USB_Mainloop_Handler();
    command[i] = c;

    // in busy-wait until char received
    c = EOF;
    while ( c == EOF ) {
      USB_Mainloop_Handler();
      c = fgetc(stdin);
    }
  }

  // echo back the return
  printf("\r\n");
  USB_Mainloop_Handler();

  // buffer is full - ignore input
  if (i == COMMAND_BUFF_LEN) {
    printf("Buffer full\r\n");
    command[COMMAND_BUFF_LEN-1]='\r';
    USB_Mainloop_Handler();
    return;
  }

  command[i] = '\0';
  if (strlen(command) == 0) {
    return;
  }
  handleCommand( command );
  return;
}

void handleCommand( char *command ) {
  int value;
  char com;
  char go_experiment = 0;

  // Start with a char - need to change this to strings
  switch( command[0] ) {
    case( 'P'):
    case ('p'):
      // TODO: Write a print_vars() function
      printf("Green count %d\n\r",green_toggle_count);
      printf("Yellow count %d\n\r",yellow_toggle_count);
      printf("Red count %d\n\r",red_toggle_count);
      // for (int i = 0; i < 3; i++){
      //   printf("The missed deadlines of task %d is %d",i,tasks[i].missed_deadlines);
      // }
      break;
    case( 'E'):
    case('e'):
      sscanf(command,"%c %d",&com,&experiment);
      printf("Ready to run experiment %d\n\r",experiment);
      break;
    case('R'):
    case('r'):
      sscanf(command,"%c %d",&com,&value);
      printf("Change green period to %d ms.\n\r", value);
      break;
    case('Z'):
    case('z'):
      printf("Zeroing variables.\n\r", command[0]);
      // TODO: Write a zero_all() function
      green_toggle_count = 0;
      yellow_toggle_count = 0;
      red_toggle_count = 0;
      break;
    case('G'):
    case('g'):
      printf("Go with experiment %d\n\r", experiment);
      in_ui_mode = 0;
      set_up_experiment = 1;
      sei();
      break;
    default:
      printf(menuString);
    }
  USB_Mainloop_Handler();
}

// initialize adc
void adc_init() {

    // AREF = AVcc
    ADMUX = (1<<REFS0);

    // ADC Enable and prescaler of 128
    // 16000000/128 = 125000
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);

    // initialize power supply. Astar pin 11 = PortB7
    DDRB |= ( 1 << DDB7 );
    PORTB |= ( 1 << PORTB7 );
}

// read adc value
uint16_t adc_read(uint8_t channel) {

  ADMUX &= clear_channel;
  if (channel >= 8) {
    ADCSRB |= (1<<MUX5);
    ADMUX |= (channel - 8);
  }
  else {
    ADCSRB &= ~(1<<MUX5);
    ADMUX |= channel;
  }



    // start single conversion
    // write '1' to ADSC
    ADCSRA |= (1<<ADSC);

    // wait for conversion to complete
    // ADSC becomes '0' again
    // till then, run loop continuously
    while(ADCSRA & (1<<ADSC));

    return (ADC);
}

// int main()
// {
//   adc_results = adc_read(10);
//   return adc_resutls;
//   // adc_results = adc_read(9);
// }
