.. include:: defs.hrst

Value Units
===========

Plaquette's **value-units** ``Float``, ``Integer``, and ``Boolean`` wrap basic types
(``float``, ``int`` or ``bool``), allowing to use them transparently as **units** in a
Plaquette flow. These units can be used like a normal variables but also send to and receive
from other units using the flow operator (``>>``). You can think of value-units as
*variables that can participate in data flows*.

The :doc:`flow operator >> <flow>` connects is **not** compatible with primitive types such
as ``float``, ``int``, and ``bool``.

**Value units** address this problem by turning primitive variables into Plaquette units.

This code shows valid and invalid uses of the flow operator:

.. code-block:: c++

   #include <Plaquette.h>

   Wave wave(1.0f); // wave oscillator
   DigitalOut led(LED_BUILTIN); // output LED
   float x = 0.0f;  // primitive type variable (float)

   void step() {
     wave >> led; // ✅ valid   : unit  >> unit
     x    >> led; // ✅ valid   : float >> unit
     wave >> x;   // ❌ invalid : unit  >> float
   }

|Example|
---------

This example combines two oscillators, stores the result in a ``Float`` unit, and reuses it
two control two LEDs.

.. code-block:: c++

   #include <Plaquette.h>

   // Create two wave oscillators.
   Wave waveA(SINE, 1.0f);
   Wave waveB(SINE, 1.2f);

   // Create two LED outputs.
   AnalogOut led1(9);
   AnalogOut led2(11);

   // Create a Float to store the combined signal.
   Float mix;

   void step() {
     // Sum the two waves and store the result.
     (waveA + waveB) >> mix;
     // Compute average.
     mix /= 2;
     // Reuse the same value in multiple places.
     mix >> led1;
     // Use math to invert the signal.
     (1-mix) >> led2;
   }


|Example|
---------

This example uses the ``Boolean`` unit type to combine two button states into a single
logical condition. It activates an LED only when both buttons are pressed and plots
the states.

.. code-block:: c++

   #include <Plaquette.h>

   // Create two button inputs.
   DigitalIn buttonA(BUTTON_A_PIN);
   DigitalIn buttonB(BUTTON_B_PIN);

   // Create LED output.
   DigitalOut led(LED_BUILTIN);

   // Create monitor.
   Plotter plotter(115200, "pressA,pressB,pressBoth"); // plotter with baud rate and labels

   // Create a Boolean to store the combined condition.
   Boolean bothPressed = false;

   void step() {
     // Combine the two button states.
     (buttonA && buttonB) >> bothPressed;
     // Send to LED.
     bothPressed >> led;
     // Plot states.
     buttonA >> plotter;
     buttonB >> plotter;
     bothPressed >> plotter;
   }


|SeeAlso|
---------
- :doc:`flow`
