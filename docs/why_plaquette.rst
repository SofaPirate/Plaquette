Why Plaquette?
==============

Rationale
---------

Media creators such as artists, musicians, and designers work with real-time
sensory signals all the time. However, few of them (especially beginners) really
"know their signals" and how to extract high-level informations from them
(such as debouncing, smoothing, normalizing, detecting peaks and regularities).

Consider the following case of learning how to work with a simple photoresistor
sensor plugged into an Arduino board on analog pin 0. The code reads as follow:

.. code-block:: c++

   int value = analogRead(A0);

The value thus read is the raw 10-bits value returned by the Arduino board's
Analog to Digital Converter (ADC) and thus reads as an integer between 0 and
1023. But how is that really useful for an artist who wants to use this value
creatively.

For example, what if one wants to react to a flash of light? Well, one solution
is to look at the kind of values we get and set a threshold.

.. code-block:: c++

  if (value > 716)
    // do something

Nice. But there are two problems with this. First, while it might work under
certain light conditions, it will likely stop working if these conditions change,
forcing us to make adjustments by hand to the threshold value.

Second, and perhaps more importantly, this piece of code does not really
*expresses* what we are after. As creative practitioners, we don't care whether
the light signal is above 716 or 456 or whatnot: what we really want to know
is whether it is *significantly high compared to ambient light*.

What this example shows is that the way we are teaching and learning about sensor
data is ineffective for creative applications. In other words: **raw digital data
lacks expressiveness**.

Continuing with our example, consider how one would take the input value and
directly reroute it to an analog (PWM) output on pin ``9``:

.. code-block:: c++

   analogWrite(9, value / 4);

Why do we need to perform that division by 4? That's because while the ADC gives
us 10-bit values (1024 possibilities), the PWM only supports 8 bits (256 possibilities)
forcing us to divide the incoming value by 4 (2 bits). But again, why is this
important to know for an artist, designer, or musician? And what exactly does it
have to do with our (expressive) intention?

A new standard
--------------

As a way to address these issues, we propose to create a general-purpose standard
interface for simple, real-time signal processing for media artists.
The objectives are as follow:

 #. **Allow creators to concentrate on the creative dimensions of their work** rather
    than on irrelevant numerical questions, hence also facilitating their learning.
 #. **Provide creators practitioners with accessible tools** that grasp high-level
    concepts such as "normalizing" and "detecting peaks" (rather than specific,
    arcane techniques on "how" to extract these informations such as "FFT", "zero-crossing" or "Chebyshev filtering").
 #. **Facilitate team work and interoperability** between applications by favouring
    an easily understandable, cross-platform way of thinking about real-time signals
    (for example by keeping all signals "in check" between 0 and 1).

Plaquette responds to these challenges by adopting the following characteristics:

 - **Easy to learn** by provide carefully-chosen functionalities that respond to
   common problems faced by creators ie. limited to only a few core functionalities
   that will solve 95% of your problems.
 - **Real-time** by allowing responsive interaction without interruptions.
 - **Focused on signals** rather than on numerical values such as 255, 1024, 716, etc.)
 - **Robust** by tolerating changes in the sensory context without breaking
   down, because interactive works are often presented in environments that are difficult
   to fully control.
 - **Interoperable and extensible** by adopting an object-oriented architecture
   fully compatible with Arduino.
