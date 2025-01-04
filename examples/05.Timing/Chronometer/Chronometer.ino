/**
 * Chronometer
 *
 * Counts time and outputs it to the serial monitor.
 * 
 * Uses a button as a controller:
 * - Long press stops the chronometer.
 * - Short press pauses/resumes the chronometer.
 *
 * The circuit:
 * - Pushbutton attached to digital pin 2 from ground
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The button.
DigitalIn button(2, INTERNAL_PULLUP);

// The main chronometer.
Chronometer mainChronometer;

// Chronometer that keeps track of how long button was pressed.
Chronometer buttonPressChronometer;

// Chronometer that triggers printing.
Chronometer printChronometer;

void begin() {
  // Debounce button.
  button.debounce();

  // Start main choronometer.
  mainChronometer.start();
  printChronometer.start();

  // Stop button press chronometer.
  buttonPressChronometer.stop();
}

void step() {
  // Button is pressed.
  if (button.rose()) {
    println("* Button pressed *");
    buttonPressChronometer.start();
  }

  // Stopped is released.
  else if (button.fell()) {
    println("* Button released *");

    // Long press: restart main chronometer.
    if (buttonPressChronometer >= 3.0) {
      println("* Long press : Restart *");
      mainChronometer.start();
      printChronometer.start();
    }
    
    // Short press: pause/resume main chronometer.
    else {
      print("* Short press : ");
      if (mainChronometer.isRunning()) {
        mainChronometer.pause();
        println("Pause *");
      }
      else {
        println("Resume *");
        mainChronometer.resume();
      }
    }
  }

  // Print every second.
  if (printChronometer.hasPassed(1.0)) { // alternative to: if (printChronometer >= 1.0)
    printChronometer.start();
    print(mainChronometer, 2); print(" secs ("); print(mainChronometer.isRunning() ? "RUNNING" : "STOPPED"); println(")");
  }
}
