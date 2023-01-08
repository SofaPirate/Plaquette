.. include:: defs.hrst

DigitalIn
=========

A digital (ie. binary) input unit that can be either "on" or "off".

The unit is assigned to a specific ``pin`` on the board.

The ``mode`` specifies the behavior of the component attached to the pin:

- in ``DEFAULT`` mode (default) the unit will be "on" when the voltage on the pin is high (Vref, typically 5V)
- in ``INVERTED`` mode the unit will be "on" when the voltage on the pin is low (GND)
- in ``PULLUP`` mode the internal 20K pullup resistor is used, which simplifies the use of switches and buttons

Debouncing
----------

Some digital inputs such as `push-buttons <https://en.wikipedia.org/wiki/Push-button>`_ often
generate spurious open/close transitions when pressed, due to mechanical and physical issues:
these transitions called "bouncing" may be read as multiple presses in a very short time, fooling
the program.

The DigitalIn object features debouncing capabilities which can prevent this kind of problems. Debouncing
can be achieved using different modes: stable (default) (``DEBOUNCE_STABLE``), lock-out (``DEBOUNCE_LOCK_OUT``)
and prompt-detect (``DEBOUNCE_PROMPT_DETECT``). For more information please refer to the documentation
of the `Bounce2 Arduino Library <https://github.com/thomasfredericks/Bounce2>`_.

|Example|
---------

Turns on and off a light emitting diode (LED) connected to digital pin 13, when
pressing a pushbutton attached to digital pin 2.

.. code-block:: c++

   #include <Plaquette.h>

   DigitalIn button(2, INTERNAL_PULLUP);

   DigitalOut led(13);

   void begin() {
     button.debounce(); // debounce button
   }

   void step() {
     // Toggle the LED each time the button is pressed.
     if (button.rose())
       led.toggle();
   }


|Reference|
-----------

.. doxygenclass:: DigitalIn
   :project: Plaquette
   :members: DigitalIn, isOn, isOff, pin, mode, getInt, get, debounce, noDebounce, debounceMode, rose, fell, changed, changeState

|SeeAlso|
---------
- :doc:`AnalogIn`
- :doc:`DigitalOut`
- `Bounce2 Arduino Library <https://github.com/thomasfredericks/Bounce2>`_
