.. include:: defs.hrst

Advanced Usage
==============

Smoothing Arbitrary Signals
---------------------------

While the :doc:`AnalogIn` unit in Plaquette provide a convenient built-in ``smooth()`` function for removing 
noise, there are many cases where you may need to smooth signals coming from other sources such as specialized
sensors. The :doc:`Smoother` unit provides a highly flexible smoothing solution for such use cases, allowing 
seamless integration into any signal pipeline. It works using an exponential moving average, acting as a 
low-pass filter to stabilize fast variations.

Here is an example of using the :doc:`Smoother` to smoothen a DHT 22 temperature and humidity sensor using the
external `DHT sensor library <https://docs.arduino.cc/libraries/dht-sensor-library/>`__:

.. code-block:: c++

    #include <Plaquette.h>
    #include <DHT.h> // External specialized library

    DHT dht(2, DHT22); // DHT 22 sensor connected to pin 2

    // Create a Smoother with a 10-second time window.
    Smoother temperatureSmoother(10.0);

    // Stream out for debugging (e.g., to Serial Monitor).
    StreamOut serialOut(Serial);

    void begin() {
      dht.begin();  // Initialize the DHT sensor
    }

    void step() {
      // Read temperature in Celsius.
      float rawTemperature = dht.readTemperature();

      // Smooth the temperature and send it to the Serial.
      rawTemperature >> temperatureSmoother >> serialOut;
    }

Vanilla Coding Style
--------------------

You can avoid Plaquette's :doc:`>> <pipe>` operator or auto-conversion of units to values 
(eg., ``if (input)``, ``input >> output``) in favor of a more conventional programming style
by simply using the ``get()`` and ``put()`` functions of Plaquette units.

The ``get()`` method returns the current value of the unit:

.. code:: cpp

  float get()

The ``put()`` method sends a value to the unit and then returns the current value
of the unit (the same that would be returned by ``get()``):

.. code:: cpp

  float put(float value)

Additionally, digital input units such as :doc:`DigitalIn`, :doc:`Metronome` 
have a ``boolean isOn()`` method that works for boolean ``true/false`` values,
while digital output units such as :doc:`DigitalOut` have a ``boolean putOn(boolean value)`` 
method.

Here are some examples of how to adopt a classic object-oriented functions style
instead of the Plaquette style.

.. list-table::
  :widths: 50 50
  :header-rows: 1

  * - Plaquette Style
    - Object-Oriented Style
  * - ``input >> output;``
    - ``output.put(input.get());``
  * - ``digitalInput >> digitalOutput;``
    - ``digitalOutput.putOn(digitalInput.isOn());``
  * - ``(2 * input) >> output;``
    - ``output.put(2 * input.get());``
  * - ``!digitalInput >> digitalOutput;``
    - ``digitalOutput.putOn(!digitalInput.isOn());``
  * - ``if (digitalInput)``
    - ``if (digitalInput.isOn())``
  * - ``if (input < 0.4)``
    - ``if (input.get() < 0.4)``
  * - ``input >> filter >> output;``
    - ``output.put(filter.put(input.get()));``

Using Plaquette as an External Library
--------------------------------------

Seasoned Arduino coders might want to avoid rewriting their code using Plaquette's
builtin ``begin()`` and ``step()`` functions, or they may want to include Plaquette's
self-updating loop in a timer interrupt function. It is possible to do so by
including the file ``PlaquetteLib.h`` instead of ``Plaquette.h``.

After this step, you is then responsible for calling ``Plaquette.begin()`` at the beginning of the
``setup()`` function, and also to call ``Plaquette.step()`` at the beginning of the
``loop()`` function, or inside the interrupt.

Here is an example of our blinking code rewritten by using this feature:

.. code:: cpp

    #include <PlaquetteLib.h>

    using namespace pq;

    DigitalOut myLed(13);

    SquareWave myWave(2.0, 0.5);

    void setup() {
      Plaquette.begin();
    }

    void loop() {
      Plaquette.step();
      myWave >> myLed;
    }


.. _secondary-engines:
  
Using Secondary Engines
-----------------------

Have you wondered how units such as waves, inputs, outputs, or ramps are automatically initialized and updated in Plaquette?
This is done thanks to an :doc:`Engine`, a control structure that acts like the **conductor of an orchestra**.
It contains an ensemble of **units** and manages their initialization and updates. Every time the engine "ticks", 
it updates all of its units, making sure they stay synchronized.

By default, all units are added to a built-in engine called the **primary engine** called ``Plaquette``. This is the object
that is used when one wants to use Plaquette as an external library. In the default mode, when one declares ``void begin()`` 
and ``void step()`` functions, the primary engine's ``begin()`` and ``step()`` functions are called automatically.

However, there are contexts where you might need more than one engine:

- **Multi-tasking** Engines allow you to take full advantage of **timer interrupts**, **threads** and/or **multiple processor cores**
  to run different unit ensembles in parallel, possibly running with different frequencies and priorities.
- **Grouping** Engines can be used to better organize your code by creating **groups of units** and possibly run them at different frequencies.
- **Switching** On computationally-intensive applications with lots of units, you may want to **switch between multiple ensembles of 
  units** to avoid running them all at the same time.
- **Saving Energy** Lowering the update frequency of units using engines allows for more energy-efficient applications.

In these cases, you can create **secondary engines** that each control their own group of units at their own refresh rate. You can 
step them in a timer interrupt, in a task running on another core, or even from a :doc:`Metronome` unit.

To create an engine, simply declare it:

.. code-block:: cpp

  Engine secondaryEngine;

When you create a unit, you can now add it to your new engine by adding the engine as the last argument of the unit's constructor:

.. code-block:: cpp

  // Ramp starting at default value.
  Ramp ramp(secondaryEngine);

  // Alarm unit with 10s duration.
  Alarm alarm(10.0, secondaryEngine);

  // Square wave unit with period of 1s and 20% width.
  SineWave wave(1.0, 0.2, secondaryEngine);

Example: Fast vs Slow Control
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In this example we will control a blinking LED with a pushbutton: every time we push the button, the LED will blink faster 
and faster. The button should be polled quite frequently to ensure it is debounced properly. However, there is no need to update 
the LED as fast as possible, so we can save some precious computation steps by updating it less often (about 25 frames per second 
would be plenty for such visual feedback).

- The **slow engine** (running at 25 fps) will control the LED with the square wave.
- The **fast engine** (running at 1000 Hz) will monitor the button.
- The **primary engine** (running as fast as possible) will use :doc:`Metronome` units to trigger the slow and fast engines.

.. code-block:: cpp

  #include <Plaquette.h>

  // The engines.
  Engine slowEngine;
  Engine fastEngine;

  // Metronomes (belong to primary engine).
  Metronome slowMetro(0.04);  // 25 fps
  Metronome fastMetro(0.001); // 1000 Hz

  DigitalIn button(2, INTERNAL_PULLUP, fastEngine); // Button (operates on fast engine).

  // Oscillator and LED (can operate more slowly to save on computation).
  SquareWave squareWave(1.0, slowEngine);
  DigitalOut led(LED_BUILTIN, slowEngine);

  float ledFrequency = 1.0; // Oscillation frequency.
  
  void begin() {
    // Initialize engines.
    slowEngine.begin();
    fastEngine.begin();
    button.debounce(); // Debounce button.
    // Attach metronomes to engine step functions.
    slowMetro.onBang(slowEngineStep);
    fastMetro.onBang(fastEngineStep);
  }

  void step() {}

  void slowEngineStep() {
    slowEngine.step(); // Update slow engine.
    // Adjust frequency and send to LED.
    squareWave.frequency(ledFrequency);
    squareWave >> led;
  }

  void fastEngineStep() {
    fastEngine.step(); // Update fast engine.
    // On button press, increase square wave frequency.
    if (button.rose())
      ledFrequency++;
  }

Example: ESP32 Dual Core
~~~~~~~~~~~~~~~~~~~~~~~~

In this example, we will take full advantage of the ESP32's dual core architecture:

- **Core 1 (default)** will run the **primary engine** to update a simple indicator LED.
- **Core 0** will run a **secondary engine** to animate Neopixel LEDs using a waveform.

.. code-block:: cpp

    #include <Plaquette.h>
    #include <Adafruit_NeoPixel.h>

    // Pin where the Neopixel strip is connected
    #define STRIP_PIN  5
    #define NUM_LEDS  16

    // The NeoPixel LED strip.
    Adafruit_NeoPixel strip(NUM_LEDS, STRIP_PIN, NEO_GRB + NEO_KHZ800); // Neopixels.

    SquareWave indicatorLedBlink(1.0, 0.2); // Blinking oscillator.
    DigitalOut indicatorLed(LED_BUILTIN);   // Indicator LED.

    Engine ledEngine; // Secondary engine for controlling NeoPixels.
    SineWave ledWave(1.0, ledEngine);  // Waveform for NeoPixels.
    
    void begin() {
      xTaskCreatePinnedToCore( // Launch LED engine on Core 0.
        [] (void* param) {
          ledEngine.begin(); // Start LED engine.

          // Initialize LED strip.
          strip.begin();
          strip.show();

          // Main loop.
          while (true) {
            ledEngine.step(); // Step engine.
            stepLeds();       // Update LEDs.
            vTaskDelay(1);    // Important! Prevents IDLE on Core 0 (1 tick = ~1ms).
          }
        },
        "LED Engine", 2048, nullptr, 1, nullptr,
        0 // Core 0
      );
    }

    // Primary engine step function.
    void step() {
      indicatorLedBlink >> indicatorLed; // Blink.
    }

    // Step function for the LED engine.
    void stepLeds() {
      // Update LED strip according to LED wave.
      for (int i = 0; i < NUM_LEDS; i++) {
        uint8_t level = ledWave.shiftBy( mapTo01(i, 0, NUM_LEDS-1) )
        strip.setPixelColor(i, strip.Color(level, 0, 0)); // Red only
      }
      strip.show(); // Display LEDs.
    }


Multiple engines give you more control and better performance, especially on multi-core platforms or in time-sensitive applications like 
LED control, audio, or robotics.