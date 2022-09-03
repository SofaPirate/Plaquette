.. include:: defs.hrst

Metro
=====

A digital source unit that emits an "on" signal at a regular pace.

|Example|
---------

.. code-block:: c++

   #include <Plaquette.h>

   Metro myMetro(0.5); // a metronome with a half-second duration

   DigitalOut led(13);

   void begin() {
   }

   void step() {
     if (myMetro)
     {
       // Change LED state.
       led.toggle();
     }
   }


|Reference|
-----------

.. doxygenclass:: Metro
   :project: Plaquette
   :members: Metro, isOn, isOff, getInt, get, rose, fell, changed, changeState, period, frequency, phase

|SeeAlso|
---------
- :doc:`Ramp`
- :doc:`SquareOsc`
