.. include:: defs.hrst

Alarm
=====

An alarm clock digital source unit. Counts time and becomes "on" when time is up.
The alarm can be started, stopped, and resumed.

When started, the alarm stays "off" until it reaches its timeout duration, after
which it becomes "on".

|Example|
---------

Uses an alarm to change the state of built-in LED at random periods of time.

.. code-block:: c++

   #include <Plaquette.h>

   Alarm myAlarm(2.0); // an alarm with 2 seconds duration

   DigitalOut led(13);

   void begin() {
     myAlarm.start(); // start alarm
   }

   void step() {
     if (myAlarm) // the alarm will stay "on" until it is stopped or restarted
     {
       // Change LED state.
       led.toggle();

       // Restarts the timer with a random duration between 1 and 5 seconds.
       myAlarm.duration(randomFloat(1.0, 5.0));
       myAlarm.start();
     }
   }


|Reference|
-----------

.. doxygenclass:: Alarm
   :project: Plaquette
   :members: Alarm, isOn, isOff, getInt, get, rose, fell, changed, changeState, start, stop, resume, elapsed, progress, isStarted

|SeeAlso|
---------
- :doc:`Ramp`
- :doc:`Timer`
- :doc:`SquareOsc`
