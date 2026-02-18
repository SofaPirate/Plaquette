.. include:: defs.hrst

Ramp
====

A source unit that generates a smooth transition between two values. The unit can be
triggered to start transitioning to a target value for a certain duration.

Parameters
----------

.. list-table::
   :header-rows: 1
   :widths: 12 33 9 16 15 15

   * - Name
     - Description
     - Range
     - Setter
     - Getter
     - Flow
   * - duration
     - Duration of the ramp transition (in seconds).
     - > 0
     - ``duration(v)``
     - ``duration()``
     - ``Duration()``
   * - speed
     - Rate of change (change-per-second).
     - > 0
     - ``speed(v)``
     - ``speed()``
     - ``Speed()``

Events
------

.. list-table::
   :header-rows: 1
   :widths: 50 25 25

   * - When
     - Trigger
     - Callback
   * - Timed process completes
     - ``finished()``
     - ``onFinish(c)``

Usage
-----

There are two ways to start the ramp:

 * ``go(to)``: starts a transition from the ramp's current value to ``to`` using latest ramp duration
 * ``go(to, duration)``: starts a transition from the ramp's current value to ``to`` in ``duration`` seconds.
 * ``go(from, to, duration)``: ramp will transition from value ``from`` to value ``to`` in ``duration`` seconds.

The following diagram shows what happens to the ramp signal if ``go(5.0, 1.0, 2.0)`` is called, followed later by ``go(3.0, 1.0)``:

.. image:: images/Plaquette-Ramp.png

.. important::
  Ramps also support the use of `easing functions <http://easings.net>`_ in order to
  create different kinds of expressive effects with signals. An easing function can
  optionally be specified at the end of a ``go()`` command or by calling the
  ``easing()`` function.

  Please refer to :doc:`this page <easings>` for a full list of available easing functions.

|Example|
---------

Sequentially ramps through random values, creating a random walk (aka "drunk") movement.

.. code-block:: c++

   #include <Plaquette.h>

   Ramp zigZagRamp(2.0); // Default duration: 2 seconds.

   Plotter plotter(115200);

   void begin() {
     // Apply an easing function (optional).
     zigZagRamp.easing(easeOutSine);
     // Go from zero (initial value) to random value.
     zigZagRamp.go( randomFloat(-10.0, 10.0) );
   }

   void step() {
     if (zigZagRamp.finished()) // event: ramp finished
     {
       // Ramp from current value to new random value, increasing duration by 1 second each time.
       zigZagRamp.go(zigZagRamp + randomFloat(-10.0, 10.0), zigZagRamp.duration() + 1);
     }

     // Send ramp value to plotter for visualization.
     zigZagRamp >> plotter;
   }


|Example|
---------

A ramp can be used as a timer. By default, ramp goes from 0 to 1 (0% to 100%) over a given duration.
The ramp can thus be used as a measure of progress over time. The example below uses the ramp value to
cycle through four phases, changing the frequency of a LED pulse at each transition:

.. code-block:: c++

   #include <Plaquette.h>

   // Ramp over 5 seconds. Default ramp always runs from 0 to 1.
   Ramp timer(5.0);

   // Sine wave oscillator driving the LED.
   Wave pulse(SQUARE);

   // Build-in LED.
   DigitalOut led(LED_BUILTIN);

   void begin() {
     timer.start(); // start the timer
   }

   void step() {
     // Change wave frequency based on progress thresholds.
     if (timer < 0.25) {
       pulse.frequency(0.5);  // slow pulse: warming up
     }
     else if (timer < 0.5) {
       pulse.frequency(3.0);  // fast pulse: running
     }
     else if (timer < 0.75) {
       pulse.frequency(1.0);  // medium pulse: cooling down
     }
     else {
       pulse.frequency(0.25); // very slow pulse: standby
     }

     // Flow wave to LED.
     pulse >> led;

     // Restart the timer when it completes, increasing duration each time.
     if (timer.finished()) {
       timer.duration( timer.duration() + 5 );
       timer.start();
     }
   }


|Reference|
-----------

.. doxygenclass:: Ramp
   :project: Plaquette
   :members:

|SeeAlso|
---------
- :doc:`Alarm`
- :doc:`Chronometer`
- :doc:`easings`
- :doc:`Metronome`
- :doc:`Wave`
