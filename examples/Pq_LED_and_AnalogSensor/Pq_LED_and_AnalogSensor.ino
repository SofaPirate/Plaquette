#include <Plaquette.h>

LED debugLed;
AnalogSensor sensor;

void setup() {
  // put your setup code here, to run once:
 
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Pq.update();
  
  // Set LED intensity to sensor reading.
  debugLed.set( sensor.get() ); 
}
