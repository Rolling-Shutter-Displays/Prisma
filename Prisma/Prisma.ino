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
#define BWIDTH 32
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

#include "PrideFlag.h"
#include "Filling.h"
#include "Animation.h"

Program* programs[3] = { &prideFlag , &filling , &animation };

int program = 0;
const int program_size = 2;

void clickNext( EButton &btn ) {
  if ( program < program_size ) {
    program++;
  } else {
    program = 0;
  }
}

void clickPrev( EButton &btn ) {
  if ( program > 0 ) {
    program--;
  } else {
    program = program_size;
  }
}

void setup() {
  //Setup of the RSD 
  rsd.begin( 30 , BWIDTH );
  
  rsd.attachChannel( &red );
  rsd.attachChannel( &green );
  rsd.attachChannel( &blue );

  rsd.attachDraw( draw );

  //Setup of the buttons
  buttonNext.attachSingleClick( clickNext );
  buttonPrev.attachSingleClick( clickPrev );

  /*
  pinMode( A1 , INPUT_PULLUP );
  pinMode( A2 , INPUT_PULLUP );
  pinMode( A3 , INPUT_PULLUP );
  */
  
  //Programs setup
  for( int i = 0 ; i <= program_size ; i++ ) {
    programs[i]->setup();
  }
  
  programs[program]->reset();

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
  if( digitalRead( A1 ) == LOW ) program = 0;
  if( digitalRead( A2 ) == LOW ) program = 1;
  if( digitalRead( A3 ) == LOW ) program = 2;
  */
}

//Let's draw!
void draw() {
  
  programs[program]->draw();

}
