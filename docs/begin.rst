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

|Examples|
---------

.. code-block:: c++

   #include <Plaquette.h>

   Wave oscillator;
   AnalogIn input(A0);

   void begin() {
     oscillator.period(1.0);
     oscillator.skew(0.75);
     input.smooth();
   }

   void step() {
     // ...
   }

   .. code-block:: c++

   #include <Plaquette.h>

   Wave oscillator(1.0); // create a square wave with a period of 1 second
   Plotter plotter(115200); // create a plotter to visualize the wave

   //notice that since there is nothing to instantiate in this case, the begin() function is
   //unneccessary

   void step() {
     oscillator >> plotter; // Send the wave to the plotter for visualization
   }

|SeeAlso|
---------
- :doc:`step`
