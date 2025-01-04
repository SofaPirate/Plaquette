.. include:: defs.hrst

Ramp
====

A source unit that generates a smooth transition between two values. The unit can be
triggered to start transitioning to a target value for a certain duration.

There are two ways to start the ramp.

By calling ``go(from, to, duration)`` the ramp will transition from value ``from`` to
value ``to`` in ``duration`` seconds.

Alternatively, calling ``go(to, duration)`` will start a transition from the ramp's
current value to ``to`` in ``duration`` seconds.

The following diagram shows what happens to the ramp signal if ``go(5.0, 1.0, 2.0)`` is
called, followed later by ``go(3.0, 1.0)``:

.. image:: images/Plaquette-Ramp.png

.. note::
  Ramps also support the use of `easing functions <http://easings.net>`_ in order to
  create different kinds of expressive effects with signals. An easing function can
  optionally be specified at the end of a ``go()`` command or by calling the
  ``easing()`` function.

  Please refer to :doc:`this page <easings>` for a full list of available easing functions.

|Example|
---------

Sequentially ramps through different values.

.. code-block:: c++

   #include <Plaquette.h>

   Ramp myRamp(0.0); // the ramp is initialized at zero (0)

   StreamOut serialOut(Serial);

   void begin() {
     // Apply an easing function (optional).
     myRamp.easing(easeOutSine);
   }

   void step() {
     if (myRamp.isFinished())
     {
       // Restarts the ramp going from current value to a random value in [-10, +10] in 2 seconds
       myRamp.go(randomFloat(-10, 10), 2.0);
     }

     myRamp >> serialOut;
   }


|Reference|
-----------

.. doxygenclass:: Ramp
   :project: Plaquette
   :members: Ramp, get, start, stop, resume, elapsed, progress, isRunning, isFinished, easing, noEasing, to, fromTo

|SeeAlso|
---------
- :doc:`Alarm`
- :doc:`easings`
- :doc:`Metronome`
- :doc:`Timer`
- :doc:`TriangleWave`
