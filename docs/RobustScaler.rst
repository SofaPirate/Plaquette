.. include:: defs.hrst

RobustScaler
============

This filtering unit regularizes incoming signals by focusing on what the signal does
*most of the time* instead of reacting to rare extreme values. It does this by keeping
track of two boundaries:

- a **lower boundary** (low quantile), below which the signal almost never goes, and
- an **upper boundary** (high quantile), above which the signal almost never goes.

You can think of these as "typical low" and "typical high" values. Once these
boundaries are known, the signal is rescaled so that the lower boundary becomes 0
and the upper boundary becomes 1.

Because it pays attention to the typical behaviour of the signal, while ignoring rare
spikes or sudden jumps, the ``RobustScaler`` is a good choice for noisy sensors,
biological signals, or any data that behaves unpredictably.

Adjusting Robustness
--------------------

The **span** controls how much of your data is considered "typical" and placed
between the two boundaries.

- A span of 0.8 (80%) means that the scaler tries to place about 80% of the most
  common values between its lower and upper boundaries.
- A span of 0.95 (95%) means it tries to place about 95% of values there, and so on.

You can set it in code like this:

.. code-block:: c++

   RobustScaler scaler;
   scaler.span(0.8);  // Keep about 80% of values between 0 and 1.

Intuition
---------

A good way to understand the span is to imagine a **corridor** in which your
signal is allowed to move. The span controls how wide that corridor is.

- **Higher span (e.g. 0.9 or 0.95)**
  This corresponds to a **wide corridor**: most of the signal’s behaviour fits
  comfortably inside it. The scaler becomes more resistant to spikes or sudden
  jumps, but extreme values will simply hit the corridor walls and stay clamped
  near 0 or 1.

- **Lower span (e.g. 0.6 or 0.7)**
  This corresponds to a **narrow corridor**: the scaler pays attention to a
  smaller core of typical values. It reacts more strongly to changes, but even
  moderate deviations may be treated as if they were already “far out.”

Choosing a span is therefore a balance between *robustness* (wide corridor,
large span) and *sensitivity* (narrow corridor, small span).

.. image:: images/Plaquette-RobustScaler.png

|Example|
---------

Filters input values from a noisy sensor. The scaler adapts to the usual behaviour
of the signal but ignores occasional spikes. An LED turns on when the signal goes
beyond the upper "typical" range.

.. code-block:: c++

  #include <Plaquette.h>

  // Example: noisy analog sensor.
  AnalogIn sensor(A0);

  // Creates a robust scaler.
  RobustScaler scaler;

  // Simple square wave.
  Wave wave;

  // LED output.
  DigitalOut led(13);

  void begin() {
    // Sets span to 90%.
    scaler.span(0.9);
  }

  void step() {
    // Rescale value based on typical low/high ranges.
    sensor >> scaler;

    // Adjust period based on rescaled value.
    wave.period(scaler.mapTo(0.5, 2));

    // Blink LED.
    wave >> led;
  }

.. note::

   For readers familiar with statistics: the ``RobustScaler`` functions by
   estimating two quantiles of the incoming signal. If the span is ``s``,
   the scaler computes:

   - a lower quantile at ``(1 - s) / 2``, and
   - an upper quantile at ``(1 + s) / 2``.

   For example:

   - ``span(0.8)`` → lower = 10th percentile, upper = 90th percentile
   - ``span(0.9)`` → lower = 5th percentile, upper = 95th percentile

   The incoming value is then linearly mapped between these two quantiles.

|Reference|
-----------

.. doxygenclass:: RobustScaler
   :project: Plaquette
   :members:

|SeeAlso|
---------
- :doc:`MinMaxScaler`
- :doc:`Normalizer`
- :doc:`Smoother`
