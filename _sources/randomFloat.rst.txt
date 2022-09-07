.. include:: defs.hrst

randomFloat()
=============

This function returns a random real-valued number.

|Example|
-------------

.. code-block:: c++

   #include <Plaquette.h>

   DigitalOut led(13);

   void begin() {
   }

   void step() {
     // 2% probability to toggle the LED
     if (randomFloat() < 0.02)
       led.toggle();
   }


|Reference|
----------------

.. doxygenfunction:: randomFloat()
   :project: Plaquette

.. doxygenfunction:: randomFloat(float)
   :project: Plaquette

.. doxygenfunction:: randomFloat(float, float)
   :project: Plaquette

|SeeAlso|
---------
- `random() <https://www.arduino.cc/reference/en/language/functions/random-numbers/random/>`_
