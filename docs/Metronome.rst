.. include:: defs.hrst

Metronome
=====

A metronome digital source unit. Emits an "on" signal at a regular pace.

|Example|
---------

.. code-block:: c++

   #include <Plaquette.h>

   Metronome myMetro(0.5); // a metronome with a half-second duration

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

.. doxygenclass:: Metronome
   :project: Plaquette
   :members: Metronome, isOn, isOff, getInt, get, rose, fell, changed, changeState, period, frequency, bpm, phase

|SeeAlso|
---------
- :doc:`Ramp`
- :doc:`SquareWave`
- :doc:`Timer`
