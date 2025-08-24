.. include:: defs.hrst

randomTrigger()
===============

Interactive designers commonly want some events to happen **occasionally**, not on every step, in a
random fashion. This function makes that easy: it gives you a simple yes/no (true/false) answer
that, when checked in each step(), will trigger roughly once per second, minute, or any other time
window that is specified.

You can think of it as a “random but controlled” switch: you decide how sparse or frequent the events
should be (for example, about once every 5 seconds), and the function takes care of the rest, even if
your loop runs very fast.

|Example|
---------

.. code-block:: c++

   #include <Plaquette.h>

   DigitalOut led(13);

   void begin() {
   }

   void step() {
     // Returns true on average once every 5 seconds.
     if (randomTrigger(5.0))
       led.toggle();
   }


|Reference|
-----------

.. doxygenfunction:: randomTrigger(float)
   :project: Plaquette

.. doxygenfunction:: randomTrigger(float, float)
   :project: Plaquette

|SeeAlso|
---------
- `random() <https://www.arduino.cc/reference/en/language/functions/random-numbers/random/>`_
- :doc:`randomFloat``
