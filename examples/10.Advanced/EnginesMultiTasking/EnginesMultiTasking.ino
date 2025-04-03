/**
 * EnginesMultiTasking
 *
 * Demonstrates the use of multiple Plaquette engines. In this example, the primary
 * engine runs in the main loop and a secondary engine runs on Arduino timer2 interrupt
 * at 1kHz.
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The secondary timer engine.
Engine timerEngine;

// Metronome associated with the primary engine (default).
Metronome serialMetro(1.0);

// Metronome toggling the built-in LED, attached to the timer engine.
Metronome toggleMetro(0.25, timerEngine);

// Built-in LED, attached to the timer engine.
DigitalOut led(LED_BUILTIN, timerEngine);

void begin() {
  // Begin timer engine.
  timerEngine.begin();

  // Initialize timer interrupt timer2.
  timerSetup();
}

void step() {
  if (serialMetro)
    println("step");
}

void timerStep() {
  // Process code
  if (toggleMetro)
    led.toggle();
}

#if defined(__AVR_ATmega328P__)  // Arduino Uno, Nano, etc.

// Timer2 setup for 1kHz on AVR
void timerSetup() {
  // Stop Timer2
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2  = 0;

  // Set compare match register for 1 kHz increments
  // 16 MHz / (prescaler * 1000) - 1 = OCR2A
  // Try prescaler = 128 => OCR2A = (16e6 / (128 * 1000)) - 1 ≈ 124
  OCR2A = 124;

  // CTC mode (Clear Timer on Compare Match)
  TCCR2A |= (1 << WGM21);

  // Set prescaler to 128
  TCCR2B |= (1 << CS22) | (1 << CS20);  // 128 prescaler

  // Enable Timer2 compare interrupt
  TIMSK2 |= (1 << OCIE2A);

  // Enable global interrupts
  sei();
}

// Timer interrupt: will be called at 1kHz frequency.
ISR(TIMER2_COMPA_vect) {
  // Step engine.
  timerEngine.step();

  // Call timer engine step function.
  timerStep();
}

#elif defined(ESP32)

void IRAM_ATTR onTimer() {
  // Step engine.
  timerEngine.step();

  // Call timer engine step function.
  timerStep();
}

hw_timer_t *timer = NULL;

void timerSetup() {
  timer = timerBegin(0, 80, true);               // 80 prescaler = 1 MHz
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000, true);            // 1000 ticks = 1ms = 1kHz
  timerAlarmEnable(timer);
}

#else
// Default: without interrupts/timer support we use a 1kHz metronome to trigger timer engine..
Metronome timerMetro(0.001);

void onTimer() {
  // Step engine.
  timerEngine.step();

  // Call timer engine step function.
  timerStep();
}

void timerSetup() {
  timerMetro.onBang(onTimer);
}
#endif

