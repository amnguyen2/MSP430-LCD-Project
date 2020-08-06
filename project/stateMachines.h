#ifndef stateMachine_included
#define stateMachine_included

char toggle_red(); // toggle red led on/off
void sm_update_led(char state); // update led values
void sm_update_buzzer(char state); // update buzzer sound
void sm_update_lcd(char state); // update lcd screen

// state_advance written in stateAdvance.s
//char state_advance(unsigned int in); 
#endif
