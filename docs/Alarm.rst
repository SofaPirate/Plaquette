.. include:: defs.hrst

Alarm
=====

An alarm clock digital source unit. Counts time and becomes "on" when time is up.
The alarm can be started, stopped, and resumed.

When started, the alarm stays "off" until it reaches its timeout duration, after
which it becomes "on".

|Example|
---------

Uses an alarm to activate built-in LED. Button is used to reset the 
alarm at random periods of time.

.. code-block:: c++

   #include <Plaquette.h>

   Alarm myAlarm(2.0); // an alarm with 2 seconds duration

   DigitalOut led(13);

   DigitalIn button(2, INTERNAL_PULLUP);

   void begin() {
     myAlarm.start(); // start alarm
   }

   void step() {
     // Activate LED when alarm rings.
     myAlarm >> led; // the alarm will stay "on" until it is stopped or restarted

     // Reset alarm when button is pushed.
     if (myAlarm && button.rose())
     {
       // Restarts the alarm with a random duration between 1 and 5 seconds.
       myAlarm.duration(randomFloat(1.0, 5.0));
       myAlarm.start();
     }
   }


|Reference|
-----------

.. doxygenclass:: Alarm
   :project: Plaquette
   :members: Alarm, isOn, isOff, getInt, get, start, stop, resume, elapsed, progress, isStarted

|SeeAlso|
---------
- :doc:`Metro`
- :doc:`Ramp`
- :doc:`Timer`
- :doc:`SquareWave`
