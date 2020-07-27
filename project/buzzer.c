#include <msp430.h>
#include "../timerLib/libTimer.h"
#include "buzzer.h"

// musical notes
// unsigned short = 2 bytes (0 to 65,535)
  
const unsigned short C3 = 15289.35; 
const unsigned short Db3 = 14431.06; 
const unsigned short D3 = 13621.19;
const unsigned short Eb3 = 12856.78;
const unsigned short E3 = 12135.19;
const unsigned short F3 = 11454.10;
const unsigned short Gb3 = 10810.81;
const unsigned short G3 = 10204.08;
const unsigned short Ab3 = 9631.59;
const unsigned short A3 = 9090.91;
const unsigned short Bb3 = 8580.74;
const unsigned short B3 = 8099.13;

const unsigned short C4 = 7644.38; 
const unsigned short Db4 = 7215.53; 
const unsigned short D4 = 6810.60;
const unsigned short Eb4 = 6428.18;
const unsigned short E4 = 6067.14;
const unsigned short F4 = 5726.88;
const unsigned short Gb4 = 5405.55;
const unsigned short G4 = 5102.04;
const unsigned short Ab4 = 4815.80;
const unsigned short A4 = 4545.45;
const unsigned short Bb4 = 4290.37;
const unsigned short B4 = 4049.57;

const unsigned short C5 = 3822.26;
const unsigned short Db5 = 3607.70;
const unsigned short D5 = 3405.24;
const unsigned short Eb5 = 3214.14;
const unsigned short E5 = 3033.75;
const unsigned short F5 = 2863.44;
const unsigned short Gb5 = 2702.74;
const unsigned short G5 = 2551.05;
const unsigned short Ab5 = 2407.87;
const unsigned short A5 = 2272.73;
const unsigned short Bb5 = 2145.16;
const unsigned short B5 = 2024.76;

// arrays of notes (songs)
const int song[12] = {G4, Gb4, Eb4, A3, Ab3, E4, Ab4, C5, C5}; 

// speed is duration of a note, 125000 is a good speed to start with
void play_song(float speed)
{
  long note_len = (long)speed * 100000; 
  for (int i = 0; i < 12; i++) { // iterate through song
    for (long j = 0; j < note_len; j++) { // play a note
      buzzer_set_period(song[i]);
    }
  }
  buzzer_set_period(0); // silence buzzer
}

void buzzer_init()
{
    /* 
       Direct timer A output "TA0.1" to P2.6.  
        According to table 21 from data sheet:
          P2SEL2.6, P2SEL2.7, anmd P2SEL.7 must be zero
          P2SEL.6 must be 1
        Also: P2.6 direction must be output
    */
    timerAUpmode();		/* used to drive speaker */
    P2SEL2 &= ~(BIT6 | BIT7);
    P2SEL &= ~BIT7; 
    P2SEL |= BIT6;
    P2DIR = BIT6;		/* enable output to speaker (P2.6) */
}

void buzzer_set_period(short cycles) /* buzzer clock = 2MHz.  (period of 1k results in 2kHz tone) */
{
  CCR0 = cycles; 
  CCR1 = cycles >> 1;		/* one half cycle */
}
