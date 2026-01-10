.. include:: defs.hrst

Value Units
===========

Plaquette's **value-units** ``Float``, ``Integer``, and ``Boolean`` wrap basic types
(``float``, ``int`` or ``bool``), allowing to use them transparently as **units** in a
Plaquette flow. These units can be used like a normal variables but also send to and receive
from other units using the flow operator (``>>``). You can think of value-units as
*variables that can participate in data flows*.

Why base types do not work with ``>>``
--------------------------------------

The :doc:`flow operator >> <flow>` connects Plaquette **units** and is not compatible with
base types such as ``float``, ``int``, and ``bool``. Hence, the following code results in
a compilation error:

.. code-block:: c++

   // Declare a wave oscillator.
   Wave wave;
   // Declare a plain float variable.
   float x = 0.0f;

   void step() {
     // This is invalid and will generate an error.
     wave >> x; // x is NOT a unit
   }

**Value units** address this problem by turning primitive variables into Plaquette units.

|Example|
---------

This example combines two oscillators, stores the result in a ``Float`` unit, and reuses it
two control two LEDs.

.. code-block:: c++

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
     mix /= 2; // 2 waves
     // Reuse the same value in multiple places.
     mix >> led1;
     (1-mix) >> led2; // inverse
   }

|Example|
---------

This example uses the ``Boolean`` unit type to combine two button states into a single
logical condition. It activates an LED only when both buttons are pressed and plots
the states.

.. code-block:: c++

   // Create two button inputs.
   DigitalIn buttonA(BUTTON_A_PIN);
   DigitalIn buttonB(BUTTON_B_PIN);

   // Create LED output.
   DigitalOut led(LED_BUILTIN);

   // Create monitor.
   Plotter plotter(115200, "pressA,pressB,pressBoth");

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
