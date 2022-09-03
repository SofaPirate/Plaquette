.. include:: defs.hrst

mapTo01()
=========

Re-maps a number to range [0, 1]. That is, a value of ``fromLow`` would get
mapped to 0, a value of ``fromHigh`` to 1, values in-between to values in-between, etc.

.. code-block:: c++

   mapTo01(x, fromLow, fromHigh)

is equivalent to:

.. code-block:: c++

   y = mapFloat(x, fromLow, fromHigh, 0, 1)

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
     // Remap to [0, 1] and send to LED.
     mapTo01(x, -1, 1) >> led;
   }


|Reference|
-----------

.. doxygenfunction:: mapTo01()
   :project: Plaquette

|SeeAlso|
---------
- :doc:`mapFloat`
- :doc:`mapFrom01`
