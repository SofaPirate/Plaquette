.. include:: defs.hrst

>> (pipe)
=========

Sends data across units from left to right. This operator is specific to Plaquette
and can be used in a chained manner.

The operation uses the ``get()`` and ``put()`` methods of units in such a way that:

.. code-block:: c++

    input >> output;

is equivalent to:

.. code-block:: c++

    output.put(input.get());

Numerical and boolean values can also be used:

.. code-block:: c++

    12 >> output;
    0.8 >> output;
    true >> output;

|Example|
---------

.. code-block:: c++

   #include <Plaquette.h>

   AnalogIn sensor(A0);

   MinMaxScaler scaler;

   AnalogOut led(9);

   void begin() {}

   void step() {
     // Rescale value and send the result to LED.
     sensor >> scaler >> led;
   }

|Syntax|
--------

.. code-block:: c++

   input >> output
   input >> filter >> output
