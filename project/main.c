#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include "led.h"
#include "switches.h"
#include "buzzer.h"

void main(void) {
  configureClocks(); // setup master oscillator, CPU & peripheral clocks
  
  // LCD screen set-up
  lcd_init(); 
  u_char width = screenWidth, height = screenHeight;
  clearScreen(COLOR_BLACK);

  drawString5x7(10,10, "HELLO!", COLOR_GREEN, COLOR_BLUE);

  // from p2sw-demo
  p2sw_init(15);
  or_sr(0x8); // GIE (enable interrupts)

  // from lab 2
  switch_init();
  led_init();
  buzzer_init();
  enableWDTInterrupts(); // enable periodic interrupt

  or_sr(0x18); // CPU off, GIE on
}
