struct Filling : Program {
  
  void setup() { }
  
  void draw() { 
    clearBackground();
    
    red.fill( 0 , frameCount % WIDTH );
    green.fill( 0 , ( ( uint16_t )( (float) frameCount * 1.7 ) % WIDTH ) );
    blue.fill( WIDTH - ( ( uint16_t )( (float) frameCount * 2.1 ) % WIDTH ) ,  WIDTH );
  }
  
  void updateState() {}

  void reset() { }

} filling;
