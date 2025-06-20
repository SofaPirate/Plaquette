.. include:: defs.hrst

SineWave
========

A source unit that can generate a sinusoid or `sine wave <https://en.wikipedia.org/wiki/Sine_wave>`_.
The signal is remapped to oscillate between 0 and 1 (rather than -1 and 1 as the traditional sine wave
function). It can be tuned by adjusting parameters such as ``period``, ``frequency``,
``amplitude``, or ``skew``.

.. image:: images/Plaquette-SineWave.png

.. tip::
  The ``skew`` parameter is included for consistency with triangle and square wave units. It controls when
  the sine wave reaches its peak within a cycle. A skew value of 0.5 (default) yields a standard symmetric
  sine wave. Lower values shift the peak earlier (left-skewed), while higher values shift it later (right-skewed),
  allowing for asymmetric sine shapes while preserving smoothness.

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
   :members:

|SeeAlso|
---------
- :doc:`SquareWave`
- :doc:`TriangleWave`
