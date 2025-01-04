.. include:: defs.hrst

Chronometer
===========

An analog unit that counts time in seconds. It can be started, stopped, paused, and
resumed.

|Example|
---------

Uses a chronometer to change the frequency a blinking LED. Restarts after 10 seconds.

.. code-block:: c++

   #include <Plaquette.h>

   Chronometer chrono;

   DigitalOut led(13);

   SquareOsc osc(1.0); // a square oscillator

   void begin() {
     chrono.start(); // start chrono
   }

   void step() {
     // Adjust oscillator's duty cycle according to current timer progress.
     osc.frequency(chrono);

     // Apply oscillator to LED state.
     osc >> led;

     // If the chronometer reaches 10 seconds: restart it.
     if (chrono >= 10.0)
     {
       // Restarts the chronometer.
       chrono.start();
     }
   }


|Reference|
-----------

.. doxygenclass:: Chronometer
   :project: Plaquette
   :members: Chronometer, get, start, stop, pause, resume, elapsed, isRunning, add, set

|SeeAlso|
---------
- :doc:`Alarm`
- :doc:`Metronome`
- :doc:`Ramp`