===================
Working with Events
===================

Plaquette supports event-driven programming, allowing you to execute specific actions automatically when 
an event occurs instead of constantly checking for changes with an **if** statement. In Plaquette, this 
is achieved by **events** and **function callbacks**.

A **callback** is a function that you write and then register with an event source, like a the rising
of a button or the tick of a metronome. When the event is triggered, the registered function is called 
automatically.

This approach offers several advantages:

- **Modularity**: Encapsulates behavior in reusable functions.
- **Expressiveness**: Focuses on declaring "what happens when".
- **Efficiency**: Reacts to changes without continuous polling.
- **Scalability**: Makes it easy to manage complex systems with multiple events.

Let us explore how this works with practical examples.

Reacting to an Event
--------------------

:doc:`DigitalIn` allows performing actions when events such as button presses or releases using 
callbacks. In this example, we register a function callback with the ``onRise()`` event which will 
trigger at the instant the button is pressed, resulting in the rise of its value (ie. goes from "off" 
to "on").

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

Managing Multiple Events
------------------------

On top of the ``onRise()`` event the :doc:`DigitalIn` unit supports the ``onFall()`` and the
``onChange()`` events. It is possible to register multiple callbacks with the same event, and one callback
can be assigned to many events.

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

Coordinating Multiple Events
----------------------------

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

Using Inline Functions
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
