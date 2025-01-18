.. include:: defs.hrst

PeakDetector
============

This unit detects peaks (minima or maxima) in an incoming signal. Peaks are detected
based on crossing a trigger threshold above (or below) which a peak is detected.

Two different ways are supported to do this:

 - In **crossing** modes (``PEAK_RISING`` and ``PEAK_FALLING``) the peak is detected *as soon as the signal crosses* the ``triggerThreshold``.
 - In **apex** modes (``PEAK_MAX`` and ``PEAK_MIN``) the peak is detected after the signal crosses the ``triggerThreshold``, reaches its apex, and then *falls back* by a certain proportion (%) between the threshold and the apex (controlled by the ``fallbackTolerance`` parameter).

In all cases, after a peak is detected, the detector will wait until the signal
crosses back the ``reloadThreshold`` (which can be adjusted to control detection
sensitivity) before it can be triggered again.

.. image:: images/Plaquette-PeakDetector.png

In summary, the four different modes available are:

 - ``PEAK_RISING``  : peak detected as soon as ``value >= triggerThreshold``, then wait until ``value < reloadThreshold``
 - ``PEAK_FALLING`` : peak detected as soon as ``value <= triggerThreshold``, then wait until ``value > reloadThreshold``
 - ``PEAK_MAX``     : peak detected after ``value >= triggerThreshold`` and then *falls back* after peaking; then waits until ``value < reloadThreshold``
 - ``PEAK_MIN``     : peak detected after ``value <= triggerThreshold`` and then *falls back* after peaking; then waits until ``value > reloadThreshold``

.. important::
   Before sending a signal to a PeakDetector unit, it is recommended to normalize
   signals, preferably using the :doc:`Normalizer` unit. Furthermore, to avoid a
   noisy signal to generate false peaks, it is recommended to smooth the signal
   by calling the source unit's ``smooth()`` method or by using a :doc:`Smoother`
   unit.

|Example|
---------

Uses a Normalizer and a PeakDetector to analyze input sensor values and detect
peaks. Toggle and LED each time a peak is detected.

.. code-block:: c++

    #include <Plaquette.h>

    // Analog sensor (eg. photocell or microphone).
    AnalogIn sensor(A0);

    // Normalization unit to normalize values.
    Normalizer normalizer;

    // Peak detector. Threshold is set at 1.5 standard deviations above normal.
    PeakDetector detector(normalizer.highOutlierThreshold(1.5)); // default mode = PEAK_MAX
    // NOTE: You can change mode using optional 2nd parameter, example:
    // PeakDetector detector(1.5, PEAK_FALLING));

    // Digital LED output.
    DigitalOut led;

    void begin() {
      // Adjust reload threshold to smaller value than reloadThreshold.
      detector.reloadThreshold(normalizer.highOutlierThreshold(1.0));

      // Adjust fallback tolerance as % between apex and trigger threshold.
      detector.fallbackTolerance(0.2); // 0.2 = 20% (default: 10%)

      // Smooth signal to avoid false peaks due to noise.
      sensor.smooth();

      // Set a time window of 1 minute (60 seconds) on normalizer.
      // This will allow the normalier to slowly readjust itself
      // if the lighting conditions change.
      normalizer.timeWindow(60.0f);
    };

    void step() {
      // Signal is normalized and sent to peak detector.
      sensor >> normalizer >> detector;

      // Toggle LED when peak detector triggers.
      if (detector)
        led.toggle();
    }


|Reference|
-----------

.. doxygenclass:: PeakDetector
   :project: Plaquette
   :members: PeakDetector, get, put, isOn, triggerThreshold, reloadThreshold, fallbackTolerance, modeInverted, modeCrossing, modePeak, mode, onBang, clearEvents

|SeeAlso|
---------
- :doc:`Normalizer`
- :doc:`MinMaxScaler`
- :doc:`Smoother`
