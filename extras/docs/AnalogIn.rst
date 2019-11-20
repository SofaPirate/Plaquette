.. include:: defs.hrst

AnalogIn
========

An analog (ie. continuous) input unit that returns values between 0 and 1 (ie. 0% and 100%).

The unit is assigned to a specific ``pin`` on the board.

The ``mode`` specifies the behavior of the component attached to the pin:

- in ``ANALOG_DEFAULT`` mode (default) the value is expressed as a percentage of the reference voltage (Vref, typically 5V)
- in ``ANALOG_INVERTED`` mode the value is inverted (ie. 0V corresponds to 100% while 2.5V corresponds to 50%).

|Example|
---------

Control an LED using a potentiometer.

.. code-block:: c++

   #include <Plaquette.h>

   AnalogIn potentiometer(A0);

   AnalogOut led(9);

   void begin() {}

   void step() {
     // The analog input controls the intensity of the LED.
     potentiometer >> led;
   }


|Reference|
-----------

.. doxygenclass:: AnalogIn
   :project: Plaquette
   :members: AnalogIn, get, mapTo, smooth, noSmooth, cutoff, pin, mode

.. warning::

   If the analog input pin is not connected to anything, the value returned by ``get()`` will fluctuate based on a number of factors (e.g. the values of the other analog inputs, how close your hand is to the board, etc.).

|SeeAlso|
---------
- :doc:`AnalogOut`
- :doc:`DigitalIn`
