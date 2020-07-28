#include <msp430.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include "stateMachines.h"
#include "led.h"
#include "buzzer.h"

static char state = 0;
extern unsigned int triangleColor;

static char new_red_on;
static char dimness = 0; // higher dimness = lower brightness
static char f = 0; // for dimming functionality. is the led being flashed right now?

void sm_update_led()
{
  dimness = 8;
  f++; // f iterates from 0 to 'dimness'
  if (f >= dimness) f = 0; // reset f back to 0
  
  switch(state) {
  case 1: // state 1
    new_red_on = 0;
    break;
  case 2: // state 2
    new_red_on = 0;
    break;
  case 3: // state 3
    new_red_on = (f==0); // dimmed
    break;
  }

  // check if led changed
  if (red_on != new_red_on) {
    red_on = new_red_on;
    led_changed = 1;
  }
  
  led_update();
}

void sm_update_buzzer()
{
  switch(state) {
  case 1: // state 1
    buzzer_set_period(0); 
    break;
  case 2: // state 2
    buzzer_set_period(4545.45); // A4
    break;
  case 3: // state 3
    buzzer_set_period(0);
    break;
  }
  return;
}

void sm_update_lcd()
{
  switch(state) {
  case 1:
    triangleColor = COLOR_RED;
    write_on_blackboard();
    break;
  case 2:
    triangleColor = COLOR_ORANGE;
    break;
  case 3:
    triangleColor = COLOR_BLUE;
    break;
  }
}

void state_advance(unsigned int in)
{
  switch(in) {
  case 1:
    state = 1;
    break;
  case 2:
    state = 2;
    break;
  case 3:
    state = 3;
    break;
  case 4:
    state = 4;
    break;
  }  
}
