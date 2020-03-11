/*
  Prisma.ino
  
  created 29 Dic 2019
  by derfaq
  
  modified 11 Mar 2020
  
*/

// Libraries  ////////////////////////////////////////////////////////////////////////////

// RSD : https://github.com/Rolling-Shutter-Displays/RSD

#include <RSD.h>
#include <Channel.h>
#include <Screen.h>

// EButton : https://github.com/jonnieZG/EButton
// ------- Specify not required features before including the EButton.h --------
#define EBUTTON_SUPPORT_TRANSITION_DISABLED
#define EBUTTON_SUPPORT_EACH_CLICK_DISABLED
#define EBUTTON_SUPPORT_DONE_CLICKING_DISABLED
//#define EBUTTON_SUPPORT_SINGLE_AND_DOUBLE_CLICKS_DISABLED
#define EBUTTON_SUPPORT_LONG_PRESS_START_DISABLED
#define EBUTTON_SUPPORT_LONG_PRESS_DURING_DISABLED

#include "EButton.h"

//Right Order
const int pinR = 6;  //OC0A
const int pinG = 5;  //OC0B
const int pinB = 11; //OC2A
const int pinW = 3;  //OC2B

RSD rsd;
#define BWIDTH 64
#define WIDTH ((BWIDTH*8)-1) 

Channel red( pinR , COMMON_CATHODE , BWIDTH );
Channel green( pinG , COMMON_CATHODE , BWIDTH );
Channel blue( pinB , COMMON_CATHODE , BWIDTH );
Channel white( pinW , COMMON_CATHODE , BWIDTH );

Screen display( &red , &green , &blue );

Channel *ch[4] = { &red , &green , &blue , &white  };

EButton buttonMain( A1 );
EButton buttonNext( A2 );
EButton buttonPrev( A3 );

// Programs  ///////////////////////////////////////////////////////////////////////////////
#include "Utils.h"
#include "Program.h"

int scene;

#define ORANGE 8

colour palette[8] =  { BLACK ,   RED ,    ORANGE ,      YELLOW ,     GREEN ,     BLUE ,     MAGENTA  ,   BLACK    };

const int p[9] =  { 0 , WIDTH/8 , WIDTH*2/8 , WIDTH*3/8 , WIDTH*4/8 , WIDTH*5/8 , WIDTH*6/8 , WIDTH*7/8 , WIDTH };
int diff[8];
int partition[9];

void setup() {
  //Setup of the RSD 
  rsd.begin( 30 , BWIDTH );
  
  rsd.attachChannel( &red );
  rsd.attachChannel( &green );
  rsd.attachChannel( &blue );

  rsd.attachDraw( draw );

  pinMode( A1 , INPUT_PULLUP );
  pinMode( A2 , INPUT_PULLUP );
  pinMode( A3 , INPUT_PULLUP );

  Serial.begin( 9600 );
}

void loop() {
  //
  buttonMain.tick();
  buttonNext.tick();
  buttonPrev.tick();
  
  //Run the engine
  rsd.update();
  
  // Tuning: Analog way
  int thick = map( analogRead( A0 ), 0 , 1023 , rsd.getLowerThick() , rsd.getHigherThick() );
  rsd.setThick( thick );
  rsd.setFine( thick );

  /*
  //Tuning: Fixed way
  //@frsd: 29.9819164276 , BWIDTH: 32 , tick: 2084 , fine: 2235 // Samsung S6 Edge 
  rsd.setTick( 2084 );
  rsd.setFine( 2235 );
  */
  if( digitalRead( A1 ) == LOW ) scene = 1;
  if( digitalRead( A2 ) == LOW ) scene = 2;
  if( digitalRead( A3 ) == LOW ) scene = 3;
}

/*
//Tuning: Serial way
void serialEvent() {
   int tick = Serial.parseInt();
   if ( tick ) { 
    tick = constrain( tick , 1 , 255 );
    tick = map( tick , 1 , 255 , rsd.getLowerTick() , rsd.getHigherTick() );
    
    rsd.setTick( tick );
    rsd.setFine( tick );
   }
}
*/

//Let's draw!
void draw() {
  display.clear();
  white.clear();

  switch( scene ){
    case 1:
  
      animation1();
      animation2();
      animation3();
      break;
    case 2: 
      // Pride flag
      for( int i = 0 ; i < 8 ; i++ ) {
    diff[i] = ( WIDTH/16 ) * sin(( TWO_PI * ( frameCount%120 ) / 120 ) + TWO_PI*i/16 );
  }

  partition[0] = p[0];
  for( int i = 0 ; i < 8 ; i++ ) {
    partition[ i+1 ] = p[i+1] + diff[i];
  }
  partition[8] = p[8];

  for( int i = 0 ; i < 8 ; i++ ) {
    if( palette[i]==ORANGE ) {
      display.fill( partition[i] , partition[i+1] , RED );
      dither( partition[i] , partition[i+1] , green );
    } else {
      display.fill( partition[i] , partition[i+1] , palette[i] );
    }
  }
    break;
    case 3:
        red.fill( 0 , frameCount % WIDTH );
        green.fill( 0 , ( ( uint16_t )( (float) frameCount * 1.7 ) % WIDTH ) );
        blue.fill( WIDTH - ( ( uint16_t )( (float) frameCount * 2.1 ) % WIDTH ) ,  WIDTH );
    break;
    default: break;
  }

}


void dither( int x0 , int x1 , Channel *ch ) {
  if ( x1 > x0 ) {  
    do {
      if ( x1 % 2 ) ch->line( x1 );
      x1--;
    } while( x1 > x0 ); 
    
    if ( x1 % 2 ) ch->line( x0 ) ;
  }
    /*
  } else if( x1 == x0 ) {
            
            line( x1 );
            
        } else {
            
            do {
                line( x0 );
                x0--;
            } while( x0 > x1 );
            
            line( x1 );
        }
}
*/
}

void animation1() {
  static int i = 0 , j;
  
  red.fillSafe( WIDTH/2 + i, WIDTH/2 + i + j );
  red.fillSafe( WIDTH/2 - i, WIDTH/2 - i - j );
  
  i = frameCount % ( WIDTH/2 ) ;
  j = i + 1;
  
  if ( i >= WIDTH/2 ) { i = 0; j = 0; };
}

void animation2() {
  static int i = 0 , j;

  green.fillSafe( WIDTH/2 + i , WIDTH/2 + i + j );
  green.fillSafe( WIDTH/2 - i , WIDTH/2 - i - j );

  i = ( uint16_t )( (float) frameCount * 1.7 ) % ( WIDTH/2 ) ;
  j = i + 1;
}

void animation3() {
  static int i = 0 , j;

  blue.fillSafe( WIDTH/2 + i, WIDTH/2 + i + j );
  blue.fillSafe( WIDTH/2 - i, WIDTH/2 - i - j );

  i = ( uint16_t )( (float) frameCount * 0.8 ) % ( WIDTH/2 ) ;
  j = i + 1;
}
