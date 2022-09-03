.. include:: defs.hrst

AnalogOut
=========

An analog (ie. continuous) output unit that takes a value between 0 and 1 (ie. 0% and 100%).

The unit is assigned to a specific ``pin`` on the board.

The ``mode`` specifies the behavior of the component attached to the pin:

- in ``SOURCE`` mode (default) the pin acts as the source of current and the value is expressed as a percentage of the maximum voltage (Vcc, typically 5V)
- in ``SINK`` mode the component the source of current is external and should be equal to Vcc

|Example|
---------

.. code-block:: c++

   AnalogOut led(9);

   void begin() {
     led.put(0.5);
   }

   void step() {
     // The LED value is changed randomly by a tiny amount (random walk).
     // Mutliplying by samplePeriod() makes sure the rate of change stays stable.
     (led + randomFloat(-0.1, 0.1) * samplePeriod()) >> led;
   }

|Reference|
-----------

.. doxygenclass:: AnalogOut
   :project: Plaquette
   :members: AnalogOut, get, put, invert, pin, mode


.. note::

    On most Arduino boards analog outputs rely on `Pulse Width Modulation (PWM) <https://www.arduino.cc/en/Tutorial/PWM>`_.
    After a call to ``put(value)``, the pin will generate a steady square wave of the
    specified duty cycle until the next call to ``put()`` on the same pin. The frequency
    of the PWM signal on most pins is approximately 490 Hz. On the Uno and similar boards,
    pins 5 and 6 have a frequency of approximately 980 Hz.

.. note::
    On most Arduino boards (those with the ATmega168 or ATmega328P), this functionality
    works on pins 3, 5, 6, 9, 10, and 11. On the Arduino Mega, it works on pins
    2 - 13 and 44 - 46. Older Arduino boards with an ATmega8 only support ``AnalogOut``
    on pins 9, 10, and 11. The Arduino DUE supports analog output on pins 2 through 13,
    plus pins DAC0 and DAC1. Unlike the PWM pins, DAC0 and DAC1 are Digital to Analog
    converters, and act as true analog outputs.

|SeeAlso|
---------
- :doc:`AnalogIn`
- :doc:`DigitalOut`
