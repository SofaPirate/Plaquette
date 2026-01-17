.. include:: defs.hrst

begin()
=======

The ``begin()`` function is optionally called at the start of a sketch to initialize
units, start using libraries, etc. The ``begin()`` function will only run once,
after each powerup or reset of the board.

.. hint::
  Function ``begin()`` is the Plaquette equivalent of Arduino's
  `setup() <https://www.arduino.cc/reference/en/language/functions/setup/>`_.
  However, Plaquette takes care of many of the initialization calls that need to
  be done in Arduino such as ``pinMode()``. Therefore in many cases it will
  contain only a few calls, or can be omitted completely.

|Example|
---------

.. code-block:: c++

   #include <Plaquette.h>

   Wave oscillator(1.0);
   AnalogIn input(A0);

   void begin() {
     oscillator.period(1.0);
     oscillator.skew(0.75);
     input.smooth();
   }

   void step() {
     // ...
   }

.. tip::

  In Plaquette, function ``begin()`` is **optional**: only declare it if you need to perform a
  specific operation at startup.

|Example|
---------

.. code-block:: c++

   #include <Plaquette.h>

   Wave oscillator(1.0);
   Plotter plotter(115200);

   // begin() function is not declared

   void step() {
     oscillator >> plotter; // Send the wave to the plotter for visualization
   }

|SeeAlso|
---------
- :doc:`prepare`
- :doc:`step`
