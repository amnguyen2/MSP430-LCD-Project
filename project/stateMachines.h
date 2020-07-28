#ifndef stateMachine_included
#define stateMachine_included

void sm_update_led(); // update led values
void sm_update_buzzer(); // update buzzer sound
void sm_update_lcd(); // update lcd screen
void state_advance(unsigned int in); // determine state

#endif
