.. include:: defs.hrst

[] (arrays)
===========

An array is a collection of variables or objects that are accessed with an index number. 
Arrays can be complicated, but using simple arrays is relatively straightforward.

For a general description of arrays, please refer to `this page <https://www.arduino.cc/reference/en/language/variables/data-types/array/>`_.

Arrays of Plaquette units such as :doc:`DigitalIn`. :doc:`SineWave`, and :doc:`MinMaxScaler` can be easily created using the following syntax:

.. code-block:: c++

    UnitType array[] = { UnitType(...), UnitType(...), ... };

For example, the following code will create an array of three (3) digital outputs on pins 10, 11, and 12:

.. code-block:: c++

    DigitalOut leds[] = { DigitalOut(10), DigitalOut(11), DigitalOut(12) };

When initializing a unit with a single parameter, one can simply use the value of the parameter at creation time. Hence the previous code could
be rewritten as:

.. code-block:: c++

    DigitalOut leds[] = { 10, 11, 12 };

When more than a single parameter is used, however, it needs to be called explicitely with the unit name:

.. code-block:: c++

    SquareWave oscillators[] = { 1.0, 2.0, SquareWave(3.0, 0.8) };

.. warning::

    Units in array need to be all of the same type. In other words, it is not currently possible to mix different types of objects such as 
    DigitalIn and SquareWave in the same array.


|Example|
---------

.. code-block:: c++

  #include <Plaquette.h>

  AnalogOut leds[] = { 9, 10, 11 };

  // Creates three triangle oscillators with a 2 seconds period, with different width.
  TriangleWave oscillators[] = { TriangleWave(2.0, 0.0), 2.0, TriangleWave(2.0, 1.0) };

  void begin() {}

  void step() {
    // Send each oscillator to its corresponding LED.
    for (int i=0; i<3; i++) {
      oscillators[i] >> leds[i];
    }
  }

