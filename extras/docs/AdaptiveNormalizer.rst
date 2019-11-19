.. include:: defs.hrst

AdaptiveNormalizer
==================

This filtering unit regularizes incoming signals by normalizing them around a target mean and
standard deviation. It works by computing the normal distribution of the incoming data (mean and
standard variation) and uses this information to re-normalize the data according to a different
normal distribution (target mean and variance).

This particular normalizer computes the mean and variance using an `exponential moving average <https://www.investopedia.com/terms/e/ema.asp>`_.
Therefore, it adapts over time by giving more importance to the most recently seen data points.

|Example|
---------

Uses an adaptive normalizer to analyze input sensor values and detect extreme values.

.. code-block:: c++

   #include <Plaquette.h>
   
   AnalogIn sensor(A0);

   AdaptiveNormalizer normalizer(0, 1);

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

.. doxygenclass:: AdaptiveNormalizer
   :project: Plaquette
   :members: AdaptiveNormalizer, get, put, targetMean, targetStdDev, reset, cutoff, time

|SeeAlso|
---------
- :doc:`MinMaxScaler`
- :doc:`Normalizer`
- :doc:`Smoother`
