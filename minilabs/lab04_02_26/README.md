##### CSCI5143 Spring 2019

#### Mini Lab 2 : Timers and Interrupts
> DUE: Tuesday, Feb 12 before class <br>
> Submit code via GitHub
> Submit write-up via Canvas

Start with example code: https://github.umn.edu/umn-cscii-5143-s19/labs/lab02

This code is almost identical to the provided lab01 code with the addition of timers.h and timers.c. It is not currently functional -- it will compile but no timers are set correctly.

<hr>

**__SYSTEM REQUIREMENTS__**

Using the code provided with this lab, implement the following behaviors.

1. Blink the RED LED at 1Hz (on at 500ms, off at 1000ms, ...)
1. Blink the YELLOW LED at 2Hz (on at 250ms, off at 500ms, ...)
2. Blink the GREEN LED at .5Hz (on at 1000ms, off at 2000ms) 2 times in response to the release of Button A.

This behavior must be achieved using the following architecture:

1. To blink RED, **__background self-schedule__** in the main loop, by observing system time (as maintained by an ISR) and toggling when appropriate. E.g.
```
if (ms_timer > release_time) { // toggle }
```
You are free to use some other conditional statement that will achieve the same logic.

2. To blink YELLOW, __**foreground time-triggered scheduling**__ by creating a timer dedicated to this task (i.e. separate from the one used as a system clock) in which a timer interrupt fires every 250ms, and the led is toggled inside the corresponding ISR.

3. To blink Green, __**foreground event-triggered scheduling**__ that is triggered by the button A release. The blinking of the led occurs inside the ISR, and importantly, DO NOT USE _delay_ms() to control time, instead observe approximately how many times you need to iterate an empty for loop to achieve the right timing. You will need to use the keyword volatile and the no-operation "nop" assembly instruction to compel the compiler to busy wait in your for loop.
```
volatile uint32_t count;
for (count=0; count < DELAY_1000ms_COUNT; count++) {
      __asm__ __volatile__("nop");
}
```
You will need to experiment some to derive a pretty good estimate of 1 second. There is no expectation that this timing will be perfect.

**__CODE MODIFICATION__**

You will need to implement the following to complete this assignment:

1. Write the line of code to appropriately set _match32_ in timers.c.
2. Complete the functions SetUpTimer_1 and SetUpTimer_3. You can _almost_ copy SetUpTimer_0, **HOWEVER** note that 0 is 8-bit and 1 and 3 are 16-bit, which means the control registers are not identical. You should look at the tables at the end of the 16-bit Interrupt section in the datasheet that define the various control and status bits in the registers to confirm the proper settings.
3. Set up a timer to maintain system time with a 1ms granularity (i.e. 1000Hz). Timers are established through the set up code, and importantly by adding the appropriate ISR.
4. Write the main loop to self-schedule red.
5. Set up a timer to schedule yellow.
6. Blink green inside the PCINT0 ISR, tracking time using an empty for loop (as discussed above). You can use a function pointer and pass it as a parameter to SetUpButtonAction() or modify the ISR code.

**__WRITE UP__**

Please answer the following questions related to the implementation above. Submit a pdf of your responses on CANVAS.

1. You were asked to simply observe the timing of the for loop and adjust the accounts to achieve the approximate timing. What method would you use if this needed to be precise? FYI, \_delay\_ms() is implemented with this technique of iterating through an empty for loop to achieve the desired delay.

1. If you were trying to establish a 1ms timer and you used 256 as your prescaler, your timer would start to drift.

  	a) State what value you would set your MATCH to, given a 256 prescaler.

  	b) Estimate how many times the interrupt would have fired after 1 minute using that match value. (It would be 60,000 interrupts per minute if the timer wasn't drifting.) Briefly justify your response.

1. State which tasks (i.e. which color led) are being disrupted by another task, in that the timing cannot be perfectly maintained, and identify the disrupting task(s). Also, if we consider the deadline equal to the period of the tasks, state how many deadlines the task will miss when it is disrupted. Briefly justify your response.

<hr>

#### Important Things to know about your board.

See the cheat sheet for more info: https://github.umn.edu/course-material/repo-rtes-public/blob/master/HardwareResources/AStarCheatSheet.md

And see the datasheet on the Atmega.

**__Registers__**

x = A-F <br>
n = 0-7

- DDRx : Data Direction Register for Port "x"
- PORTx : Port Output for Port "x"
- PINx : Port Input for Port "x"
- PCICR : Pin Change Interrupt Control Register
- PCMSK0 : Pin Change Enable Mask for Interrupt 0

**__Pin/Bit Locations__**

- DDxn : Data Direction bit location for port "x" pin "n"
- PORTxn : Port Output location for port "x" pin "n"
- PINxn : Pin Output location for port "x" pin "n"
- PCINTn : Pin Change Interrupt Enable for pin "n"

**__Timers__**

Timer ISR (so many details to keep track of!)
- You can choose between an 8-bit (Timer 0) or 16-bit timer (Timer 1 or Timer 3 ). For 8-bit, the match value cannot be higher than 2^8-1=255 and for 16-bit, you guessed it, 2^16-1=65,535.
- Frequency of the interrupt depends on the clock division of the 16MHz oscillator (clock select bits) and the match/compare value (in the output compare register).
- 8-bit and 16-bit timers have different bit configurations in the control register. Make sure you are looking at the right section in the datasheet (8-bit is Section 13, 16-bit is Section 14).
- TCCRxA and TCCRxB, where x signifies timer number.
	- Clock Select bits: see Table 13-8 or 14-5.  
	- Mode: see Table 13-7 or 14-4. Use CTC mode.
- TIMSKx: enable the compare match interrupt.
- OCRxc: x is port, c refers to channel (A or B)
- Create ISR(TIMERx_COMPx_vect) : iom32u4.h in avr directory.
- Manage when all interrupts are enabled or disabled with sei() and cli(), respectively.

<hr>

RESOURCES:
- iom32u4.h in avr library
- AStar pinout
- Atmel Datasheet, sections 10 (I/O), 13 (8-bit counters), 14 (16-bit counters)
- AStarDataCheatSheet
