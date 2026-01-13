
.. include:: defs.hrst

seconds()
=========

This function returns the number of seconds since the program started.

|Example|
-------------

.. code-block:: c++

   #include <Plaquette.h>

   DigitalOut led(13, DIRECT);

   void begin() {
   // Initialize LED as "off".
     led.off();
   }

   void step() {
     // Switch the LED on after 10 seconds.
     if (seconds() > 10)
       led.on();
   }


|Reference|
----------------

.. doxygenfunction:: pq::seconds
   :project: Plaquette

|SeeAlso|
---------
- `micros() <https://www.arduino.cc/reference/en/language/functions/time/micros/>`_
- `millis() <https://www.arduino.cc/reference/en/language/functions/time/millis/>`_
