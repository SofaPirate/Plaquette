.. include:: defs.hrst

MinMaxScaler
============

This filtering unit regularizes incoming signals by remapping them into a new interval of [0, 1].
It does so by keeping track of the minimum and the maximum values ever taken by the signal and
rescales it such that the minimum value of the signal is mapped to 0 and the maximum value
is mapped to 1.

.. image:: images/Plaquette-MinMaxScaler.png

In order to accommodate signals that might be changing through time, the user can specify a
"decay time window" to control the rate of decay of the minimum and maximum boundaries. The
principle is similar to the how the :doc:`Smoother` and the :doc:`Normalizer` make use of
`exponential moving average <https://www.investopedia.com/terms/e/ema.asp>`_.

.. caution::

   This filtering unit works well as long as there are no "outliers" in the signal (ie. extreme values)
   that appear in rare conditions. Such values will replace the minimum or maximum value and greatly
   restrict the spread of the filtered values.

   There are three ways to prevent this:

   #. Specifying a decay window using the ``time(decayTime)`` function.
   #. Smoothing incoming values using the ``smooth()`` method or a :doc:`Smoother` unit before sending to the ``MinMaxScaler``.
   #. Using a regularization unit that is less prone to outliers such as the :doc:`Normalizer`.


|Example|
---------

Reacts to high input values by activating an output LED. Scaler is used to
automatically adapt to incoming sensor values.

.. code-block:: c++

   #include <Plaquette.h>

   // Analog sensor (eg. photocell or microphone).
   AnalogIn sensor(A0);

   // Min-max scaler.
   MinMaxScaler scaler;

   // Output indicator LED.
   DigitalOut led(13);

   void begin() {}

   void step() {
     // Rescale value.
     sensor >> scaler;

     // Light led on threshold of 80%.
     (scaler > 0.8) >> led;
   }

|Reference|
-----------

.. doxygenclass:: MinMaxScaler
   :project: Plaquette
   :members:

|SeeAlso|
---------
- :doc:`Normalizer`
- :doc:`RobustScaler`
- :doc:`Smoother`
