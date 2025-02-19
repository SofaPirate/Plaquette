.. include:: defs.hrst

DigitalOut
==========

A digital (ie. binary) output unit that can be switched "on" or "off".

The unit is assigned to a specific ``pin`` on the board.

The ``mode`` specifies the behavior of the component attached to the pin:

- in ``DIRECT`` mode (default) the pin acts as the source of current and the component is "on" when the pin is "high" (Vcc, typically 5V)
- in ``INVERTED`` mode the source of current is external (Vcc) and the component is "on" when the pin is "low" (GND)

|Example|
-------------

Switches off an LED connected in "sink" mode after a timeout.

.. code-block:: c++

   #include <Plaquette.h>

   DigitalOut led(13, INVERTED);

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
   :members: DigitalOut, on, off, toggle, isOn, isOff, smooth, noSmooth, get, getInt, put, pin, mode, rose, fell, changed, changeState, onRise, onFall, onChange, clearEvents

|SeeAlso|
---------
- :doc:`AnalogOut`
- :doc:`DigitalIn`
