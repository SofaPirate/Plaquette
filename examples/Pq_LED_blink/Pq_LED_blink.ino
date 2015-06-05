#include <Plaquette.h>

LED debugLed(13);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  Pq.update();
  
  debugLed.on();
  delay(500); // DO NOT DO THIS, THIS IS FOR DEBUGGING
  debugLed.off();
  delay(500);
  
}
