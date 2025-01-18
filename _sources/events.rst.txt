===================
Working with Events
===================

Plaquette supports event-driven programming, allowing you to execute specific actions automatically when 
an event occurs instead of constantly checking for changes with an **if** statement. In Plaquette, this 
is achieved by **events** and function **callbacks**.

An **event** is an instantaneous situation that is triggered by a unit under specific conditions, such as
the push of a button, the tick of a metronome, the end of a timer, or a peak detection. 

A **callback** is a custom function that is registered with a source event: when the event is triggered, 
the registered callback is automatically called.

This approach offers several advantages:

- **Modularity**: Encapsulates behavior in reusable functions.
- **Expressiveness**: Focuses on declaring "what happens when".
- **Efficiency**: Reacts to changes without continuous polling.
- **Scalability**: Makes it easy to manage complex systems with multiple events.

Let us explore how this works with practical examples.

Supported Events
----------------

.. list-table::
  :header-rows: 0

  * - **Event:**
    - ``onRise()``
    - ``onFall()``
    - ``onChange()``
    - ``onBang()``
    - ``onFinish()``
  * - **Activation:**
    - Value rises
    - Value falls
    - Value changes
    - Unit fires
    - Time out
  * - :doc:`Alarm`
    - ✔
    - ✔
    - ✔
    - 
    - ✔
  * - :doc:`DigitalIn`
    - ✔
    - ✔
    - ✔
    - 
    - 
  * - :doc:`Metronome`
    - 
    - 
    - 
    - ✔
    - 
  * - :doc:`PeakDetector`
    - 
    - 
    - 
    - ✔
    - 
  * - :doc:`Ramp`
    - 
    - 
    - 
    - 
    - ✔

Reacting to an Event
--------------------

Let us take an example where we want to react to the push of a button by switching an LED on and off.

First, let us create the units we will be working with:

.. code-block:: cpp

   #include <Plaquette.h>

   DigitalOut led(LED_BUILTIN);  // LED connected to built-in pin
   DigitalIn button(2, INTERNAL_PULLUP);  // Button connected to pin 2

In order to react to an event, we first need to create a callback function which will be called when
the event will happen:

.. code-block:: cpp

   // Callback function to toggle the LED.
   void toggleLed() {
     led.toggle();
   }

Then, we need to register our callback to an event. In this case, we will register our function ``toggleLed()``
to the ``onRise()`` event of our button unit, which will trigger at the instant the button is pressed.

.. code-block:: cpp

   void begin() {
     button.debounce();  // Enable debouncing to avoid multiple events

     // Register callbacks for button events.
     button.onRise(toggleLed); // Toggle the LED on button press
   }

In this case, the ``step()`` function can be left empty because the callback will take care of all the logic.

.. code-block:: cpp

   void step() {} // Nothing to do here

Here is the final code for this example:

.. code-block:: cpp

   #include <Plaquette.h>

   DigitalOut led(LED_BUILTIN);  // LED connected to built-in pin
   DigitalIn button(2, INTERNAL_PULLUP);  // Button connected to pin 2

   // Callback function to toggle the LED.
   void toggleLed() {
     led.toggle();
   }

   void begin() {
     button.debounce();  // Enable debouncing to avoid multiple events

     // Register callbacks for button events.
     button.onRise(toggleLed); // Toggle the LED on button press
   }

   void step() {} // Nothing to do here

Now, try changing ``onRise()`` to ``onFall()`` or to ``onChange()``. How does that affect the interaction
between the button and the LED?

Managing Multiple Events
------------------------

It is possible to register multiple callbacks with the same event, and one callback can be assigned to many events.

Example: Launch both ``toggleLed()`` and ``printButton()`` on button press, registering ``printButton()`` to both
press and release events.

.. code-block:: cpp

   #include <Plaquette.h>

   DigitalOut led(LED_BUILTIN);  // LED connected to built-in pin
   DigitalIn button(2, INTERNAL_PULLUP);  // Button connected to pin 2

   // Callback function to toggle the LED.
   void toggleLed() {
     led.toggle();
   }

   // Callback function to print button state.
   void printButton() {
     print("Button ");
     println(button ? "pressed" : "released")
   }

   void begin() {
     button.debounce();  // Enable debouncing to avoid multiple events

     // Register callbacks for button events.
     button.onRise(toggleLed); // Toggle the LED on button press

     button.onRise(printButton); // Print button state
     button.onFall(printButton); // Same here
   }

   void step() {} // Nothing to do here

Coordinating Parallel Events with Metronomes
--------------------------------------------

There are many applications for which things happen concurrently at different pace, making
one wish there could be multiple ``step()`` functions being called in paralle. Plaquette makes 
it easy to manage multiple time-based events using :doc:`Metronome` units and event-driven
programming. Metronomes generate periodic "bang" events, allowing you to coordinate different actions 
running in parallel.

In this example, two metronomes control two LEDs, one digital and one analog, each at a different
interval. A ramp is used to fade the analog LED.

.. code-block:: cpp

   #include <Plaquette.h>

   DigitalOut led1(LED_BUILTIN);  // First LED (digital) connected to built-in pin
   AnalogOut led2(9); // Second LED (PWM) connected to pin 9
   Metronome metro1(1.0);  // Metronome with a one second period
   Metronome metro2(2.0);  // Metronome with a 2 second period
   Ramp rampLed(0.5); // Short ramp to control LED 2

   // Function to toggle the first LED.
   void pingLed1() {
     led1.toggle();
   }

   // Function to start the ramp on second LED.
   void pingLed2() {
     ramp.start();
   }

   void begin() {
     // Register callbacks for the metronomes.
     metro1.onBang(pingLed1);  // Toggle LED 1 every second.
     metro2.onBang(pingLed2);  // Fade in LED 2 every 2 seconds.
   }

   void step() {
     ramp >> led2; // Ramp second LED from 100% to 0%.
   }

Using Inline Callbacks
----------------------

For simple, localized actions, you can define callback functions directly inline using the 
``[]() { }`` syntax. This is called a **lambda function**. It allows you to write concise code 
without defining separate named functions and are thus especially useful for short, self-contained 
actions, keeping the code clean and readable.

For example, we could rewrite the callback registration from the example above in a shorter
way, like this:

.. code-block:: cpp

   void begin() {
     // Register callbacks for the metronomes.
     metro1.onBang([]() { led1.toggle(); }); // Toggle LED1 every second.
     metro2.onBang([]() { ramp.start(); });  // Light up and fade out LED2 every 2 seconds.
   }

Conclusion
----------

Event-driven programming in Plaquette simplifies the process of reacting to changes and scheduling 
actions, allowing you to write modular, expressive, and efficient code. By using callbacks and event 
sources like buttons and metronomes, you can manage complex behaviors that happen concurrently and at 
different rhythms.
