.. include:: defs.hrst

mapFloat()
==========

Re-maps a number from one range to another. That is, a value of ``fromLow`` would get
mapped to ``toLow``, a value of ``fromHigh`` to ``toHigh``, values in-between to values
in-between, etc.

Does not constrain values to within the range, because out-of-range values are sometimes
intended and useful. The ``constrain()`` function may be used either before or after this
function, if limits to the ranges are desired.

Note that the "lower bounds" of either range may be larger or smaller than the "upper bounds"
so the ``mapFloat()`` function may be used to reverse a range of numbers, for example

.. code-block:: c++

   y = mapFloat(x, 1.0, 50.0, 50.0, 1.1);

The function also handles negative numbers well, so that this example

.. code-block:: c++

   y = mapFloat(x, 1.0, 50.0, 50.0, -100.0);

is also valid and works well.

Unlike the Arduino `map() <https://www.arduino.cc/reference/en/language/functions/math/map/>`_ function,
``mapReal()`` uses floating-point math and *will* generate fractions.

|Example|
---------

.. code-block:: c++

   #include <Plaquette.h>

   SineOsc modulator(10.0);

   SquareOsc oscillator(1.0);

   DigitalOut led(13);

   void begin() {
   }

   void step() {
     // Change frequency of oscillator between 2Hz and 15Hz.
     float freq = mapFloat(modulator, 0.0, 1.0, 2.0, 15.0);
     oscillator.frequency(freq);
     // Send to LED.
     oscillator >> led;
   }


|Reference|
-----------

.. doxygenfunction:: mapFloat()
   :project: Plaquette

|SeeAlso|
---------
- :doc:`mapFrom01`
- :doc:`mapTo01`
