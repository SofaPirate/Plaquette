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

   Ramp myRamp(3.0); // initial duration: 3 seconds

   StreamOut serialOut(Serial);

   void begin() {
     // Apply an easing function (optional).
     myRamp.easing(easeOutSine);
     // Launch ramp: ramp from -10 to 10.
     myRamp.go(-10, 10);
   }

   void step() {
     if (myRamp.isFinished())
     {
       // Launch ramp from current value to half, increasing duration by one second.
       myRamp.go(myRamp / 2, myRamp.duration() + 1);
     }

     myRamp >> serialOut;
   }


|Reference|
-----------

.. doxygenclass:: Ramp
   :project: Plaquette
   :members: Ramp, get, start, stop, pause, resume, elapsed, progress, isRunning, add, set, isFinished, easing, noEasing, to, fromTo

|SeeAlso|
---------
- :doc:`Alarm`
- :doc:`Chronometer`
- :doc:`easings`
- :doc:`Metronome`
- :doc:`TriangleWave`
