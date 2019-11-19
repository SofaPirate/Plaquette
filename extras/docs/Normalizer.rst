.. include:: defs.hrst

Normalizer
==========

This filtering unit regularizes incoming signals by normalizing them around a target mean and
standard deviation. It works by computing the normal distribution of the incoming data (mean and
standard variation) and uses this information to re-normalize the data according to a different
normal distribution (target mean and variance).

This normalizer computes the mean and variance over all the data it receives.

|Example|
---------

Uses a normalizer to analyze input sensor values and detect extreme values.

.. code-block:: c++

   #include <Plaquette.h>

   AnalogIn sensor(A0);

   Normalizer normalizer(0, 1);

   DigitalOut led(13);

   void begin() {}

   void step() {
     // Normalize value.
     sensor >> normalizer;

     // Light led if value differs from mean by more
     // than twice the standard deviation.
     (abs(normalizer) > 2.0) >> led;
   }

|Reference|
-----------

.. doxygenclass:: Normalizer
   :project: Plaquette
   :members: Normalizer, get, put, targetMean, targetStdDev, reset

|SeeAlso|
---------
- :doc:`AdaptiveNormalizer`
- :doc:`MinMaxScaler`
- :doc:`Smoother`
