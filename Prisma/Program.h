// Programs //////////////////////////////////////////////////////////////////////////////

struct Program {
  
  unsigned int pot;
  unsigned int potValue;
  bool potState;
  
  bool paused = false;

  virtual void setup() {}
  virtual void draw() {}
  virtual void updateState() {}
  
  virtual void pause() { paused = true; }
  virtual void play() { paused = false; }
  virtual void reset() {}
  
};
