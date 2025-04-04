.. include:: defs.hrst

Engine
======

A control structure that acts like the **conductor of an orchestra**, managing an ensemble of **units**. 
It handles their initialization, updates, and timing, ensuring that all components remain synchronized.

By default, all units are automatically added to the **primary engine** which can be accessed using the
global object ``Plaquette``. By using **secondary engines** you can organize and optimize your code, 
allowing for multi-tasking, grouping units, switching between ensembles of units, and save power by
running engines at lower frequency.

Usage
-----

To create and use an engine, simply declare it:

.. code-block:: c++

   Engine myEngine;

You will need to call the engine's ``begin()`` function at initialization, and then its ``step()`` function
at a regular pace.

To assign units to a specific engine, add it as the last parameter at construction:

.. code-block:: c++

   SquareWave wave(1.0, 0.2, myEngine); // Use myEngine instead of the default

Each engine provides its own time measurements:

.. code-block:: c++

   float sec = myEngine.seconds();        // Time since engine started, in seconds
   uint32_t ms = myEngine.milliSeconds(); // Time in milliseconds
   uint64_t us = myEngine.microSeconds(); // Time in microseconds

For more in-depth explanations and examples please read :ref:`secondary-engines`.

|Reference|
----------

.. doxygenclass:: Engine
   :project: Plaquette
   :members: Engine, begin, step, nUnits, seconds, milliSeconds, microSeconds, nSteps, sampleRate, samplePeriod, primary, isPrimary

|SeeAlso|
---------

- :doc:`begin`
- :doc:`step`
- :doc:`seconds`
- :ref:`secondary-engines`
