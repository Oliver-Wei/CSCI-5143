#ifndef TASKS_H_
#define TASKS_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>

#include <math.h>
#include "leds.h"

#define HEIGHT 5
#define WIDTH 5

int Invert();
int DelayDelay();
int MaxMin();
int TaskDelay1();
int EventTask();
int TaskDelay2();
int TaskDelay3();
int Hough();
int Average();
int blink();

#endif
