#ifndef stateMachine_included
#define stateMachine_included

char toggle_red(); // toggle red led on/off
void sm_update_led(); // update led values
void sm_update_buzzer(); // update buzzer sound
void sm_update_lcd(); // update lcd screen

void state_advance(unsigned int in); 
#endif
