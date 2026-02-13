.. include:: defs.hrst

Alarm
=====

An alarm clock digital source unit. Counts time and becomes "on" when time is up.
The alarm can be started, stopped, and resumed.

When started, the alarm stays "off" until it reaches its timeout duration, after
which it becomes "on".

Parameters
----------

.. list-table::
   :header-rows: 1
   :widths: 12 33 9 16 15 15

   * - Name
     - Description
     - Range
     - Setter
     - Getter
     - Flow
   * - duration
     - Time before the alarm triggers (in seconds).
     - > 0
     - ``duration(v)``
     - ``duration()``
     - ``Duration()``

Events
------

.. list-table::
   :header-rows: 1
   :widths: 50 25 25

   * - When
     - Trigger
     - Callback
   * - Digital value toggles (on/off)
     - ``changed()``
     - ``onChange(c)``
   * - Digital value goes from on to off
     - ``fell()``
     - ``onFall(c)``
   * - Timed process completes
     - ``finished()``
     - ``onFinish(c)``
   * - Digital value goes from off to on
     - ``rose()``
     - ``onRise(c)``

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
   :members:

|SeeAlso|
---------
- :doc:`Chronometer`
- :doc:`Metronome`
- :doc:`Ramp`
- :doc:`Wave`
