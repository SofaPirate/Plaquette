.. include:: defs.hrst

prepare()
=========

The ``prepare()`` function is an optional function that is called at the very beginning
of a sketch, before any units are initialized. Use it when you need to perform operations that
must happen before Plaquette units call their ``begin()`` methods, typically when using
non-Plaquette objects or functions that need to be built or configured before units
are initialized.

.. tip::
  In Plaquette, function ``prepare()`` is rarely used and is **optional**: only declare it
  if you need to perform operations before units are initialized.

|Example|
---------

.. code-block:: c++

   #include <Plaquette.h>

   AnalogIn sensor(A0);
   DigitalOut led(13);

   void prepare() {
     // Early setup before units initialize.
     Serial.begin(115200);
   }

   void begin() {
     // Runs after units are initialized.
     sensor.smooth();
   }

   void step() {
     sensor >> led;
   }

|SeeAlso|
---------
- :doc:`begin`
- :doc:`step`
