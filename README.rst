.. title: Plaquette documentation

Plaquette
=========

Plaquette is an object-oriented, user-friendly, signal-centric programming
framework for creative physical computing compatible with Arduino. Plaquette
allows creative coders to focus on designing expressive physical computing
systems.

Object-oriented
---------------

Plaquette is designed using input, output, and filtering units that can be
easily interchanged in a plug-and-play fashion. Units are created using expressive
code. For example, the code ``DigitalOut led`` creates a new digital output object
that can be used to control and LED.

User-friendly
-------------

Plaquette allows users to quickly design interactive systems using an expressive
language that abstracts away low-level functions. This allows both beginners and
experts to create truly expressive code. For example, switching our LED
object can be achieved by calling: ``led.on()``. Find out more about Plaquette's
base units by following :doc:`this link <base_units>`.

Signal-centric
--------------

Plaquette helps designers manipulate real-time signals from inputs to outputs.
In Plaquette, signals are represented as either ``true/false`` conditions in the
case of digital binary signals such as those coming from a button or switch, or as
floating-point numbers typically in the [0..1] range (ie. 0% to 100%) in the case
of analog signals such as those emitted by a light sensor, microphone, or
potentiometer. No more need to perform counter-intuitive conversions on integer
values.

Filtering
---------

Plaquette provides simple yet powerful data filtering tools for debouncing,
smoothing, and normalizing data. Removing noise in input signals can be as simple as
calling a function such as ``debounce()`` or ``smooth()``. Rather than guessing by
trial-and-error the right threshold to trigger an event based on an input sensor,
one can use auto-normalizing :doc:`filters <filters>` such as :doc:`MinMaxScaler` and :doc:`Normalizer`.

Data-flow
---------

Signals in Plaquette can easily flow between units, in a similar fashion as modern
data-flow softwares such as `Max <https://cycling74.com/products/max>`_,
`Pure Data <https://puredata.info>`_, and `TouchDesigner <https://derivative.ca>`_.
While this can be achieved using function calls, Plaquette further provides a
special **piping operator** (``>>``) which allows the sending of data from one unit
to another. Example: ``sensor >> filter >> output``.

Real-time
---------

Plaquette avoids blocking processes such as Arduino's (in)famous ``delay()`` by
providing a set of :doc:`timing units <timing>` as well as time-based
:doc:`signal generators <generators>`. The processing loop is thus never interrupted,
allowing interactive and generative processes to flow smoothly.

.. warning::
  Plaquette forbids the use blocking functions such as Arduino's `delay() <https://www.arduino.cc/reference/en/language/functions/time/delay/>`_
  and `delayMicroseconds() <https://www.arduino.cc/reference/en/language/functions/time/delaymicroseconds/>`_.
  Rather, it invites programmers to adopt a frame-by-frame approach to coding
  similar to `Processing <https://processing.org/>`_.

Arduino compatible
------------------

Plaquette is installed as an Arduino library and provides a replacement for the
core Arduino functionalities while remaining fully compatible with Arduino code.

.. note::
  Plaquette is still at an experimental stage of development. If you
  have any issues or questions, please contact the developers or file
  a bug in our `issue tracker`_.
.. _issue tracker: https://github.com/SofaPirate/Plaquette/issues

.. note::
  The Plaquette reference text is licensed under a `Creative Commons Attribution-Share
  Alike 3.0 License <https://creativecommons.org/licenses/by-sa/3.0/>`_. Parts of
  the documentation has been borrowed and/or adapted from the `Arduino Reference
  <https://www.arduino.cc/reference/en/>`_ and from the `Processing Reference
  <https://processing.org/reference/>`_ texts.

Getting started
---------------

Step 1: Install Plaquette
~~~~~~~~~~~~~~~~~~~~~~~~~

If you do not have Arduino installed on your machine you need to
`download and install the Arduino
IDE <https://www.arduino.cc/en/Main/Software>`__ for your platform.

Once Arduino is installed, please install Plaquette as an Arduino
library following `these
instructions <https://www.arduino.cc/en/Guide/Libraries>`__.

Step 2: Your first Plaquette program
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

We will begin by creating a simple program that will make the built-in
LED blink.

Create a new sketch
^^^^^^^^^^^^^^^^^^^

Create a new empty sketch by selecting **File > New**.

**IMPORTANT:** New Arduino sketches are initialized with some "slug"
starting code. Make sure to erase the content of the sketch before
beginning. You can use **Edit > Select All** and then click **Del** or
**Backspace**.

Include library
^^^^^^^^^^^^^^^

Include the Plaquette library by typing:

.. code:: cpp

    #include <Plaquette.h>

Create an output unit
^^^^^^^^^^^^^^^^^^^^^

Now, we will create a new unit that will allow us to control the
built-in LED:

.. code:: cpp

    DigitalOut myLed(13);

In this statement, ``DigitalOut`` is the **type** of unit that we are
creating. There exist other types of units as we will soon see.
``DigitalOut`` is a type of unit that is attached to one of the many
digital outputs on the Arduino board which can be set to one of two
states ("on/off", "high/low", "1/0").

The word ``myLed`` is a **name** for the object we are creating.

Finally, ``13`` is a **parameter** of the object ``myLed`` that
specifies the *pin* it corresponds to on the board.

In English, the statement would thus read as: "Create a unit named
``myLed`` of type ``DigitalOut`` on pin ``13``."

Create an input unit
^^^^^^^^^^^^^^^^^^^^

We will now create another unit that will generate a signal which will
be sent to the LED to make it blink. To this effect, we will use the
``SquareOsc`` unit type which generates a `square
wave <https://en.wikipedia.org/wiki/Square_wave>`__ oscillating between
"on/high/one" and "off/low/zero" at a regular period of 2.0 seconds and
a `duty-cycle <https://en.wikipedia.org/wiki/Duty_cycle>`__ of 50%:

.. code:: cpp

    SquareOsc myOsc(2.0, 0.5);

Create the begin() function
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Each Plaquette sketch necessitates the declaration of two functions:
``begin()`` and ``step()``.

Function ``begin()`` is called only once at the beginning of the sketch
(just like the
`setup() <https://www.arduino.cc/reference/en/language/structure/sketch/setup/>`__
function in Arduino). In our case, we do not to perform any special
configuration at startup so we will leave the ``begin()`` function
empty:

.. code:: cpp

    void begin() {}

Create the step() function
^^^^^^^^^^^^^^^^^^^^^^^^^^

The ``step()`` function is called repetitively and indefinitely during
the course of the program (like the
`loop() <https://www.arduino.cc/reference/en/language/structure/sketch/loop/>`__
function in Arduino).

Here, we need to send the signal generated by the ``myOsc`` input unit
to the ``myLed`` output unit. We will do this using Plaquette's special
``>>`` operator:

.. code:: cpp

    void step() {
      myOsc >> myLed;
    }

In plain English, the statement ``myOsc >> myLed`` reads as: "Take the
value generated by ``myOsc`` and put it in ``myLed``."

Upload sketch
^^^^^^^^^^^^^

Upload your sketch to the Arduino board. You should see the LED on the
board blinking once every two seconds at a regular pace.

Et voilà!

Full code
^^^^^^^^^

.. code:: cpp

    #include <Plaquette.h>

    DigitalOut myLed(13);

    SquareOsc myOsc(2.0, 0.5);

    void begin() {}

    void step() {
      myOsc >> myLed;
    }

Step 3 : Experiment!
~~~~~~~~~~~~~~~~~~~~

Period and duty
^^^^^^^^^^^^^^^

Try changing the *period* and/or *duty-cycle* parameters in the square
wave unit construction:

.. code:: cpp

    SquareOsc myOsc(<period>, <duty-cycle>);

-  ``<period>`` can be any positive number representing the period of
   oscillation (in seconds)
-  ``<duty-cycle>`` can be any number between 0.0 (0%) and 1.0 (100%)
   and represents the proportion of the period during which the signal
   is "high" (ie. "on duty")

What happens?

Adding and multiplying
^^^^^^^^^^^^^^^^^^^^^^

Add another oscillator with a different period and duty cycle: multiply
their values and send the result to the LED.

.. code:: cpp

    SquareOsc myOsc2(<period>, <duty-cycle>);
    // ...
    void step() {
      (myOsc * myOsc2) >> myLed;
    }

Try adding their values instead: what do you see?

Use a conditional
^^^^^^^^^^^^^^^^^

Add a third oscillator that will "switch" between the two oscillators
every 5 seconds using an
`if...else <https://www.arduino.cc/reference/en/language/structure/control-structure/if/>`__
statement.

.. code:: cpp

    // TIP: omitting the duty-cycle parameter results in default value (0.5)
    SquareOsc mySwitcher(5.0);
    // ...
    void step() {
      if (mySwitcher)
        myOsc >> myLed;
      else
        myOsc2 >> myLed;
    }

**ADVANCED**: You can rewrite this expression in a more compact way
using the `? : conditional
operator <https://www.tutorialspoint.com/arduino/arduino_conditional_operator.htm>`__:

.. code:: cpp

    void step() {
      (mySwitcher ? myOsc : myOsc2) >> myLed;
    }

More examples
^^^^^^^^^^^^^

You will find more examples in **File > Examples > Plaquette** including:

- Using a button
- Using an analog input such as a photocell or potentiometer
- Using an analog output
- Basic filtering (smoothing, re-scaling)
- Serial input and output

Credits
-------

Developers:

- Sofian Audry • `Website <http://sofianaudry.com>`__ • `GitHub <https://github.com/sofian>`__
- Thomas Ouellet Fredericks • `Website <http://www.t-o-f.info>`__ • `GitHub <https://github.com/thomasfredericks>`__

Plaquette's base source code was produced as part of a research project
at `labXmodal <http://xmodal.hexagram.ca>`__. A special thanks to `Chris
Salter <http://chrissalter.com>`__ for his support.

Plaquette borrows ideas from the `Arduino <https://arduino.cc>`__,
`ChucK <http://chuck.cs.princeton.edu/>`__,
`mbed <https://www.mbed.com/>`__,
`Processing <https://processing.org/>`__, and `Pure
Data <https://puredata.info/>`__.

License
-------

Plaquette is distributed under the `Gnu General Public License v
3.0 <https://www.gnu.org/licenses/gpl-3.0.en.html>`__.

The text of the Plaquette documentation is licensed under a `Creative Commons
Attribution-ShareAlike 3.0
License <https://creativecommons.org/licenses/by-sa/3.0/>`__. Parts of
the text was copied and/or adapted from the `Arduino documentation <https://www.arduino.cc/>`_. Code
samples in the guide are released into the public domain.
