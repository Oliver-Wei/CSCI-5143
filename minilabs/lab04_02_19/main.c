#ifndef F_CPU
#define F_CPU 16000000UL
#endif

// Uncomment this to print out debugging statements.
//#define DEBUG 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "common.h"
#include "timers.h"
#include "buttons.h"
#include "tasks.h"

// tri-state system
// Blocking (waiting for release). Ready (waiting for cpu). Running (has cpu)
#define BLOCKING 0
#define READY 1
#define RUNNING 2

// setting caps on priority and number of spawned tasks
#define MAX_PRIORITY 250
#define MAX_TASKS 1

// used for system time
volatile uint64_t ms_ticks;

// mutex access to ms_ticks
uint64_t get_ticks() {
  uint64_t temp;
  cli();
  temp = ms_ticks;
  sei();
  return temp;
}

/****************************************************************************
   TASK Data Structures
****************************************************************************/
// holds a task. All will be gathered into an array
typedef struct {
	int (*funptr)();
  int period; 						// milliseconds
  uint64_t next_release;  // absolute time of next release in ms
  int missed_deadlines;
  char id;
  int priority; 		// priority 1 has the highest priority
	int buffered;			// the number of jobs waiting to execute
  int max_buffered; // maximum bufferend while system running
  int releases;     // number of times released
  int executed;     // number of times executed
  int state;				// one of the 3 states
} Task;

// shared structure between scheduler (ISR) and server (main)
volatile Task tasks[MAX_TASKS];

// Array is initially empty. Spawn tasks to add to scheduler.
int task_count = 0;

/****************************************************************************
   TASK Scheduling Functions
****************************************************************************/
/* The creation of a single task, which is added to the array of tasks
 * Assuming all tasks in phase and will be released at start of system.
 * param [in] funptr : the code to be executed for a single job of the task.
 * param [in] id : not used in this version, but might have future utility
 * param [in] p : (period) time between releases in ms
 * param [in] priority : a fixed priority system (probably RM)
 */
int spawn(int(*fp)(), int id, int p, int priority) {
	if (task_count == MAX_TASKS) {
		return ERROR;
	}
  tasks[task_count].funptr = fp;
  tasks[task_count].period = p;
  tasks[task_count].next_release = p;
  tasks[task_count].id = id;
  tasks[task_count].priority = priority;
	tasks[task_count].buffered = 1;
  tasks[task_count].max_buffered = 1;
  tasks[task_count].releases = 0;
  tasks[task_count].executed = 0;
  tasks[task_count].state = READY;
  tasks[task_count].missed_deadlines = 0;
  ++task_count;
	return 1;
}

// A fixed priority system based on the period of a task
// The smaller the period, the higher the priority.
void spawn_all_tasks() {
	// @TODO: confirm that all tasks are spawned without error.
  // spawn(fptr, id, period, priority)
  // **** vvvvvvvvvv   FILL THIS IN   vvvvvvvvv ******* //
  // spawn(Invert,0,10,9);
  // spawn(MaxMin,2,15,8);
  // spawn(DelayDelay,3,20,7);
  // spawn(Average,5,20,6);
  // spawn(TaskDelay1,1,25,5);
  // spawn(TaskDelay3,6,25,4);
  // spawn(TaskDelay2,4,30,3);
  // spawn(Hough,7,50,2);
  // spawn(EventTask,9,55,1);
  spawn(blink,8,250,1);
}

/****************************************************************************
   ALL INITIALIZATION
****************************************************************************/
void initialize_system(void)
{
	initialize_leds();
	light_show();

  initialize_buttons();

	spawn_all_tasks();

	// SCHEDULER: timer 0, prescaler 64, period 1 ms
	SetUpTimerCTC(0, 64, 1000);
}

// void ReleaseA() {
//   // toggle the green to confirm button release recognized
//   // PORTD &= ~(1<<PORTD5);
//   // _delay_ms(100);
//   // PORTD |= (1<<PORTD5);
//   tasks[8].state = READY;
//   tasks[8].buffered += 1;
//   cli();
// }

/****************************************************************************
   MAIN
****************************************************************************/

int main(void) {
  // This prevents the need to reset after flashing
  USBCON = 0;

  int task_id = -1;
	int highest = MAX_PRIORITY + 1;

	initialize_system();

  int temp;
  int i;

  // used to toggle led to show liveness
  int period_ms = 1000;
  uint64_t expires = period_ms;

  // Set up to fire ISR upon button A activity
  // Upon the release (2nd input param) of the button, it will call fn ReleaseA
  SetUpButton(&_button_A);
  // SetUpButtonAction(&_button_A, 1, ReleaseA );

  SetUpTimerPWM(1, 256, 2);
  DDRB |= (1 << DDB6);
  PORTB |= (1 << PB6);
  // HERE WE GO
  ms_ticks = 0;
  sei();

  char c;


  //*******         THE CYCLIC CONTROL LOOP            **********//
  //*************************************************************//
  while(1) {

    /*
    // heartbeat
    if (get_ticks() >= expires) {
      TOGGLE_BIT(*(&_yellow)->port, _yellow.pin);
      expires = get_ticks() + period_ms;
    }
    */

    // Determine highest priority ready task
    // vvvvv FILL THIS IN vvvvvvv
    task_id = -1;
    for (int i = 0; i < MAX_TASKS; i++){
      if (tasks[i].state == READY){
        task_id = i;
        break;
      }
    }

		// Execute the task, then do housekeeping in the task array
		if (-1 != task_id) {
      cli();
			tasks[task_id].state = RUNNING;
      sei();
			tasks[task_id].funptr();
      cli();
      tasks[task_id].executed += 1;
			tasks[task_id].buffered -= 1;
			if (tasks[task_id].buffered > 0) {
				tasks[task_id].state = READY;
			} else {
				tasks[task_id].state = BLOCKING;
			}
      sei();
		} // end if -1 != task_id
  } /* end while(1) loop */
} /* end main() */

/****************************************************************************
   ISR for TIMER used as SCHEDULER
****************************************************************************/
// Timer set up in timers.c always enables COMPA
ISR(TIMER0_COMPA_vect) {
  // vvvvvvvvv    FILL THIS IN    vvvvvvvvvvvvv
	// Find all tasks that are ready to release.
	// Determine if they have missed their deadline.
  // Check the task table to see if anything is ready to release
  for (int i = 0; i < MAX_TASKS; i++){
    if (ms_ticks == tasks[i].next_release){
      tasks[i].next_release = ms_ticks + tasks[i].period;
      tasks[i].buffered += 1;
      tasks[i].state = READY;
      if (tasks[i].buffered > tasks[i].max_buffered){
        tasks[i].missed_deadlines += 1;
      }
    }
  }
  // ms_ticks is down here because want all tasks to release at 0 ticks
  ++ms_ticks;
  //if (ms_ticks%250 == 0) ; // PE6
}


