.. include:: defs.hrst

begin()
=======

The ``begin()`` function is called when a sketch starts. Use it to initialize
units, start using libraries, etc. The ``begin()`` function will only run once,
after each powerup or reset of the board.

.. hint::
  Function ``begin()`` is the Plaquette equivalent of Arduino's
  `setup() <https://www.arduino.cc/reference/en/language/functions/setup/>`_.
  However, Plaquette takes care of many of the initialization calls that need to
  be done in Arduino such as ``pinMode()``. Therefore in many cases it will
  contain only a few calls or even be left empty.

|Example|
---------

.. code-block:: c++

   #include <Plaquette.h>

   SquareWave oscillator;
   AnalogIn input(A0);

   void begin() {
     oscillator.period(1.0);
     oscillator.width(0.75);
     input.smooth();
   }

   void step() {
     // ...
   }

|SeeAlso|
---------
- :doc:`step`
