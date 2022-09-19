.. include:: defs.hrst

Timer
=====

A timer digital source unit that counts time. The timer can be started, stopped, and
resumed.

When started, the timer goes from 0 to 1 through its duration.

|Example|
---------

Uses a timer to change the duty cycle of a blinking LED, then restarts with a
new random duration.

.. code-block:: c++

   #include <Plaquette.h>

   Timer myTimer(2.0); // a timer with 2 seconds duration

   DigitalOut led(13);

   SquareOsc osc(3.0); // a square oscillator with a 3 seconds period

   void begin() {
     myTimer.start(); // start timer
   }

   void step() {
     // Adjust oscillator's duty cycle according to current timer progress.
     osc.dutyCycle(myTimer);

     // Apply oscillator to LED state.
     osc >> led;

     if (myTimer.isComplete()) // if the timer has completed its course
     {
       // Restarts the timer with a random duration between 1 and 5 seconds.
       myTimer.start(randomFloat(1.0, 5.0));
     }
   }


|Reference|
-----------

.. doxygenclass:: Timer
   :project: Plaquette
   :members: Timer, get, start, stop, resume, elapsed, isStarted, isComplete

|SeeAlso|
---------
- :doc:`Alarm`
- :doc:`Metro`
- :doc:`Ramp`
- :doc:`TriOsc`
