.. include:: defs.hrst

wrap()
======

Restricts a value to an interval [low, high) by wrapping it around.

+----------------------------+-----------+
| Code                       | Result    |
+============================+===========+
| ``wrap(1.0, 1.0, 5.0)``    | ``1.0``   |
+----------------------------+-----------+
| ``wrap(3.0, 1.0, 5.0)``    | ``3.0``   |
+----------------------------+-----------+
| ``wrap(4.9999, 1.0, 5.0)`` | ``4.9999``|
+----------------------------+-----------+
| ``wrap(5.0, 1.0, 5.0)``    | ``1.0``   |
+----------------------------+-----------+
| ``wrap(6.0, 1.0, 5.0)``    | ``2.0``   |
+----------------------------+-----------+
| ``wrap(-1.0, 1.0, 5.0)``   | ``3.0``   |
+----------------------------+-----------+

Two alternative versions are provided:

+-----------------------------+---------------------------+
| Version                     | Equivalent to             |
+=============================+===========================+
| ``wrap(x, high)``           | ``wrap(x, 0.0, high)``    |
+-----------------------------+---------------------------+
| ``wrap01(x)``               | ``wrap(x, 0.0, 1.0)``     |
+-----------------------------+---------------------------+

|Example|
---------

Ramp LED up and then back to zero once every 10 second:

.. code-block:: c++

   #include <Plaquette.h>

   AnalogOut led(9);

   void begin() {
   }

   void step() {
      wrap(seconds(), 10.0) >> led;
   }


|Reference|
-----------

.. doxygenfunction:: wrap(double, double, double)
   :project: Plaquette

.. doxygenfunction:: wrap(double, double)
   :project: Plaquette

.. doxygenfunction:: wrap01(double)
   :project: Plaquette

|SeeAlso|
---------
- :doc:`mapFloat`
- :doc:`mapTo01`
