#include <Plaquette.h>

AnalogIn in; // defaults on pin A0
DigitalIn button(2);

DigitalOut led; // defaults on pin 13
SquareOsc osc1; // square oscillator (default 1 sec period with 50% duty-cycle)
SquareOsc osc2(2, 0.75); // square oscillator with period of 2 sec and 75% duty-cycle

void begin() {}

void run() {
  // If button is pressed and input signal is big enough then use first oscillator
  if (button || in < 0.2)
    0 >> led; // another way to write "led.off()"
  // else combine both oscillators
  else
    (osc1 * osc2) >> led;
}
