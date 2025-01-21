Why Plaquette?
==============

Plaquette is a groundbreaking creative coding framework designed to empower creative practitioners 
by simplifying the way they work with real-time signals in tangible computing applications. By 
bridging the gap between low-level electronics and high-level creative expression, Plaquette enables 
creators to focus on what matters most: bringing their visions to life.

.. note::
  Plaquette is a French noun pronounced `pla-kett <https://www.youtube.com/watch?v=wG5xK352dfE>`__
  which refers to prototyping plates or boards ("plaquette de prototypage") commonly used in designing electronic projects.

  .. list-table:: 

    * - .. image:: images/Prototyping-Board.png
      - .. image:: images/Breadboard.png

The Need for a New Standard
---------------------------

Media artists, interactive designers, digital luthiers, and electronic musicians constantly 
engage with real-time signals. However, when working with tangible computing systems such 
as embedded sensors, robotics, connected objects, and electronic music instruments, available 
tools such as `Arduino <https://www.arduino.cc/>`_ are often very low-level and lack expressivity. 
Creative practitioners thus struggle to implement their vision directly using such platforms.

Consider the following case of learning how to work with a simple lignt sensor 
(eg. photoresistor) connected to an Arduino board on analog pin 0. The code reads 
as follows:

.. code-block:: c++

   int value = analogRead(A0);

The value that is read is a raw 10-bit value returned by the Arduino board's
Analog to Digital Converter (ADC), an integer between 0 and 1023. But how is this value 
intuitively useful for an artist who wants to use this value creatively?

For example, what if one wants to react to a flash of light? Well, one solution
would be to look at the value and compare it to a threshold:

.. code-block:: c++

  if (value > 716) {
    ...
  }

There are two problems with this approach.

Firstly, while it might work under certain lighting conditions, it will likely stop 
working if these conditions change, forcing us to make adjustments to the threshold 
value by hand.

Secondly, and perhaps more importantly, this piece of code does not really
*express* what we are after. As creative practitioners, we don't care whether
the light signal is above 716 or 456 or whatnot: what we really want to know
to detect a flash of light is whether the light signal is *significantly high compared 
to ambient light*.

What this example shows is that the way we are teaching and learning about sensor
data is inefficient for creative applications. In other words: **raw digital data
lacks expressiveness**.

Continuing with our example, consider how one would take the input value and
directly reroute it to an analog (PWM) output on pin 9:

.. code-block:: c++

   analogWrite(9, value / 4);

Why do we need to perform that division by 4? That's because while the ADC gives
us 10-bit values (1024 possibilities), the PWM only supports 8 bits (256 possibilities)
forcing us to divide the incoming value by 4 (2 bits). But again, why is this detail
important to know for an artist, designer, or musician? And what exactly does it
have to do with our expressive intention?

Meet Plaquette
--------------

As a way to address these issues, Plaquette offers a general-purpose, standard interface for simple, 
real-time signal processing tailored for media artists.

Plaquette's key objectives are:

1. **Empowering creators to focus on the creative aspects of their work**, rather than getting lost 
   in irrelevant numerical details, which supports a smoother learning process.
2. **Providing accessible tools for creative practitioners** that capture high-level concepts such as 
   *"normalizing"* and *"detecting peaks"*, without requiring deep technical knowledge of complex 
   techniques like Fast Fourier Transforms or Chebyshev filtering.
3. **Facilitating teamwork and interoperability** by promoting an intuitive, cross-platform approach 
   to real-time signals, such as by keeping all signals consistently scaled between ``0`` and ``1`` for 
   easier integration across different applications.

Plaquette achieves these goals by embracing the following core principles:

- **Ease of use:** Offering a carefully selected set of functionalities that address the most common 
  challenges faced by creators—keeping things simple while solving 95% of typical use cases.
- **Real-time performance:** Enabling smooth, uninterrupted interactions to ensure responsiveness in 
  dynamic environments.
- **Signal-oriented approach:** Focusing on meaningful signal manipulation rather than dealing with 
  arbitrary numerical values such as ``255``, ``1024``, or ``716``.
- **Robustness:** Adapting to changes in the sensory context without breaking down, ensuring reliability 
  in unpredictable and evolving environments such as art installations, live performances, and public art.
- **Interoperability and extensibility:** Leveraging an object-oriented architecture that seamlessly 
  integrates with the Arduino ecosystem, ensuring compatibility and future scalability.