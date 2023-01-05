.. include:: defs.hrst

mapFloat()
==========

Re-maps a number from one range to another. That is, a value of ``fromLow`` would get
mapped to ``toLow``, a value of ``fromHigh`` to ``toHigh``, and values in-between to values
in-between, proportionally.

.. code-block:: c++

   y = mapFloat(x, 10.0, 50.0, 100.0, 0.0);

The function also handles negative numbers well, so that this example

.. code-block:: c++

   y = mapFloat(x, 10.0, 50.0, 100.0, -100.0);

is also valid and works well.

By default, does *not* constrain output to stay within the [``fromHigh``, ``toHigh``] range, because 
out-of-range values are sometimes intended and useful. In order to constrain the return value within
range, use the ``CONSTRAIN`` argument as the last parameter:

.. code-block:: c++

   y = mapFloat(x, 10.0, 50.0, 100.0, -100.0, CONSTRAIN);


.. note::
   Note that the "lower bounds" (``fromLow`` and ``toLow``) of either range may be larger or smaller than 
   the "upper bounds" (``fromHigh`` and ``toHigh``) so the ``mapFloat()`` function may be used to reverse a
   range of numbers, for example


Unlike the Arduino `map() <https://www.arduino.cc/reference/en/language/functions/math/map/>`_ function,
``mapReal()`` uses floating-point math and *will* generate fractions.

|Example|
---------

.. code-block:: c++

   #include <Plaquette.h>

   SquareOsc oscillator(1.0);

   DigitalOut led(13);

   void begin() {
   }

   void step() {
     // Change frequency between 2Hz and 15Hz over a 30 seconds period, then the frequency will stay at 15Hz.
     float freq = mapFloat(seconds(), 0.0, 30.0, 2.0, 15.0, CONSTRAIN); // try removing CONSTRAIN and see what happens
     oscillator.frequency(freq);

     // Send to LED.
     oscillator >> led;
   }


|Reference|
-----------

.. doxygenfunction:: mapFloat(double, double, double, double, double, bool)
   :project: Plaquette

|SeeAlso|
---------
- :doc:`mapFrom01`
- :doc:`mapTo01`
