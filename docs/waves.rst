============
Using Waves
============

In this section, we will explore waves (also called oscillators), essential tools for creating dynamic 
and expressive media. Oscillators generate repeating waveforms, which can control various outputs such 
as LEDs or motors. We will also learn how to visualize and manipulate oscillators using Plaquette's features.
The concept of modulation will be introduced, allowing one wave to influence another.

To follow along with the examples, set up a simple circuit:

- A **potentiometer** connected to ``A0`` to control proprties dynamically.
- A **button** connected to pin ``2`` with an internal pull-up resistor to trigger actions.
- An **LED** connected to pin ``9`` (PWM capable) through a 330 :math:`\Omega` resistor.

Visualizing Waves with the Serial Plotter
-----------------------------------------

In this section, we will use **serial communication** to send data from our Arduino board to our
PC so as to visualize the waves in real time. The ``print()`` and ``println()`` functions allow 
you to send data to the serial, which is invaluable for debugging and visualizing data. They will
provide a way to graphically observe how wave properties like amplitude, phase, or frequency affect 
the output.

To visualize the data, open the **Serial Plotter** in the Arduino IDE. The Serial Plotter can 
graphically display waveforms by interpreting each printed value as a separate line on the graph.

**Example**: Print the value of the potentiometer:

.. code-block:: cpp

    #include <Plaquette.h>

    AnalogIn pot(A0); // The potentiometer

    void begin() {}

    void step() {
      println(pot); // Print the potentiometer value and ends the line
    }

For multiple waveforms, print their values separated by spaces in a single line, followed by a 
newline using ``println()``.

**Example**: Print the value of the potentiometer and a sine wave:

.. code-block:: cpp

    #include <Plaquette.h>

    AnalogIn pot(A0);   // The potentiometer
    SineWave wave(2.0); // Sine wave with period of 2 seconds

    void begin() {}

    void step() {
      print(wave);  // Print wave value
      print(" ");   // Print white space
      println(pot); // Print the potentiometer value and ends the line
    }


Types of Waves
--------------

Plaquette provides 3 types of oscillators:

- :doc:`SquareWave`: Alternates between two levels with sharp transitions. Useful for creating rhythmic
  on-off patterns such as blinking LEDs or simple tone generators for buzzers. Possesses some properties
  of digital units.

- :doc:`TriangleWave`: Smoothly transitions between two levels in a linear fashion. By varying the
  width of the wave, you can create a **sawtooth wave** (width = 0) or an **inverted sawtooth wave**
  (width = 1). This is ideal for simulating ramping motions or gradual changes in brightness.

- :doc:`SineWave`: Produces a sinusoidal waveform for smoother modulation. Commonly used for
  creating natural, flowing transitions, such as smooth dimming or speed control.

You can visualize these waves on the Serial Plotter by streaming their values:

**Example**: Display different waves for comparison:

.. code-block:: cpp

    #include <Plaquette.h>

    SquareWave square(1.0);
    TriangleWave triangle(1.0);
    SineWave sine(1.0);

    void begin() {}

    void step() {
      // Print all wave values separated by spaces
      print(square); print(" ");
      print(triangle); print(" ");
      println(sine);
    }

Core Properties
---------------

Oscillators are defined by their **phase**, **period**, **frequency**, **amplitude**, and **width**. 
Let us explore these properties and their corresponding functions:

- **phase()**: Sets the initial point in the wave cycle (in range [0, 1]).
- **period()**: Sets the duration of one cycle in seconds.
- **frequency()**: Inverse of period; sets the cycles per second (Hz).
- **bpm()**: Alternative way to set the frequency using beats per minute (BPM).
- **amplitude()**: Sets the peak level of the wave (as % of max) (in range [0, 1]);
- **width()**: Controls the balance between the rising and falling portions of the wave cycle (in range [0, 1]).
  For each wave type, this property has a specific effect:

    - For :doc:`SquareWave`, it adjusts the duty cycle (the ratio of ON to OFF time).
    - For :doc:`TriangleWave`, it determines whether the wave skews towards a sawtooth (width = 0) or inverted sawtooth (width = 1).
    - For :doc:`SineWave`, it shifts the inflection points of the wave, altering its symmetry.

There properties can be assigned at the beginning of the program or during runtime.

**Example**: Assign some properties of a wave at program startup:

.. code-block:: cpp

    #include <Plaquette.h>

    TriangleWave wave;

    void begin() {
      wave.frequency(2); // 2 Hz
      wave.width(0.9); // width 90%
      wave.phase(0.1); // dephased by 10% of period
      wave.amplitude(0.5); // 50% amplitude
    }

    void step() {
      println(wave); // Print wave value
    }

**Example**: Control the width of the waves using the potentiometer:

.. code-block:: cpp

    #include <Plaquette.h>

    AnalogIn pot(A0);   // The potentiometer

    SquareWave square(1.0);
    TriangleWave triangle(1.0);
    SineWave sine(1.0);

    void begin() {}

    void step() {
      // Assign new width value.
      square.width(pot);
      triangle.width(pot);
      sine.width(pot);
      // Print all wave values separated by spaces
      print(square); print(" ");
      print(triangle); print(" ");
      println(sine);
    }

**Example**: Control the period of the waves using the potentiometer. Necessitates remapping
potentiometer value to appropriate ranges.

.. code-block:: cpp

    #include <Plaquette.h>

    AnalogIn pot(A0);   // The potentiometer

    SquareWave square(1.0);
    TriangleWave triangle(1.0);
    SineWave sine(1.0);

    void begin() {}

    void step() {
      // Read new period value.
      float newPeriod = pot.mapTo(0.5, 5); // Map to 0.5-5 seconds period
      // Assign new period value.
      square.period(newPeriod);
      triangle.period(newPeriod);
      sine.period(newPeriod);
      // Print all wave values separated by spaces
      print(square); print(" ");
      print(triangle); print(" ");
      println(sine);
    }

Try using the potentiometer to control the different different properties and visualize the 
result using the Serial Plotter.

Accessors and Mutators
~~~~~~~~~~~~~~~~~~~~~~

All properties in wave units have two variants:

- A **mutator** variant allowing to change the value of the property. Example: ``wave.period(3.0);``.
- An **accessor** read-only variant that returns the current value of the property. Example: ``float value = wave.period();``

**Example**: Increase the wave's period by one second each time the button is pressed:

.. code-block:: cpp

    #include <Plaquette.h>

    DigitalIn button(2, INTERNAL_PULLUP);

    TriangleWave wave(1.0);

    void begin() {}

    void step() {
      if (button.rose()) {
        wave.period( wave.period() + 1 ); // Set period to period plus one
      }
      println(wave); // Print wave value
    }

Wave Addition
-------------

Adding waves together allows for the creation of complex and dynamic waveforms. By superimposing 
multiple signals, you can simulate natural phenomena, generate rhythmic patterns, or create rich 
textures for artistic applications. In Plaquette, wave addition is as simple as summing the values 
of different waves, with the result automatically normalized to stay within the range [0, 1].

One compelling example of wave addition is simulating a **heartbeat**. A heartbeat typically has 
two peaks: a stronger primary beat followed by a softer secondary beat. This can be achieved by adding 
two waves with different amplitudes and timings.

**Example**: Heartbeat simulation. This example uses two :doc:`SineWave` units: one for the primary 
beat one for the secondary beat. The ``bpm()`` function sets the frequency of the waves in beats 
per minute.

.. code-block:: cpp

    #include <Plaquette.h>

    SineWave primary;   // Main heartbeat wave
    SineWave secondary; // Secondary beat
    AnalogOut led(9);   // LED for visualizing the heartbeat

    void begin() {
      primary.bpm(80); // Set primary beat to 80 beats per minute
      secondary.bpm(2*primary.bpm()); // Set secondary beat to twice primary BPM
      secondary.amplitude(0.8); // Secondary beat is less strong
    }

    void step() {
      float heartBeat = (primary + secondary) / 2; // Combine and normalize waves
      led.put(heartBeat);  // Drive LED with combined signal
      println(heartBeat);  // Stream the combined wave for visualization
    }

In this simulation, the `primaryBeat` provides the dominant rhythm, while the `secondaryBeat` introduces a softer, complementary pulse. The resulting waveform mimics the double-thump pattern of a human heartbeat.

Try experimenting with different wave types, amplitudes, and frequencies to see how the combined waveform changes. Wave addition opens up endless possibilities for creating expressive and engaging outputs.


Modulation
----------

Modulation involves using one oscillator to influence the properties of another, creating rich and
dynamic effects. For example, a slower wave (also called a **Low-Frequency Oscillator (LFO)**) can 
modulate the frequency, phase, period, amplitude, or width of a faster wave.

**Example**: Modulate the frequency of a sine wave with a triangle wave:

.. code-block:: cpp

    #include <Plaquette.h>

    TriangleWave modulator(10.0);
    SineWave sine;
    AnalogOut led(9);

    void begin() {}

    void step() {
      sine.frequency(modulator.mapTo(1.0, 10.0)); // Modulate frequency between 1 and 10 Hz
      sine >> led; // Drive LED with modulated sine wave
      println(sine); // Stream the modulated wave
    }


Make Some Noise
---------------

While oscillators are incredibly useful for generating regular and predictable waveforms, there are 
times when you may want to introduce randomness to add a sense of natural variation or lifelike behavior. 
Plaquette provides the `randomFloat()` function, which is a powerful tool for generating random values.

The `randomFloat()` function can be used in several ways:

- ``randomFloat()`` generates a random float between 0.0 and 1.0.
- ``randomFloat(max)`` generates a random float between 0.0 and ``max``.
- ``randomFloat(min, max)`` generates a random float between ``min`` and ``max``.

These random values can be used to add noise directly to a signal.

**Example**: Add noise to a sine wave.

.. code-block:: cpp

    #include <Plaquette.h>

    SineWave wave(1.0);
    AnalogOut led(9);

    void begin() {}

    void step() {
      float noise = randomFloat(-0.1, 0.1); // Generate noise value in [-0.1, 0.1]
      float noisyWave = wave + noise; // Compute sine value + noise
      noisyWave >> led;   // Drive LED with noisy sine wave
      println(noisyWave); // Stream the noisy sine wave
    }

These random values can also be used to modify properties such as amplitude, frequency, 
width, or phase.

**Example**: Update the wave's period according to a random walk. The potentiometer controls 
the amount of noise.

.. code-block:: cpp

    #include <Plaquette.h>

    AnalogIn pot(A0);
    SineWave wave(1.0);
    AnalogOut led(9);

    void begin() {}

    void step() {
      float noise = randomFloat(-pot, pot); // Generate noise according to potentiometer value
      wave.period( wave.period() + noise );
      wave >> led;   // Drive LED with noisy sine wave
      println(wave); // Stream the sine wave
    }

**Example**: Introduce randomness to the frequency of a triangle wave. Frequency updated on
each push of the button.

.. code-block:: cpp

    #include <Plaquette.h>

    DigitalIn button(2, INTERNAL_PULLUP);
    TriangleWave wave(1.0);
    AnalogOut led(9);

    void begin() {
      wave.frequency(5.0); // Start at 5 Hz
    }

    void step() {
      if (button.rose()) {
        wave.frequency(randomFloat(4.0, 6.0)); // Random frequency between 4 and 6 Hz
      }
      println(wave); // Stream the wave
    }

Randomness can also be combined with modulation to create highly dynamic and expressive behaviors. 
Experiment with adding random noise to various properties and observe the effects using the 
Serial Plotter. Try to simulate a natural phenomena like a flickering flame or a lightning bolt.

Timing Functions
----------------

Oscillators come with various timing functions to control their behavior:

- **start()**: Starts/restarts the oscillator.
- **stop()**: Stops it and resets it.
- **pause()**: Pauses the wave at its current point.
- **resume()**: Resumes from the paused point.
- **togglePause()**: Toggles between paused and running states.
- **isRunning()**: Returns whether the oscillator is active.
- **setTime()**: Sets the current phase of the oscillator based on absolute time (in seconds).

**Example**: Use the button to start and stop the wave:

.. code-block:: cpp

    #include <Plaquette.h>

    DigitalIn button(2, INTERNAL_PULLUP);
    SineWave sine;
    AnalogOut led(9);

    void begin() {
      sine.frequency(2.0); // 2 Hz
    }

    void step() {
      if (button.rose()) {
        sine.togglePause(); // Pause or resume the wave
      }      
      sine >> led; // Drive LED with sine wave
      println(sine); // Stream the wave for visualization
    }

Phase Shifting with shiftBy()
-----------------------------

The ``shiftBy()`` function allows you to offset the phase of an oscillator relative to its current
position and returns the value of the dephased wave. This is useful for creating complex, synchronized
patterns.

**Example**: Shift the phase of a sine wave:

.. code-block:: cpp

    #include <Plaquette.h>

    SineWave wave(1.0);

    void begin() {}

    void step() {
      // Print shifted values separated by white spaces.
      print(wave); print(" "); // 0% shift
      print(wave.shiftBy(0.25)); print(" "); // 25% shift
      print(wave.shiftBy(0.5)); print(" "); // 50% shift
      println(wave.shiftBy(0.75)); // 75% shift
    }

Conclusion
----------

Oscillators are powerful tools for creating dynamic, expressive systems. By combining their
waveforms, timing functions, and phase-shifting capabilities, you can achieve intricate and
synchronized behaviors. Modulation and randomness add another layer of complexity, enabling you 
to create engaging and responsive media systems. Explore these features and see how oscillators 
can bring your projects to life.