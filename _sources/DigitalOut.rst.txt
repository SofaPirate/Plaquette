DigitalOut
==========

Represents a digital (ie. binary) output pin that can be either "on" or "off".

The unit is assigned to a specific ``pin`` on the board.

The ``mode`` specifies the behavior of the component attached to the pin:

- in ``SOURCE`` mode the pin acts as the source of current and the component is "on" when the pin is "high" (ie. 3.3V or 5V)
- in ``SINK`` mode the component is "on" when the pin is "low" (ie. 0V / ground)

Example code
------------

.. code-block:: c++

   DigitalOut led(13, SOURCE);

   void begin() {
     led.on();
   }

   void step() {
     // Switch the LED off after 5 seconds.
     if (seconds() > 5)
       led.off();
   }


Class Reference
---------------

.. doxygenclass:: DigitalOut
   :project: Plaquette
   :members: DigitalOut, on, off, toggle, isOn, isOff, smooth, noSmooth, get, getInt, put, pin, mode

Related classes
---------------
- :doc:`AnalogOut`
- :doc:`DigitalIn`
