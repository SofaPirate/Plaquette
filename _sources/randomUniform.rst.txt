.. include:: defs.hrst

randomUniform()
===============

This function returns a random real-valued number.

|Example|
-------------

.. code-block:: c++

   DigitalOut led(13);

   void begin() {
   }

   void step() {
     // 2% probability to toggle the LED
     if (randomUniform() < 0.02)
       led.toggle();
   }


|Reference|
----------------

.. doxygenfunction:: randomUniform()
   :project: Plaquette

.. doxygenfunction:: randomUniform(float)
   :project: Plaquette

.. doxygenfunction:: randomUniform(float, float)
   :project: Plaquette

|SeeAlso|
---------
- `random() <https://www.arduino.cc/reference/en/language/functions/random-numbers/random/>`_
