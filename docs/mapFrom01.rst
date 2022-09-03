.. include:: defs.hrst

mapFrom01()
===========

Re-maps a number in the range [0, 1] to another range. That is, a value of 0 would get
mapped to ``toLow``, a value of 1 to ``toHigh``, values in-between to values in-between, etc.

.. code-block:: c++

   mapFrom01(x, toLow, toHigh)

is equivalent to:

.. code-block:: c++

   y = mapFloat(x, 0, 1, toLow, toHigh)

See :doc:`mapFloat` for more details.

|Example|
-------------

.. code-block:: c++

   #include <Plaquette.h>

   SineOsc modulator(10.0);

   SquareOsc oscillator(1.0);

   DigitalOut led(13);

   void begin() {
   }

   void step() {
     // Change duty-cycle of oscillator in range [0.2, 0.8].
     oscillator.dutyCycle(mapFrom01(modulator, 0.2, 0.8));
     // Send to LED.
     oscillator >> led;
   }


|Reference|
-----------

.. doxygenfunction:: mapFrom01()
   :project: Plaquette

|SeeAlso|
---------
- :doc:`mapFloat`
- :doc:`mapTo01`
