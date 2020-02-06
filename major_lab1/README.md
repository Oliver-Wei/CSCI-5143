### RTES Scheduling Lab: Counters, Timers, and Scheduling
### Due : Friday, March 8 at 11:55pm

#### Introduction

The purpose of this lab is to analyze an embedded system with respect to its timing requirements, constraints, and characteristics. The program you write will execute (schedule) several tasks using the various programming constructs that we have been working on over the past few weeks. The MOST IMPORTANT part of this assignment is understanding the behavior of your system. Please answer the questions at the bottom of the document thoughtfully to demonstrate your depth of understanding.

There are several pieces to this assignment. Please read this document carefully to ensure you understand what is being asked of the system and of you.

#### System Analysis

Use dynamic analysis to determine the WCET of each of these tasks, except for communication. Express your findings in microseconds. Develop a periodic task model such that you can define the response time calculated using the response time analysis equation w(t) for each task, except for communication. This model should include blocking due to nonpreemption.

The tasks are described below, but here is a handy table explaining how each is managed.

| TASK | Scheduling Mechanism |
|------|----------------------|
| Scheduler  | Inside an ISR  |
| Red LED    | Self-scheduled. Highest priority in main loop. |
| Yellow LED | Inside an ISR different from the Scheduler |
| Event Polling | Managed by the scheduler |
| Semaphore Task | Managed by the scheduler that uses flag to detect release. |
| Hough Transform | Managed by the scheduler |
| Green LED | PWM Signal |
| Green LED toggle count | In an ISR tied to the timer for the PWM signal |
| Communication | Managed by the scheduler, but do not include in analysis |

#### The System Tasks

Write a program that manages the tasks described below using your scheduler from the previous lab. Implement these in a way that is as precise as possible with respect to the specified timing requirements. Once this system is complete, you will add delays to disrupt the timing of these components, thereby making it impossible to meet some of the timing requirements.

**__Scheduler Task__**: Use your scheduler from lab to manage the scheduled tasks (not all tasks are scheduled).

**__RED LED Task__**: Toggle the GPIO Red LED at a default rate of 10 Hz (change state every 100 ms). This should be self-scheduled in that it monitors the time to decide when to run. It should have the highest priority among all tasks that run inside the main while-loop.

**__YELLOW LED Task__**: Toggle the GPIO Yellow LED at a default rate of 10 Hz (change state every 100 ms). This task is executed from inside an ISR (not the one used for the scheduler) with a frequency of 40Hz. Note that the frequency of the ISR is greater than the frequency of the toggle rate.

**__EVENT POLLING TASK__**: Periodically poll for a change in value of the potentiometer from the last time that it was read. This event should then set a flag that the scheduler will check to release the Semaphore Task. This follows the logic of a semaphore in which one task will wait for the signal of another to execute. This polling task should be managed by the scheduler.

**__SEMAPHORE TASK__**: Turn on the yellow on-board LED for a fixed amount of time in response to the “event” which is monitored by the EVENT POLLING TASK. When the task is released, turn on the on-board Yellow LED, delay for 5ms, then turn it off. The intent is that those 5ms represent use of the CPU, thus the delay should be implemented as a busy-wait. This task is managed by the scheduler.

**__Hough Transform Task__**: The Hough transform task is managed by the scheduler. Currently, the transform is being run over a 10x10 image, which takes much longer than 50ms. If you change the height and width of the image (specified inside image10.h), then you can decrease the timing to something close to 50ms. Determine the image size using WCET analysis so that the Hough task takes about 50ms (but not less than 50ms). (Note that it is not possible to store an entire image in memory, thus if your system had to analyze real-time video, it would either have to be transferred in small segments or preprocessed prior to transferring to the microprocessor. This Hough Transform Task simulates a computationally intensive task such as real-time image analysis.)

> The Hough Transform code serves multiple purposes in this assignment: 1) it gives you experience with calculating WCET, 2) demonstrates how to use PROGMEM, 3) demonstrates how constrained the resources are, and 4) makes the computationally intensive task more real than spinning in a busy-wait. The functionality of the code is not meaningful, thus use it to fulfill the purposes listed above.

**__GREEN LED Task__**: Toggle the PWM Green LED at a default rate of 10 Hz (change state every 100 ms). Allow the user to modify the frequency of this task through the menu. This task is implemented using a PWM signal. You might want to use Phase Correct PWM instead of Fast PWM to make it easier to count the toggles (see next task).

**__GREEN LED Counting Task__**: Maintain a count of the number of toggles of the GREEN LED from within an ISR. You should use the same timer/counter used to generate the PWM signal.

**__Communication Task__**: Upon the release of button A, pause the system and prompt the user with some basic menu options to either reconfigure the system or print out some data. The menu options are provided below. When the user is done, resume or reset the system, as appropriate. Remember that you can set timer/counters to 0 to start the count over.

> The Communication Task is a means for us to modify the system and report results. It should not be included as part of the overall analysis, nor does it have a deadline.

### The System Hardware

If you place your LEDs at different locations, please make your code flexible enough that the TA can easily change your code to these port pins.
- Green : Port B, pin 6.
- Yellow: Port D, pin 6.
- Red: Port B, pin 4.

### The System Menu

Note: The menu is a necessary part of the assignment so that you can see the results of your experiments. However, please do not spend significant time getting this to work -- share your code with your peers. Also, there is example code with a basic menu available at (https://github.umn.edu/course-material/repo-rtes-public/blob/master/ExampleCode/basic-menu/example/main.c).

Use the following menu options:
- p : Print data collected for experiment, including job releases and missed deadlines.
- e # : Set-Up this experiment number (e.g. Set flags to activate/deactivate delays in ISRs).
- z : Reset all variables to set up for a new experiment.
- g : Go signal for start experiment. (Try to synchronize all activities when this signal is given).

Example:
To set up experiment 2 type
```
e 2
```

You can add any additional menu options that make it easy for you to develop, debug, and experiment with your system. Please provide a description of these commands in a readme file.

### System Data Collection

For each task, keep track of the number of times a job is executed and the number of missed deadlines. A missed deadline would be one in which the next job was released before the current one completes. For example, if the Hough Transform Task was released but it was still executing, this would be a missed deadline. Note that no task should miss its deadline, except maybe the RED LED Task, when the system is first configured without any delay loops.

### Code Assessment

Logically correct, meets functional requirements, well organized, good naming conventions, and commented, but not superfluously.

### Program Execution

1. Turn on all GPIO LEDs at the start of the program, delay, and turn off to confirm LEDs are functional.
2. Set-up all the appropriate components of your system.
3. Print out the basic menu options to the user.
4. A “go” prompt from the user should set timer/counters to 0, enable interrupts, and enter the main loop.


### Report:

The intent of this lab is to demonstrate the mechanisms by which you can “schedule” on a microprocessor and to understand the advantages and disadvantages of those techniques. The delays that are being inserted into the system are impacting the performance and are meant to help you understand the impact of bad design decisions (like delays in ISRs), as well as understand the impact of spurious disruption to the system and which techniques are more robust in the face of that disruption.

> As the delays increase, it will become more difficult to gather and report missed deadlines and toggle counts. Do the best that you can, and when you arrive at a situation in which your results are unreliable (or unattainable), state that in your report and explain why.


### Experiments

Run a series of experiments as described below then answer the questions. For each experiment,
- Zero all job release and missed deadline counters.
- Configure the system for the experiment.
- Run the experiment for approximately 15 seconds, using ms_ticks.
- Record the number of releases and missed deadlines for all LEDs.

1. Configure the system so that all LEDs except the “semaphore” LED, toggle at a frequency of 2 Hz (I set the frequency at that so you could better see what is going on. If you want to run it slower or faster, feel free.) Run the system for about 30 seconds to confirm everything is functional and meeting the time requirements. This means that everything but the yellow on-board LED should be blinking at the same rate, at least at the start of the experiment.

2. Configure the system to the default frequencies (mostly 10 Hz). Place a 20ms busy-wait delay in the ISR for the GREEN LED Counting Task. Place it after the count. Run experiment and record results.

3. Place a 20ms busy-wait delay in the ISR for the YELLOW LED Task (remove the delay from #2 above). Place it after the toggle and make sure it delays every time the ISR is executed. Run experiment and record results.

Perform the following:

4. Repeat #2, except use a 30ms busy-wait.

5. Repeat #3, except use a 30ms busy-wait.


Perform the following:

6. Repeat #2, except use a 105ms busy-wait.

7. Repeat #3, except use a 105ms busy-wait.


8. Perform the following:
  - Repeat #7 (i.e. 10Hz toggle with 105ms busy-wait in YELLOW LED Task), except place an sei() at the top of the ISR before the toggle and delay.



#### Answer the Following Questions and Submit with Code

```
The most important part of this assignment is understanding the behavior of your system.
Please answer the questions thoughtfully and demonstrating your depth of understanding.
Large quantities of text does not equate to depth of understanding.
```

1. Describe your method for WCET analysis. How confident are you in those results? Briefly explain. What factors influence the accuracy of a WCET analysis?

2. Define the response time (i.e. w(t)) equation for each of the tasks. Write out the full equation so that no summation symbol is in the equation and blocking time is incorporated. Solve the response time analysis equation for the 3rd highest priority task. You do not need to solve it for the others.

3. In experiment #1,
<ol type="a">
 <li>Did you observe any “drift” in the blinking of the LEDs, meaning they were in sync but then one seemed to blink slightly slower or faster than the other? Briefly explain how drift can occur.
  <li>Were the LEDs synchronized at the start, meaning were they all on, then all off at the exact same time? Describe the factors that influence the ability to synchronize these events.
</ol>

4. Using the data collected in the above experiments, describe the behavior for each of the experiments #2 through #7, including number of job releases, missed deadlines, and expected number of releases. Explain the reason for the behavior for each of the experiments #2 through #7 (you can discuss each individually or describe them collectively). Notice that I am asking you to both describe AND explain the behavior.

5. Using the data collected in experiment #8, describe and explain the behavior of the system.

6. Consider the various scheduling methods used here and discussed in class. For each method below, discuss the control over the timing of that task, responsiveness of the system, and the impact on other tasks with respect to timing.
<ol type="a">
  <li> Time-driven execution inside an ISR (e.g. the scheduler),
  <li> Time-driven release managed by the scheduler,
  <li> External interrupt with execution inside an ISR (e.g. Communication Task, if inside ISR),
  <li> Periodic polling for an event that releases another task (e.g. Polling and Semaphore tasks).
