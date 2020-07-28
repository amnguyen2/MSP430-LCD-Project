#ifndef stateMachine_included
#define stateMachine_included

void state_advance(unsigned int in);

void sm_update_dimness();
void sm_update_led();
void sm_update_buzzer();
void sm_update_lcd();

#endif
