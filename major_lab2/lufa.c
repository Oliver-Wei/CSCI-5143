#include "lufa.h"

#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <math.h>

#include <avr/io.h>
#include <avr/interrupt.h>

extern volatile int ms_ticks;

extern volatile int16_t global_counts_m2;
extern volatile int16_t last_global_counts_m2;
extern volatile int8_t global_error_m2;

extern int kD;
extern int kP; 
extern int Vm_100;
extern int T;
extern int PM_array[300];
extern int T_array[300];
extern uint16_t adc_10;
extern uint16_t adc_9;
extern int Pr;
extern volatile char in_ui_mode;

#define menuString "z(ero) e(xperiment) g(o) p(rint) r(period)\n\r"



void init() {
    SetupHardware(); //This setups the USB hardware and stdio
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

  // if (T == 0) {
  //   printf("----------------------------------\n\r");
  //   printf("PM_array:\n\r");
  //   for(int j=0; j<PM_array_len; j++) printf("%d\n\r",PM_array[j]);
  //   PM_array_len = 0;
  //   printf("----------------------------------\n\r");
  // }

  command[i] = '\0';
  if (strlen(command) == 0) {
    return;
  }
  handleCommand( command );
  return;
}

void handleCommand( char *command ) {
  // int value;
  char com;
  int value;
  // char go_experiment = 0;

  // Start with a char - need to change this to strings
  switch( command[0] ) {
    case('R'):
    case('r'):
      sscanf(command,"%c %d",&com,&value);
      Pr = value * 2248.9 / 360.0; 
      printf("Change reference position to %d degree.\n\r", value);
      break;
    case('Z'):
    case('z'):
      global_counts_m2 = 0;

      Pr = 0;
      printf("Zeroing current place as 0 degree.\n\r");
      break;
    case('V'):
    case('v'):
      printf("Current Value:\n\r");
      printf("kD: 0.%d\n\r", kD);
      printf("kP: 0.%d\n\r", kP);
      printf("Pr: %d\n\r", Pr);
      printf("Pm: %d\n\r", global_counts_m2);
      printf("Vm: %d\n\r", Vm_100);
      // printf("Error: %d\n\r",global_error_m2);
      printf("ms_ticks: %d\n\r", ms_ticks);
      printf("T: %d\n\r", T);
      break;
    case('S'):
    case('s'):
      printf("----------------------------------\n\r");
      printf("PM_array:\n\r");
      for(int j=0; j<100; j++) printf("%d\n\r",PM_array[j]);
      // PM_array_len = 0;
      printf("----------------------------------\n\r");
      printf("----------------------------------\n\r");
      printf("T_array:\n\r");
      for(int j=0; j<100; j++) printf("%d\n\r",T_array[j]);
      // PM_array_len = 0;
      printf("----------------------------------\n\r");
      break;
    
    case('G'):
    case('g'):
      printf("Read ADC Result\r\n");
      printf("ADC 10: %d\r\n", adc_10);
      printf("ADC 9: %d\r\n", adc_9);
      break;
    case ('Q'):
    case('q'):
      printf("EXIT!");
      ms_ticks = 0;
      in_ui_mode = 0;
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

uint8_t clear_channel=(1<<REFS1) | (1<<REFS0) | (1<<ADLAR);

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
