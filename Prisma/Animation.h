void animation1();
void animation2();
void animation3();

struct Animation : Program {
  
  void setup() { }
  
  void draw() { 
    clearBackground();
    
    animation1();
    animation2();
    animation3();
  }
  
  void updateState() {}

  void reset() { }

} animation;

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
