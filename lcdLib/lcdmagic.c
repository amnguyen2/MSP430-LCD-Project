/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

/** Initializes everything, clears the screen, draws "hello" and a square */
int
main()
{
  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;

  clearScreen(COLOR_BLUE);
  /*
  drawString5x7(20,20, "hello", COLOR_GREEN, COLOR_RED);

  fillRectangle(30,30, 60, 60, COLOR_ORANGE);
  */
  int i;
  int j;

  /*
  for (i = 128; i > 100; i--) {
    drawPixel(i, j++, COLOR_RED);
  }

  j = 0;
  for (j = 50; j <= 60; j++) {
    int row1 = j;
    int col1 = j;
    int row2 = j;
    int col2 = 128 - j;

    drawPixel(row1, col1, COLOR_PINK);
    drawPixel(row2, col2, COLOR_ORANGE);
  }
  */
  // triangles from sides of screen
  for (j = 0; j <= 64; j++) {
    int row = j;
    for (int col = j; col <= screenWidth-j; col++) {
      drawPixel(row, col, COLOR_GREEN); // rightward triangle (from left)
      drawPixel(col, row, COLOR_RED);   // downward triangle (from top)
    }
  }

  // weird black triangle
  for (j = 0; j <= 94; j++) { 
    //int row = j; // right-angle triangle
    //int row = j/2; // wide-angle triangle
    int row = j*1.5; // acute-angle triangle (triangle may be transparent) 

    for (int col = j; col <= screenWidth-j; col++) { // change col to move triangle left/right
      drawPixel(row, col, COLOR_PINK); // rightward triangle
      drawPixel(col, row, COLOR_BLACK);   // downward triangle
    }
  }


  clearScreen(COLOR_BLACK);
  drawTriangle(10, 10, 20, COLOR_WHITE);
}
