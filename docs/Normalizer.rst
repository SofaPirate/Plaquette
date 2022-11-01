.. include:: defs.hrst

Normalizer
==========

This filtering unit regularizes incoming signals by normalizing them around a target mean and
standard deviation. It works by computing the normal distribution of the incoming data (mean and
standard variation) and uses this information to re-normalize the data according to a different
normal distribution (target mean and variance).

By default, the unit computes the mean and variance over all the data ever received. However,
it can instead compute over a time window using an `exponential moving average <https://www.investopedia.com/terms/e/ema.asp>`_.

|Example|
---------

Uses a normalizer to analyze input sensor values and detect extreme values.

.. code-block:: c++

   #include <Plaquette.h>

   // Analog sensor (eg. photocell or microphone).
   AnalogIn sensor(A0);

   // Creates a normalizer with mean 0 and standard deviation 1.
   Normalizer normalizer(0, 1);

   // Output indicator LED.
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
   :members: Normalizer, get, put, timeWindow, infiniteTimeWindow, timeWindowIsInfinite, reset, start, stop, isStarted, targetMean, targetStdDev, isHighOutlier, isLowOutlier, isOutlier, highOutlierThreshold, lowOutlierThreshold

|SeeAlso|
---------
- :doc:`MinMaxScaler`
- :doc:`Smoother`
