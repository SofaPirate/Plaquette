DigitalIn
=========


Example code
------------

.. code-block:: c++

   DigitalIn button(2);

   DigitalOut led(13);

   void begin() {
     button.smooth(); // debounce button
   }

   void step() {
     button >> led;
   }


Class Reference
---------------

.. doxygenclass:: DigitalIn
   :project: Plaquette
   :members: DigitalIn, isOn, isOff, pin, mode, smooth, noSmooth, getInt, get

Related classes
---------------
- :doc:`AnalogIn`
- :doc:`DigitalOut`
