.. include:: defs.hrst

mapTo01()
=========

Re-maps a number between 0.0 and 1.0. That is, a value of ``fromLow`` would get
mapped to 0.0, a value of ``fromHigh`` to 1.0, values in-between to values in-between, etc.

.. code-block:: c++

   mapTo01(x, fromLow, fromHigh)

is equivalent to:

.. code-block:: c++

   mapFloat(x, fromLow, fromHigh, 0, 1)

By default, does *not* constrain output to stay within the [``fromHigh``, ``toHigh``] range, because 
out-of-range values are sometimes intended and useful. In order to constrain the return value within
range, use the ``CONSTRAIN`` argument as the last parameter:

.. code-block:: c++

   mapTo01(x, fromLow, fromHigh, CONSTRAIN)

See :doc:`mapFloat` for more details.

|Example|
---------

.. code-block:: c++

   #include <Plaquette.h>

   AnalogOut led(9);

   void begin() {
   }

   void step() {
     // Generate a sinusoidal values between -1 and 1.
     float x = sin(seconds());

     // Remap to the range [0, 1] and send to LED.
     mapTo01(x, -1, 1) >> led;
   }


|Reference|
-----------

.. doxygenfunction:: mapTo01(double, double, double, uint8_t)
   :project: Plaquette

|SeeAlso|
---------
- :doc:`mapFloat`
- :doc:`mapFrom01`
