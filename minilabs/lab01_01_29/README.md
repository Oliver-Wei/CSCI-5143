##### CSCI5143 Spring 2019

#### Lab 1 : Blinking LEDs
> CODE DUE: Tuesday, January 29 before class <br>
> WRITE UP DUE: Thursday, January 31 at 11:55pm <br>
> Submit via GitHub (or email it to me (zipped) if you are still working on getting Github functional).

Start with example code in this directory. It is functional code that is currently in debug mode. It generates a _heartbeat_ within the main loop (i.e. blinks the yellow led) and it will flash the green led whenever the pin change interrupt is activated, which is related to pressing and releasing buttons A and C. Give it a try and see what happens. Hold the button for a bit before you release it and watch the green LED.

<i>If you do not know Github, please do not spend a significant amount of time trying to figure it out. Ask a peer or attend an office hour. With assistance, you should be able to learn most of what you need to know in about 15 minutes. See below for directions and references for github.

Start the lab by cloning the class-repo and your personal repo (or use the download button if you want to deal with github later). If you don't see a repo, you can make one -- "new" icon in the upper right. Please name it "repo-<x500>", for example Dr. Larson's would be "repo-lars1050". Copy the content of the class-repo to your personal, maintaining the same folder and path structure. Please modify your personal repo and push that to the server for submission. Please push often so that you don't lose any work.

<hr>

_Toggle_ means to change from one state to another (if ON, turn OFF;  if OFF, turn ON). _Blink_ means to turn ON then turn OFF after some delay.

You can develop code in your favorite editor and compile on the command line with avr-gcc and the makefile. I will add more info here about Windows machines as I consult with others who are using that OS.

**__SYSTEM REQUIREMENTS__**

We did not get as far along on our first day as I thought that we would. For Tuesday, January 29, please have a solid attempt at the implementation of this lab. What that means is:
- understand how the PCINT0 interrupt is set-up and enabled (the code is there, you just have to try to understand it).
- implement some reasonable code in PCINT0 to read the value of a button and determine whether it was pressed (i.e. value 0) or released (i.e. value 1). Then set a flag (or a variable that controls the state of the led) accordingly that will be read in main to determine if the light should be on, off, or blinking.
- implement some reasonable code to read your flag or state variable and control the LED accordingly. You can use _delay_ms(). There is code already in place for toggling (blinking) an LED, so you can copy that for other LEDs.

We will work on this lab together in class on Tuesday to get it fully functional. Then you can finish the write-up and turn that in by Thursday. 

Using the code provided with this lab, implement the following functionality.

1. Do not use any timer interrupts. Use only the PCINT ISR.

1. Start the system with both LEDs off.

1. Onboard buttons modify LED functionality (state) in the following way:
	- Button A: modifies only the Green LED state
	- Button C: modifies only the Yellow LED state
	- 1st **__release__**: LED ON (solidly, no blinking)
	- 2nd **__release__**: LED Blink at frequency provided below
	- 3rd **__release__**: LED OFF

1. When an LED is in a blinking state:
	- GREEN LED
		- blink at 2 Hz (ON at 250 ms, OFF at 500 ms, ON at 750 ms, OFF at 1 sec, ...)
	- YELLOW LED
		- blink at .4 Hz (ON at 1250 ms, OFF at 2500 ms, … ).

> The green and yellow LED are independent. Either can be in any state regardless of the state of the other.

**__WRITE UP__**

Please answer the following questions related to the implementation above. Submit a pdf of your responses on CANVAS.

1. Please provide the code of your while loop here for easier grading. You can provide a snapshot or cut and paste it into your write-up.

1. When in a blink state, the LEDs will not blink perfectly at the required frequency, no matter how good your coding skills. For example, if I let the system run for 10 minutes, there would be fewer toggles of yellow than the anticipated 10\*60\*1000ms/1250=480. What in the system is interfering with the  timing of the blink?

2. The code can be simplified because the period of yellow (1250ms) is a multiple of green's (250ms). How would you modify your code if the period of yellow were a prime number (e.g. 983)?

1. Please provide the code of your PCINT ISR here for easier grading.

1. If debug is active (i.e. #define DEBUG is not commented out), then there is a 150ms delay in the button press. Why is that a terrible place for a delay?

1. What will happen in your system if both buttons are pressed and released at the same time? Will the state of both buttons change or will one of the user requests get lost? Briefly justify your response. (Note, you can implement it to have either of those two behaviors.)

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

sei() : function call to enable all interrupts

<hr>

#### GitHub

If you have never used github, see this tutorial to get started on the umn github: https://www-users.cs.umn.edu/~lars1050/repo-website-resources/website/examples/csresources/git.html

Once you have an established repo for the course, do the following at the command prompt:

```
cd <location of where you want course folder to be on your laptop>
mkdir csci5143
cd csci5143
git clone https://github.umn.edu/umn-csci-5143-F18/class-repo
git clone https://github.umn.edu/umn-csci-5143-F18/repo-<your UMN user ID>
cd repo-<your UMN user ID>
mkdir minilabs
mkdir lab01_09_18
cp ../class-repo/minilabs/lab01_09_18 minilabs/lab01_09_18/.
git add *
git commit -m "minilabs(lab01): initial commit"
git push
```

Now you have a functional version of the code to develop. Try uploading to the board.

<hr>

#### Upload to the board

> You will probably need to change the port setting in your makefile

- Plug your board into your USB
- cd minilabs/lab01
- at command prompt ```make```
- push the reset button twice on your board which makes the yellow LED blink slowly (it is ready for upload)
- at command prompt ```make program```

The leds will flash as it is uploading and you will get a message scroll across your screen. When complete, the yellow, green, and red LEDs should blink 2 times each. If they keep blinking, hit reset again. After the 2 blinks, green should turn off and yellow continues to blink. Push the A or C button to make the green blink once.
