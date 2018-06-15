.. include:: defs.hrst

DigitalIn
=========

A digital (ie. binary) input unit that can be either "on" or "off".

The unit is assigned to a specific ``pin`` on the board.

The ``mode`` specifies the behavior of the component attached to the pin:

- in ``INTERNAL_PULLUP`` mode (default) the internal 20K pullup resistor is used
- in ``EXTERNAL_PULLUP`` mode you need to use an external pullup resistor connected to Vcc
- in ``EXTERNAL_PULLDOWN`` mode you need to use an external pulldown resistor connected to GND

|Example|
---------

.. code-block:: c++

   DigitalIn button(2);

   DigitalOut led(13);

   void begin() {
     button.smooth(); // debounce button
   }

   void step() {
     button >> led;
   }


|Reference|
-----------

.. doxygenclass:: DigitalIn
   :project: Plaquette
   :members: DigitalIn, isOn, isOff, pin, mode, getInt, get, rose, fell, changed, changeState

|SeeAlso|
---------
- :doc:`AnalogIn`
- :doc:`DigitalOut`
