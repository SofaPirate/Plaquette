.. include:: defs.hrst

SineWave
=======

A source unit that can generate a sinusoid or `sine wave <https://en.wikipedia.org/wiki/Sine_wave>`_.
The signal is remapped to oscillate between 0 and 1 (rather than -1 and 1 as
the traditional sine wave).

.. image:: images/Plaquette-SineWave.png

|Example|
---------

Pulses an LED.

.. code-block:: c++

   #include <Plaquette.h>

   AnalogOut led(9);

   SineWave osc;

   void begin() {
     osc.frequency(5.0); // frequency of 5 Hz
   }

   void step() {
     osc >> led;
   }

.. doxygenclass:: SineWave
   :project: Plaquette
   :members: SineWave, get, period, frequency, phase, amplitude, bpm, mapTo, shiftBy

|SeeAlso|
---------
- :doc:`SquareWave`
- :doc:`TriangleWave`
