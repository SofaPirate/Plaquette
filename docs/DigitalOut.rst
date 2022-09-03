.. include:: defs.hrst

DigitalOut
==========

A digital (ie. binary) output unit that can be switched "on" or "off".

The unit is assigned to a specific ``pin`` on the board.

The ``mode`` specifies the behavior of the component attached to the pin:

- in ``SOURCE`` mode (default) the pin acts as the source of current and the component is "on" when the pin is "high" (Vcc)
- in ``SINK`` mode the source of current is external and the component is "on" when the pin is "low" (GND)

|Example|
-------------

Switches off an LED connected in "sink" mode after a timeout.

.. code-block:: c++

   #include <Plaquette.h>

   DigitalOut led(13, SINK);

   void begin() {
     led.on();
   }

   void step() {
     // Switch the LED off after 5 seconds.
     if (seconds() > 5)
       led.off();
   }


|Reference|
-----------

.. doxygenclass:: DigitalOut
   :project: Plaquette
   :members: DigitalOut, on, off, toggle, isOn, isOff, smooth, noSmooth, get, getInt, put, pin, mode

|SeeAlso|
---------
- :doc:`AnalogOut`
- :doc:`DigitalIn`
