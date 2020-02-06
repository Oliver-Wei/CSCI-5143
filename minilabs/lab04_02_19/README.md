##### RTES CSCI5143 Spring 2019

#### Lab 3 : Periodic Nonpreemptive Scheduler
> DUE: Tuesday, February 19 before class <br>
> Submit report via Canvas
> Submit code via GitHub

Establish the framework (primary code requirement) and get it as functional as possible before class.

Regardless of how far you get in the code, make sure you answer the questions about utilization in the Analysis section.

```
You can work with other people on the code.
Please respond to the questions individually.
```

<hr>

**__YOUR TASK__**

Use the provided code. Note that the common, buttons, and timers code is not included. Please use your versions developed from the previous lab. If you could not get the timers functional, please let me know so that we can get it working.

#### 1. The Server

In the main loop, write code to execute tasks by checking for the highest priority task that is ready to run in the table. If there is a task ready, the server calls the highest priority ready task.

#### 2. The Scheduler

Fill in the code in the ISR for the timer to check if a task should be moved to the ready state.

#### 3. The Tasks

You have been provided with the file _tasks.c_ that contain a collection of tasks with WCET (worst-case execution time) at approximately the value provided in the table. Priority for each task for making scheduling decisions should be determined using RM (i.e. rate monotonic) prioritization. The deadline is equal to the period of the task.

|Task ID | Task  | WCET (ms) | period (ms) |
|------|-------|------|--------|
| 1 | Invert()     | 1 | 10 |
| 2 | TaskDelay1() | 1 | 25 |
| 3 | MaxMin()     | 2 | 15 |
| 4 | DelayDelay() | 2 | 20 |
| 5 | TaskDelay2() | 2 | 30 |
| 6 | Average()    | 3 | 20 |
| 7 | TaskDelay3() | 3 | 25 |
| 8 | Hough()      | 6 | 50 |
| 9* | EventTask()  | 1 | 55 |

Task 9 is an event-triggered task. Whenever button A is pressed, this should send a signal to the schedule to release task 9. You can implement this in any way that seems reasonable.

#### 5. Analysis

Answer the following questions and submit a .pdf on Canvas.

1. What is the utilization of this set of periodic tasks? (Note that we are ignoring the time it takes for any ISR's. Of course, we need to do that to prove feasibility, but we will ignore that for now.)

2. What did you set as the priority of each task?

3. Briefly describe how you implemented the event-triggered task.

4. Let's say that a human can press a button twice in a row in the span of 55ms, which is why we have a 55ms period for this. But it isn't reasonable that a human would continuously press a button while the system is running. Why would we use 55ms, which will overestimate the typical utilization of this task?

5. If you were to create a static schedule of these tasks, how far out in time would your schedule have to go? Briefly justify your response.

6. If you were to create a static clock-driven schedule (with a fixed timer), what would you set the period of that timer to be? Briefly justify your response.

7. For task 8, the full code is executed only every 5th release. Why would we include the wcet of 6 rather than an average over those 5 releases?

8. Consider the tasks (10,1), (20,2), and (100,18) in a nonpreemptive scheduler. The utilization is quite minimal, yet we know there will be missed deadlines. Which task(s) will miss their deadline(s) and why?


#### Executing the Scheduler

You won't be able to determine whether or not the system is functioning properly because we have no means of reporting data or visualizing the tasks executing. Next week however, we will be getting serial communication up and running. If you get this lab done or you really want the ability to print to a terminal, you can try to get communication functional right away.

Those not on Windows have had good luck with the LUFA library. If you are on Windows or you don't want to deal with the LUFA library, you can use this dongle: https://www.adafruit.com/product/954. There is example code for this: https://github.umn.edu/course-material/repo-rtes-public/tree/master/ExampleCode/basic-serial

For LUFA, go here and follow the directions:
https://github.umn.edu/course-material/repo-rtes-public/tree/master/ExampleCode/basic-lufa
