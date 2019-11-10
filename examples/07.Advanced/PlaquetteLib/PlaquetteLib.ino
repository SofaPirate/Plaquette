/**
 * PlaquetteLib
 *
 * Demonstrates the use of the "library mode". In this mode
 * functions begin() and step() are not defined by the user.
 *
 * Rather the user can use Plaquette as an external library
 * by calling the following functions:
 * - Plaquette.begin() to initialize the Plaquette environment
 * - Plaquette.step() to perform one step in a looping process
 * - (optional) Plaquette.end() at the end of the program
 *
 * Created in 2018 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <PlaquetteLib.h>

// This is important; otherwise you need to precise naming by
// by prefixing pq:: in front of all Plaquette names
// eg. pq::DigitalIn, pq::Plaquette.begin(), etc.
using namespace pq;

// The button.
DigitalIn button(2); // defaults to INTERNAL_PULLUP

void setup() {
  Plaquette.begin(); // needs to be called at top of setup()
  button.smooth(); // debounce button
  Serial.begin(9600);
}

void loop() {
  Plaquette.step(); //needs to be called at top of loop()
  Serial.print("Button value: ");
  Serial.println(button);
}
