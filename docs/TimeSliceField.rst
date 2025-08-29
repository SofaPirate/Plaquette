.. include:: defs.hrst

TimeSliceField
==============

This field unit stores a **series of values collected over time**, which can then be sampled spatially like an array
across the normalized range [0, 1]. It is useful for plotting time-varying signals, such as mapping audio or sensor
input onto an LED strip or a motor array.

The unit gathers samples over a defined time period, storing them in its internal memory. When it is ready to be
read, it fires an ``updated`` event which can be used to update outputs only when necessary.

The size ``SIZE`` of the internal memory (ie., the number of values that are stored internally) and the overall time
``period`` (in seconds) over which the field applies is specified at creation time:

.. code-block:: cpp

  TimeSliceField<SIZE> field(period);

The unit supports two modes:
 #. In the ``block`` mode (default) the buffer fills up and fires the ``updated`` event when it is full; then it is
    emptied and starts filling up again.
 #. In the ``rolling`` mode the buffer fills up, then...

   - ... when it is full it fires ``updated``;
   - ... when enough time has passed that a new value needs to be replaced, the older value is removed and the buffer
     is shifted by one value and it fires ``updated`` to signal a change.

|Example|
---------

Here is a simple example that uses a `TimeSliceField` to collect data from a sine wave and render it to an LED strip.

.. code-block:: cpp

  #include <Plaquette.h>

  // The number of LEDs.
  const int N_LEDS = 8;

  // An array of LEDs.
  DigitalOut leds[] = { 2, 3, 4, 5, 6, 7, 8, 9 }; // shorthand for DigitalOut leds[] = { DigitalOut(2), DigitalOut(3), DigitalOut(4), ... };

  // The sine wave with a period of one second.
  Wave wave(SINE, 1.0f);

  // The time slice field (over 2 seconds period).
  TimeSliceField<N_LEDS> timeSlice(2.0f);

  void begin() {
    // Set field to rolling mode.
    timeSlice.rolling();
  }

  void step() {
    // Update the field.
    wave >> timeSlice;

    // Update the LEDs.
    if (timeSlice.updated()) {
      for (int i=0; i<N_LEDS; i++) {
        float proportion = mapTo01(i, 0, N_LEDS-1); // maps i to [0, 1]
        timeSlice.at(proportion) >> leds[i]; // send to LED
      }
    }
  }

|Reference|
-----------

.. doxygenclass:: TimeSliceField
   :members:
   :undoc-members:

|SeeAlso|
---------

- :doc:`array`
- :doc:`PivotField`
