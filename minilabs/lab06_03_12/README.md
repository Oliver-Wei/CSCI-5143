##### CSCI5143 Spring 2019
DUE at end of Class Today

#### Mini Lab 5 : non-PD Positional Control
> DUE: Thursday, November 1 <br>
> In-class demonstration for credit

This is meant to be completed during class.

<hr>

**__YOUR TASK__**

#### 1. Example Code Functional

You will need to use these pins for the example code. You can change how you connect the motor, but then you have to change the code.

|Color | Function| Pin |
|-----|-----|-----|
| Red | motor power | motor 2, green header (B6) |
| Black | motor power | motor 2, green header (B6)
| Green | encoder GND | |
| Blue | encoder Vcc | D1 (astar 2) |
| Yellow | encoder A | B4, PCINT 4 (astar 8) |
| White | encoder B  | B5, PCINT 5 (astar 9) |

Connect the motor powers to the green header at M2A and M2B. It does not matter which way you plug it in, but it will impact the "positive" direction as measured by the encoder.

You have to use the wall adapter to give the board enough power to run the motor. There is a power switch on the side of the board. When you turn it on, that gives power to the motors from the wall adapter.

#### 2. "UI" for Entering Reference Position

Configure it so that whenever button A is pressed, the reference position is set to +360 degrees relative to its current position. This should turn the motor on.

Configure it so that whenever button C is pressed, the reference position is set to -360 degrees relative to its current position. This should turn the motor on.

Monitor the global counts of the encoder (this is your measured position), and when you have reached your reference position, turn the motor off.

#### 3. "UI" for Managing Motor Speed

Calibrate the potentiometer to control the speed of the motor. This is identical to your previous in-class exercise in which you used the potentiometer to control the blink rate.

```
Do not let your motor run faster than 60% duty cycle for this exercise.
```
