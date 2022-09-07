.. include:: defs.hrst

Timer
=====

A digital source unit that counts time. The timer can be started, stopped, and
resumed.

When started, the timer stays "off" until it reaches its timeout duration, after
which it becomes "on".

|Example|
---------

Uses a timer to change the state of built-in LED at random periods of time.

.. code-block:: c++

   #include <Plaquette.h>

   Timer myTimer(2.0); // a chronometer with 2 seconds duration

   DigitalOut led(13);

   void begin() {
     myTimer.start(); // start timer
   }

   void step() {
     if (myTimer) // the timer will stay "on" until it is stopped or restarted
     {
       // Change LED state.
       led.toggle();

       // Restarts the timer with a random duration between 1 and 5 seconds.
       myTimer.duration(randomFloat(1.0, 5.0));
       myTimer.start();
     }
   }


|Reference|
-----------

.. doxygenclass:: Timer
   :project: Plaquette
   :members: Timer, isOn, isOff, getInt, get, rose, fell, changed, changeState, start, stop, resume, elapsed, progress, isStarted

|SeeAlso|
---------
- :doc:`Ramp`
- :doc:`SquareOsc`
