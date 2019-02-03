.. include:: defs.hrst

Smoother
========

Smooths the incoming signal by removing fast variations and noise (high frequencies).

.. image:: images/Plaquette-Smooth.png

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

.. note::
   The filter uses an `exponential moving average <https://en.wikipedia.org/wiki/Exponential_smoothing>`_
   which corresponds to a form of `low-pass filter <https://en.wikipedia.org/wiki/Low-pass_filter>`_.

|Reference|
-----------

.. doxygenclass:: Smoother
   :project: Plaquette
   :members: Smoother, get, put, cutoff, time

|SeeAlso|
---------
- :doc:`AnalogIn`
- :doc:`DigitalIn`
