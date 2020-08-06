# Project 3 - Moving Eraser

I wrote code to make the MSP430 board into a simple toy. The state machine I wrote
plays notes using the buzzer and flashes the LEDs after pressing the buttons on the green board.
The LCD screen draws shapes and text that change over time and when buttons are pressed. The CPU
sleeps when the 4th button is pressed.

## Running the Program

To compile and run the program on the MSP430, use:

**$ make load**

To clean any unnecessary or temporary files from the project directory:

**$ make clean**

## Initial Explanation of Program

  The toy I created uses a state machine. The state machine begins at state 0. 
Pressing the first 3 buttons cause the state machine to advance to any of the 3 states, 
respective to each of the buttons. 
Holding down the fourth button will turn the CPU off and keep it asleep until the fourth
button is not being pressed.

  - The green LED will be exclusively used to indicate whether the CPU is asleep or not. While
  the CPU is awake/on, the green led is bright. The LED goes dimmer when the CPU is asleep/off.
  - 4 dashes will be written at the top-right corner of the screen. These change depending on
  the current button being pressed at the time.

## State Machine

Program start-up:
   - The LCD initially draws a white box, red square, and text reading "DO NOT ERASE".
   - The buzzer plays a short song. The program then begins in state 0.

0. Starting State
   - The state machine begins in this state. You cannot return back to it using buttons.
   - A message is written at the top-left of the screen, "i'm safe :)"
   - 4 dashes are written at the top-right of the screen indicating which buttons are being
   pressed.
   - A white triangle is drawn at the bottom-left of the screen.
   - The green LED begins shining brightly to indicate the CPU is awake.
   - The red square starts to bounce around, erasing the message "DO NOT ERASE".
   
1. State 1
   - Rewrite the message "DO NOT ERASE".
   - The triangle's color changes to red.
   - As SWITCH 1 is being pressed, a "1" will show itself at the top-right.
   - Must use SWITCH 1 to advance to this state.
   
2. State 2
   - The triangle's color changes to yellow.
   - The buzzer makes a very annoying sound.
   - As SWITCH 2 is being pressed, a "2" will show itself at the top-right.
   - Must use SWITCH 2 to advance to this state.
   
3. State 3
   - The triangle's color changes to blue.
   - The red LED shines dim.
   - As SWITCH 3 is being pressed, a "3" will show itself at the top-right.
   - Must use SWITCH 3 to advance to this state.

4. While button 4 is being pressed down
   - The LCD screen stops drawing, notice the red square isn't moving on screen.
   - As SWITCH 4 is being pressed, a "4" will show itself at the top-right.
   
## Notable Bug(s)

  - While in state 3 (button 3), the interrupts involving the dimming of the red LED
  causes the LED screen not to be drawn as intended:
    - The moving red square begins to leave "trails" of itself. 
    - The 4th button indicator at the top-right of the screen begins to flash as a 
    red rectangle, leaving behind an odd red line beneath. 
    - There are other noticable pixels that seem to be drawn without purpose. 
    - The extra pixels do not seem to go away until restart of the LCD screen. 
