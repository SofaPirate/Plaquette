============
Using Waves
============

In this section, we will explore oscillators, essential tools for creating dynamic and expressive
media. Oscillators generate repeating waveforms, which can control various outputs such as LEDs or
motors. We'll also learn how to visualize and manipulate oscillators using Plaquette's features.
The concept of modulation will be introduced, allowing one wave to influence another.

To follow along with the examples, set up a simple circuit:

- A **potentiometer** connected to ``A0`` to control parameters dynamically.
- A **button** connected to pin ``2`` with an internal pull-up resistor to trigger actions.
- An **LED** connected to pin ``9`` (PWM capable) through a 330 :math:`\Omega` resistor.

Visualizing Waves with the Serial Plotter
-----------------------------------------

In this section, we will use **serial communication** to send data from our Arduino board to our
PC so as to visualize the waves in real time. The ``print()`` and ``println()`` functions allow 
you to send data to the serial, which is invaluable for debugging and visualizing data. They will
provide a way to graphically observe how wave parameters like amplitude, phase, or frequency affect 
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

Core Parameters
---------------

Oscillators are defined by their **phase**, **period**, **frequency**, **amplitude**, and **width**. 
Let us explore these concepts and their corresponding functions:

- **phase()**: Sets the initial point in the wave cycle (in range [0, 1]).
- **period()**: Sets the duration of one cycle in seconds.
- **frequency()**: Inverse of period; sets the cycles per second (Hz).
- **amplitude()**: Sets the peak level of the wave (as % of max) (in range [0, 1]);
- **width()**: Controls the balance between the rising and falling portions of the wave cycle (in range [0, 1]).
  For each wave type, this parameter has a specific effect:

    - For :doc:`SquareWave`, it adjusts the duty cycle (the ratio of ON to OFF time).
    - For :doc:`TriangleWave`, it determines whether the wave skews towards a sawtooth (width = 0) or inverted sawtooth (width = 1).
    - For :doc:`SineWave`, it shifts the inflection points of the wave, altering its symmetry.

There parameters can be assigned at the beginning of the program or during runtime. Here are a few
examples showing the use of wave parameters.

**Example**: Assign some parameters of a wave at program startup:

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

**Example**: Read the wave's period to increase it each time the button is pressed:

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

Try using the potentiometer to control the different different parameters and visualize the 
result using the Serial Plotter.

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

The `shiftBy()` function allows you to offset the phase of an oscillator relative to its current
position and returns the new phase as a float. This is useful for creating complex, synchronized
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

Modulation
----------

Modulation involves using one oscillator to influence the parameters of another, creating rich and
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

Conclusion
----------

Oscillators are powerful tools for creating dynamic, expressive systems. By combining their
waveforms, timing functions, and phase-shifting capabilities, you can achieve intricate and
synchronized behaviors. Modulation adds another layer of complexity, enabling you to create
engaging and responsive media systems. Explore these features and see how oscillators can bring your
projects to life.