Features
========

Plaquette is an object-oriented_, user-friendly_, signal-centric_ framework that
facilitates `signal filtering`_ in real-time_. It is fully `compatible with Arduino`_.

.. _object-oriented:

Object-oriented
---------------

Plaquette is designed using input, output, and filtering units that are easily
interchangeable in a plug-and-play fashion. Units are created using expressive
code.

For example, the code ``DigitalOut led`` creates a new digital output object
that can be used to control an LED.

+------------------------------------------------+------------------------------------------------+
| Arduino                                        | Plaquette                                      |
+================================================+================================================+
| *Create digital output to control an LED:*                                                      |
+------------------------------------------------+------------------------------------------------+
| ``pinMode(12, OUTPUT);``                       | ``DigitalOut led(12);``                        |
+------------------------------------------------+------------------------------------------------+
| *Create digital input push-button:*                                                             |
+------------------------------------------------+------------------------------------------------+
| ``pinMode(2, INPUT);``                         | ``DigitalIn button(2);``                       |
+------------------------------------------------+------------------------------------------------+

.. _user-friendly:

User-friendly
-------------

Plaquette allows users to quickly design interactive systems using an expressive
language that abstracts low-level functions. This allows both beginners and
experts to create truly expressive code. For example, switching our LED
object on or off can be achieved by calling: ``led.on()``. Find out more about Plaquette's
base units by following :doc:`this link <base_units>`.

+------------------------------------------------+------------------------------------------------+
| Arduino                                        | Plaquette                                      |
+================================================+================================================+
| *Turn LED on:*                                                                                  |
+------------------------------------------------+------------------------------------------------+
| ``digitalWrite(12, HIGH);``                    | ``led.on();``                                  |
+------------------------------------------------+------------------------------------------------+
| *Check if button is pushed:*                                                                    |
+------------------------------------------------+------------------------------------------------+
| ``if (digitalRead(2) == HIGH)``                | ``if (button.isOn())``                         |
+------------------------------------------------+------------------------------------------------+

.. _signal-centric:

Signal-centric
--------------

Plaquette helps designers manipulate real-time signals from inputs to outputs.
In Plaquette, signals are represented either as ``true/false`` conditions (in the
case of digital binary signals such as those coming from a button or switch), or as
floating-point numbers in the [0, 1] range (ie. 0% to 100%) (in the case
of analog signals such as those emitted by a light sensor, microphone, or
potentiometer.) Because of this, there is no more need for users to perform counter-intuitive
conversions on integer values.

+------------------------------------------------+------------------------------------------------+
| Arduino                                        | Plaquette                                      |
+================================================+================================================+
| *Check if button is released:*                                                                  |
+------------------------------------------------+------------------------------------------------+
| ``if (digitalRead(2) != HIGH)``                | ``if (!button)``                               |
+------------------------------------------------+------------------------------------------------+
| *Check if sensor value is higher than 70%:*                                                     |
+------------------------------------------------+------------------------------------------------+
| ``if (analogRead(A0) >= 716)``                 | ``if (sensor >= 0.7)``                         |
+------------------------------------------------+------------------------------------------------+

.. _data flow:

Data-flow
---------

Plaquette provides simple yet powerful data filtering tools for debouncing,
smoothing, and normalizing data. Removing noise in input signals can be as simple as
calling a function such as ``debounce()`` or ``smooth()``. Rather than guessing the right threshold
for triggering an event based on input sensor input, one can use auto-normalizing
:doc:`filters <filters>` such as :doc:`MinMaxScaler` and :doc:`Normalizer`.

Signals in Plaquette can easily flow between units, in a similar fashion to modern
data-flow software such as `Max <https://cycling74.com/products/max>`_,
`Pure Data <https://puredata.info>`_, and `TouchDesigner <https://derivative.ca>`_.
While this can be achieved using function calls, Plaquette provides a
special **piping operator** (``>>``) which allows data to be sent from one unit
to another.

+------------------------------------------------+------------------------------------------------+
| Arduino                                        | Plaquette                                      |
+================================================+================================================+
| *Set LED to ON when button is pressed:*                                                         |
+------------------------------------------------+------------------------------------------------+
| ``digitalWrite(12, digitalRead(2)/4);``        | ``button >> led;``                             |
+------------------------------------------------+------------------------------------------------+
| *Set LED to ON when input sensor is high:*                                                      |
+------------------------------------------------+------------------------------------------------+
| ``digitalWrite(12, (analogRead(A0) >= 716 ?    | ``(sensor >= 0.7) >> led;``                    |
| HIGH : LOW));``                                |                                                |
+------------------------------------------------+------------------------------------------------+

Read :doc:`regularizing` to see how you can take full advantage of
Plaquette's signal filtering features.

.. _real-time:

Real-time
---------

Plaquette avoids blocking processes such as Arduino's (in)famous ``delay()`` by
providing a set of :doc:`timing units <timing>` as well as time-based
:doc:`signal generators <generators>`. As such, the processing loop is never interrupted,
allowing interactive and generative processes to flow smoothly.

Plaquette forbids the use of blocking functions such as Arduino's
`delay() <https://www.arduino.cc/reference/en/language/functions/time/delay/>`_ and
`delayMicroseconds() <https://www.arduino.cc/reference/en/language/functions/time/delaymicroseconds/>`_.
Rather, it invites programmers to adopt a frame-by-frame approach to coding similar
to `Processing <https://processing.org/>`_.

Compare the following attempt to make an `LED blink <https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink>`_
when pressing a button in Arduino, versus Plaquette's real-time approach:

+------------------------------------------------+------------------------------------------------+
| Arduino                                        | Plaquette                                      |
+================================================+================================================+
| .. code-block:: c++                            | .. code-block:: c++                            |
|                                                |                                                |
|   int buttonPin = 2;                           |     DigitalIn button(2);                       |
|   int ledPin = 12;                             |     DigitalOut led(12);                        |
|                                                |                                                |
|   void setup() {                               |     // Square wave 1 second period.            |
|     pinMode(buttonPin, OUTPUT);                |     Wave oscillator(1.0);                      |
|     pinMode(ledPin, OUTPUT);                   |                                                |
|   }                                            |     void begin() {}                            |
|                                                |                                                |
|   void loop() {                                |     void step() {                              |
|     // Button is checked once per second.      |       // Button is checked at all time.        |
|     if (digitalRead(buttonPin) == HIGH) {      |       if (button)                              |
|       digitalWrite(ledPin, HIGH);              |         oscillator >> led;                     |
|       delay(500); // do nothing for 500ms      |     }                                          |
|       digitalWrite(ledPin, LOW);               |                                                |
|       delay(500); // do nothing for 500ms      |                                                |
|     }                                          |                                                |
|   }                                            |                                                |
+------------------------------------------------+------------------------------------------------+

.. _compatible with Arduino:

Arduino-compatible
------------------

Plaquette is installed as an Arduino library and provides a replacement for the
core Arduino functionalities while remaining fully compatible with Arduino code.
Seasoned Arduino users should consult the :doc:`advanced` section for some tips
on how to integrate Plaquette into their existing code.

The following example uses Plaquette to control a blinking LED that slows down with
each button push, using Arduino's `constrain() <https://docs.arduino.cc/language-reference/en/functions/math/constrain/>`_
to keep the LED oscillation period within a certain range and `Serial <https://docs.arduino.cc/language-reference/en/functions/communication/serial/>`_ object
to reset the counter to a random integer value using `random() <https://docs.arduino.cc/language-reference/en/functions/random-numbers/random/>`_.

.. code-block:: c++

   #include <Plaquette.h>

   DigitalIn button(2, INTERNAL_PULLUP); // button input

   DigitalOut led(LED_BUILTIN); // LED output

   Wave oscillator(1.0); // square oscillator

   int currentPeriod = 0; // oscillator period counter

   void begin() {
      button.debounce(); // debounce button
   }

   void step() {
      if (Serial.read() == 'R') // reset counter
        currentPeriod = random(1, 10);

      if (button.rose()) // true when value rises (ie. button is pushed)
        currentPeriod = constrain(currentPeriod+1, 1, 10); // increment

      oscillator.period(currentPeriod); // set period
      oscillator >> led; // send signal to LED
   }

.. danger::
   Plaquette needs the main processing loop to run continuously without interruption
   to work correctly. Users should thus **avoid using blocking processes** such as Arduino's
   `delay() <https://docs.arduino.cc/language-reference/en/functions/time/delay/>`__ and
   `delayMicroseconds() <https://docs.arduino.cc/language-reference/en/functions/time/delayMicroseconds/>`__ and
   functions in their code when using Plaquette.

.. warning::
   Many of the core Arduino functions work with integer types such as ``int`` or ``long``
   rather than floating-point types such as ``float``. Plaquette provides alternative
   :doc:`functions <functions>` which should be used instead.

   In particular, please use:

   * :doc:`mapFloat` instead of `map() <https://docs.arduino.cc/language-reference/en/functions/math/map/>`_
   * :doc:`randomFloat` instead of `random() <https://docs.arduino.cc/language-reference/en/functions/random-numbers/random/>`_
   * :doc:`seconds` instead of `millis() <https://docs.arduino.cc/language-reference/en/functions/time/millis/>`_

.. warning::
   Plaquette is still at an experimental stage of development. If you
   have any issues or questions, please contact the developers, or file
   a bug in our `issue tracker <https://github.com/SofaPirate/Plaquette/issues>`_.
