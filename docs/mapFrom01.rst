.. include:: defs.hrst

mapFrom01()
===========

Re-maps a number in the range [0, 1] to another range. That is, a value of 0 would get
mapped to ``toLow``, a value of 1 to ``toHigh``, values in-between to values in-between, etc.

.. code-block:: c++

   mapFrom01(x, toLow, toHigh)

is equivalent to:

.. code-block:: c++

   mapFloat(x, 0, 1, toLow, toHigh)

By default, does *not* constrain output to stay within the [``fromHigh``, ``toHigh``] range, because
out-of-range values are sometimes intended and useful. In order to constrain the return value within
range, use the ``CONSTRAIN`` argument as the last parameter:

.. code-block:: c++

   mapFrom01(x, toLow, toHigh, CONSTRAIN)

See :doc:`mapFloat` for more details.

|Example|
-------------

.. code-block:: c++

   #include <Plaquette.h>

   SineWave modulator(10.0);

   SquareWave oscillator(1.0);

   DigitalOut led(13);

   void begin() {
   }

   void step() {
     // Change duty-cycle of oscillator in range [0.2, 0.8].
     float skew = mapFrom01(modulator, 0.2, 0.8); // alternative: modulator.mapTo(0.2, 0.8)
     oscillator.skew(skew);

     // Send to LED.
     oscillator >> led;
   }


|Reference|
-----------

.. doxygenfunction:: mapFrom01(double, double, double, MapMode)
   :project: Plaquette

|SeeAlso|
---------
- :doc:`mapFloat`
- :doc:`mapTo01`
