/**
 * EventsTiming
 *
 * Demonstrates the use of events for timing.
 * 
 * In this scenario, two metronomes are used to trigger the activation
 * of two ramps at different periods. The first metronome is also used to 
 * toggle the LED. A pushbutton allows to toggle the events from the first
 * metronome by clearing them and re-adding them.
 * 
 * This example is best viewed using the Arduino Serial Plotter.
 *
 * The circuit:
 * - LED attached from digital pin 13 to ground (*)
 * - pushbutton attached to digital pin 2 from ground (optional)
 * Note: on most Arduinos there is already an LED on the board
 * attached to pin 13.
 *
 * Created in 2025 by Sofian Audry
 * 
 * This example code is in the public domain.
 */
#include <Plaquette.h>  // Library for handling metronome and ramp functionalities

// LED connected to built-in pin.
DigitalOut led(LED_BUILTIN); 

// Button (optional) used to toggle the events support of metronome 1.
DigitalIn button(2, INTERNAL_PULLUP);

// Instantiate two Metronome objects with different periodicity.
Metronome metro1(1.0f); // period of 1 second
Metronome metro2(5.0f); // period of 5 seconds

// Ramp objects to show metronome activation (duration 100ms).
Ramp ramp1(0.1f);
Ramp ramp2(0.1f);

// State variable for toggling metro1 events.
bool eventsMetro1 = true; 

void begin() {
  // Ramps associated with metronomes.
  ramp1.fromTo( 0, 3);
  ramp2.fromTo(-3, 0);

  // Metronome events.
  metro1.onBang(toggleLed);
  metro1.onBang(launchRamp1);
  metro2.onBang(launchRamp2);

  // Button configuration.
  button.debounce(); // enable debouncing
  button.onRise(toggleEventsMetro1); // trigger function on button press
}

void step() {
  // Print ramps.
  printRamps();
}

// Callback functions /////////////////////////////////////////////////
void toggleLed() {
  led.toggle();
}

void launchRamp1() {
  ramp1.start();
}

void launchRamp2() {
  ramp2.start();
}

// Toggles event registration for metro1.
void toggleEventsMetro1() {
  if (eventsMetro1) {
    // If events are active, clear all events from metro1
    metro1.clearEvents();
  } else {
    // Re-register events for metro1
    metro1.onBang(launchRamp1);  
    metro1.onBang(toggleLed);  
  }
  // Toggle state variable
  eventsMetro1 = !eventsMetro1;
}

// Print the current values of the ramps to the serial monitor
void printRamps() {
  Serial.print(ramp1);    // Print value of ramp1
  Serial.print(" ");      // Separator
  Serial.println(ramp2);  // Print value of ramp2
}
