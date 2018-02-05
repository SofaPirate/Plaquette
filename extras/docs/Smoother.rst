.. include:: defs.hrst

Smoother
========

Smooths the incoming signal.

|Example|
---------

.. code-block:: c++

   AnalogIn sensor(A0);

   Smoother smoother(0.01);

   StreamOut serialOut(Serial);

   void begin() {}

   void step() {
     // Smooth value and send it to serial output.
     sensor >> smoother >> serialOut;
   }

|Reference|
-----------

.. doxygenclass:: Smoother
   :project: Plaquette
   :members: Smoother, get, put

|SeeAlso|
---------
- :doc:`AnalogIn`
- :doc:`DigitalIn`
