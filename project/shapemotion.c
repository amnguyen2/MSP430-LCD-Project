/** \file shapemotion.c
 *  \brief This is a simple shape motion demo.
 *  This demo creates two layers containing shapes.
 *  One layer contains a rectangle and the other a circle.
 *  While the CPU is running the green LED is on, and
 *  when the screen does not need to be redrawn the CPU
 *  is turned off along with the green LED.
 */  
#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>
#include "buzzer.h"
#include "led.h"
#include "stateMachines.h"

extern char state;

AbRect rect10 = {abRectGetBounds, abRectCheck, {10,10}}; /**< 10x10 rectangle */

AbRectOutline fieldOutline = {	/* playing field */
  abRectOutlineGetBounds, abRectOutlineCheck,   
  {screenWidth/2 - 20, screenHeight/2 - 20} 
};

Layer fieldLayer = {		/* playing field as a layer */
  (AbShape *) &fieldOutline,
  {screenWidth/2, screenHeight/2},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_WHITE,
  0 // no layers afterward
};


Layer layer1 = {		/**< Layer with a red square */
  (AbShape *)&rect10,
  {screenWidth/2, 50}, /**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_RED,
  &fieldLayer // next layer
};

/** Moving Layer
 *  Linked list of layer references
 *  Velocity represents one iteration of change (direction & magnitude)
 */
typedef struct MovLayer_s {
  Layer *layer;
  Vec2 velocity;
  struct MovLayer_s *next;
} MovLayer;


/* initial value of {0,0} will be overwritten */
MovLayer ml1 = { &layer1, {1,2}, 0}; 


void movLayerDraw(MovLayer *movLayers, Layer *layers)
{
  int row, col;
  MovLayer *movLayer;

  and_sr(~8);			/**< disable interrupts (GIE off) */
  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Layer *l = movLayer->layer;
    l->posLast = l->pos;
    l->pos = l->posNext;
  }
  or_sr(8);			/**< enable interrupts (GIE on) */

  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Region bounds;
    layerGetBounds(movLayer->layer, &bounds);
    lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1], 
		bounds.botRight.axes[0], bounds.botRight.axes[1]);
    for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
      for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
	Vec2 pixelPos = {col, row};
	u_int color = bgColor;
	Layer *probeLayer;
	for (probeLayer = layers; probeLayer; 
	     probeLayer = probeLayer->next) { /* probe all layers, in order */
	  if (abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelPos)) {
	    color = probeLayer->color;
	    break; 
	  } /* if probe check */
	} // for checking all layers at col, row
	lcd_writeColor(color); 
      } // for col
    } // for row
  } // for moving layer being updated
}	  

/** Advances a moving shape within a fence
 *  
 *  \param ml The moving shape to be advanced
 *  \param fence The region which will serve as a boundary for ml
 */
void mlAdvance(MovLayer *ml, Region *fence)
{
  Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  for (; ml; ml = ml->next) {
    vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);

    for (axis = 0; axis < 2; axis ++) {

      if ((shapeBoundary.topLeft.axes[axis] < fence->topLeft.axes[axis]) ||
	  (shapeBoundary.botRight.axes[axis] > fence->botRight.axes[axis]) ) {

	int velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis]; // bounce
	newPos.axes[axis] += (2*velocity);
      }	/**< if outside of fence */
    } /**< for axis */
    ml->layer->posNext = newPos;
  } /**< for ml */
}

void write_on_blackboard() {
  // 14 8x12 chars fit across the screen            
  drawString8x12(40,screenHeight/2,    "DO NOT", COLOR_WHITE, COLOR_BLACK);
  drawString8x12(44,screenHeight/2+15, "ERASE", COLOR_WHITE, COLOR_BLACK);
}



u_int triangleColor = COLOR_VIOLET;
u_int bgColor = COLOR_BLACK;     /**< The background color */
int redrawScreen = 1;           /**< Boolean for whether screen needs to be redrawn */

Region fieldFence;		/**< fence around playing field  */

/** Initializes everything, enables interrupts and green LED, 
 *  and handles the rendering for the screen
 */
void main()
{
  P1DIR |= GREEN_LED;		/**< Green led on when CPU on */		
  P1OUT |= GREEN_LED;

  P1DIR |= RED_LED;		
  
  configureClocks();
  lcd_init();
  p2sw_init(15); // use all 4 buttons 
  shapeInit();
  buzzer_init();
  led_init();
  
  layerInit(&layer1);
  layerDraw(&layer1);
  
  write_on_blackboard(); // write text on screen
  play_song(1.25); // plays array of notes at certain speed
  
  drawString5x7(0,0, "i'm safe :)", COLOR_WHITE, COLOR_BLACK);
  
  layerGetBounds(&fieldLayer, &fieldFence);

  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */

  // main loop
  for(;;) { 
    while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
      P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
      or_sr(0x10);	      /**< CPU OFF */
    }
    P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
    redrawScreen = 0;
    movLayerDraw(&ml1, &layer1);

    drawTriangle(10, screenHeight-10, 10, triangleColor); // this triangle will change color

    // decide what happens with switches
    u_int switches = p2sw_read(), i;
    
    char str[5];
    for (i = 0; i < 4; i++) {
      if (switches & (1<<i)) {
	str[i] = '-';
	
      } else {
	str[i] = '1'+i;
	state_advance(i); // advance state machine
	
	if (i == 0) { // sw1
	  triangleColor = COLOR_RED;
	  write_on_blackboard();
 	  
	} else if (i == 1) { // sw2
	  triangleColor = COLOR_ORANGE;
	  
	} else if (i == 2) { // sw3
	  triangleColor = COLOR_YELLOW;
	  
	} else if (i == 3) { // sw4
	  triangleColor = COLOR_GREEN;
	}
      }	
     
    }
    str[4] = 0;
    drawString8x12(screenWidth-40, 0, str, COLOR_WHITE, COLOR_BLACK);
  }
}

/** Watchdog timer interrupt handler. 15 interrupts/sec */
void wdt_c_handler()
{
  static short count = 0;
  
  P1OUT |= GREEN_LED;		      /**< Green LED on when cpu on */
  count++;
  if (count == 15) {
    mlAdvance(&ml1, &fieldFence);
    
    if (p2sw_read()) {
      redrawScreen = 1;
    }
    count = 0;
  }
  P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}
