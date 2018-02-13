.. include:: defs.hrst

Ramp
====

A source unit that generates a smooth transition between two values. The unit can be
triggered to start transitioning to a target value for a certain duration.

There are two ways to start the ramp.

By calling ``start(from, to, duration)`` the ramp will transition from value ``from`` to
value ``to`` in ``duration`` seconds.

Alternatively, calling ``start(to, duration)`` will start a transition from the ramp's
current value to ``to`` in ``duration`` seconds.

|Example|
---------

.. code-block:: c++

   Ramp myRamp(0.5); // the ramp is initalized at zero (0)

   StreamOut serialOut(Serial);

   void begin() {
   }

   void step() {
     if (myRamp.isComplete())
     {
       // Restarts the ramp going from current value to a random value in [-10, +10] in 2 seconds
       myRamp.start(randomFloat(-10, 10), 2.0);
     }

     myRamp >> serialOut;
   }


|Reference|
-----------

.. doxygenclass:: Ramp
   :project: Plaquette
   :members: get, start, stop, resume, elapsed, progress, isStarted, isComplete

|SeeAlso|
---------
- :doc:`TriOsc`