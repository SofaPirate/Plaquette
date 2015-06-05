#include <Plaquette.h>

LED debugLed(13);
Switch button(3);

void setup() {
  // put your setup code here, to run once:
 
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Pq.update();
  
  if ( button.isPressed() ) debugLed.on();
  else debugLed.off();
  
}
