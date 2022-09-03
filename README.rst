.. title: Plaquette documentation

Plaquette
=========

Plaquette is an object-oriented, user-friendly, signal-centric programming
framework for **creative physical computing** compatible with Arduino. Plaquette
provides a simple yet powerful interface, allowing both beginner and advanced
creative practitioners to design expressive physical computing systems in an intuitive way.

Plaquette allows you to:
 * React to multiple sensors and actuators in real-time without interruption.
 * Automatically calibrate sensors to generate stable interactions in changing environments.
 * Design complex interactive behaviors by seamlessly combining powerful effects such as ramps and oscillators.

 .. image:: images/Plaquette-LogoTextAnim.gif

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
  Plaquette forbids the use of blocking functions such as Arduino's `delay() <https://www.arduino.cc/reference/en/language/functions/time/delay/>`_
  and `delayMicroseconds() <https://www.arduino.cc/reference/en/language/functions/time/delaymicroseconds/>`_.
  Rather, it invites programmers to adopt a frame-by-frame approach to coding
  similar to `Processing <https://processing.org/>`_.

Arduino compatible
------------------

Plaquette is installed as an Arduino library and provides a replacement for the
core Arduino functionalities while remaining fully compatible with Arduino code.

.. warning::
   Plaquette is still at an experimental stage of development. If you
   have any issues or questions, please contact the developers or file
   a bug in our `issue tracker <https://github.com/SofaPirate/Plaquette/issues>`_.
