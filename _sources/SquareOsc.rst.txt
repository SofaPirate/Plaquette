.. include:: defs.hrst

SquareOsc
=========

A source unit that generates a `square wave <https://en.wikipedia.org/wiki/Square_wave>`_
signal. The signal can be tuned by changing the ``period`` and/or ``frequency``
of the oscillation, as well as the ``duty cycle`` -- which represents the percentage
of time during which the wave is "on" or "high".

|Example|
---------

This code will make the built-in LED blink with a period of 2 seconds.

.. code-block:: c++

   DigitalOut led(13);

   SquareOsc blinkOsc(2.0);

   void begin() {
     blinkOsc.dutyCycle(0.25); // Sets the duty cycle to 25%
   }

   void step() {
     blinkOsc >> led;
   }

.. doxygenclass:: SquareOsc
   :project: Plaquette
   :members: SquareOsc, isOn, isOff, period, frequency, dutyCycle, getInt, get

|SeeAlso|
---------
- :doc:`SineOsc`
- :doc:`TriOsc`
