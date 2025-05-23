.. include:: defs.hrst

Engine
======

A control structure that acts like the **conductor of an orchestra**, managing an ensemble of **units**. 
It handles their initialization, updates, and timing, ensuring that all components remain synchronized.

By default, all units are automatically added to the **primary engine** which can be accessed using the
global object ``Plaquette``. By using **secondary engines** you can organize and optimize your code, 
allowing for multi-tasking, grouping units, switching between ensembles of units, and save power by
running engines at lower frequency.

Usage
-----

To create and use an engine, simply declare it:

.. code-block:: c++

   Engine myEngine;

You will need to call the engine's ``begin()`` function at initialization, and then its ``step()`` function
at a regular pace.

To assign units to a specific engine, add it as the last parameter at construction:

.. code-block:: c++

   SquareWave wave(1.0, 0.2, myEngine); // Use myEngine instead of the default

Each engine provides its own time measurements:

.. code-block:: c++

   float sec = myEngine.seconds();        // Time since engine started, in seconds
   uint32_t ms = myEngine.milliSeconds(); // Time in milliseconds
   uint64_t us = myEngine.microSeconds(); // Time in microseconds

For more in-depth explanations and examples please read :ref:`secondary-engines`.

|Example|
---------

This example demonstrates the use of a secondary engine on an Arduino Uno or Nano using
timer2 interrupt.

.. code-block:: c++

  #include <Plaquette.h>

  Engine timerEngine; // The secondary timer engine.

  Metronome serialMetro(1.0); // Metronome (primary engine).

  Metronome toggleMetro(0.25, timerEngine); // Metronome (timer engine).
  DigitalOut led(LED_BUILTIN, timerEngine); // Built-in LED (timer engine).

  // Primary engine begin().
  void begin() {
    timerEngine.begin(); // Begin timer engine.
    timerSetup();        // Initialize timer interrupt timer2.
  }

  // Primary engine step().
  void step() {
    if (serialMetro)
      println("step");
  }

  // Timer2 interrupt: will be called at 1kHz frequency.
  ISR(TIMER2_COMPA_vect) {
    timerEngine.step(); // Step engine.
    
    if (toggleMetro) // Toggle LED on metro bang.
      led.toggle();
  }

  // Timer2 setup for 1kHz on AVR.
  void timerSetup() {
    // Stop Timer2
    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2  = 0;

    // Set compare match register for 1 kHz increments
    // 16 MHz / (prescaler * 1000) - 1 = OCR2A
    OCR2A = 124;

    TCCR2A |= (1 << WGM21); // CTC mode (Clear Timer on Compare Match)
    TCCR2B |= (1 << CS22) | (1 << CS20); // Set prescaler to 128
    TIMSK2 |= (1 << OCIE2A); // Enable Timer2 compare interrupt

    sei(); // Enable global interrupts
  }

|Reference|
----------

.. doxygenclass:: Engine
   :project: Plaquette
   :members: Engine, begin, step, nUnits, seconds, milliSeconds, microSeconds, nSteps, sampleRate, samplePeriod, primary, isPrimary

|SeeAlso|
---------

- :doc:`begin`
- :doc:`step`
- :doc:`seconds`
- :ref:`secondary-engines`
