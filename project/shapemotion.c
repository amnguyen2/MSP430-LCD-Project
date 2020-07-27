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

#include <stdio.h>

#define GREEN_LED BIT6
#define RED_LED BIT0

u_char state = 0;
u_char switches;

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
  0
};

Layer layer1 = {		/**< Layer with a red square */
  (AbShape *)&rect10,
  {screenWidth/2, 50}, /**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_RED,
  &fieldLayer
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
  p2sw_init(3); // use all 4 buttons 
  shapeInit();
  buzzer_init();
  
  layerInit(&layer1);
  layerDraw(&layer1);
  
  write_on_blackboard(); // write text on screen
  play_song(1.25); // plays array of notes at certain speed
  
  drawString5x7(0,0, "i'm safe :)", COLOR_WHITE, COLOR_BLACK);
  
  layerGetBounds(&fieldLayer, &fieldFence);

  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */

  for(;;) { 
    while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
      P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
      or_sr(0x10);	      /**< CPU OFF */
    }
    P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
    redrawScreen = 0;
    movLayerDraw(&ml1, &layer1);
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
    switches = p2sw_read();
    
    if ((switches & 0b0010) == 0b0010) {
      redrawScreen = 1;
    }

    /*
      drawTriangle(10, screenHeight-10, 10, COLOR_VIOLET);
      triangleColor = COLOR_YELLOW;
      drawTriangle(10, screenHeight-10, 10, triangleColor);
      play_song(1.0);
    */
  
    if ((switches & 0b0010) == 0b0010) { // sw2
      
    }

    else if ((switches & 0b0100) == 0b0100) { // sw3
      
    }

    else if ((switches & 0b1000) == 0b1000) { // sw4
    
    }
    //switches = 0;
    count = 0;
  } 
  P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}
