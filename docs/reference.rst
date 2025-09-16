Reference
=========

Base Units
----------

* :doc:`AnalogIn` Reads analog input values between 0 and 1. Typically used for sensors that output a range of values, such as potentiometers or light sensors.
* :doc:`AnalogOut` Writes analog output values between 0 and 1. Useful for controlling devices like dimmable LEDs or motor controllers.
* :doc:`DigitalIn` Reads digital input values as boolean true/false. Commonly used with buttons, switches, or other on/off signals.
* :doc:`DigitalOut` Writes digital output values as boolean true/false. Often used to control relays, LEDs, or other binary devices.

Generators
----------

* :doc:`Ramp` Generates a linear ramp signal over time. Commonly used for smooth parameter transitions like fading lights or scaling values.
* :doc:`Wave` Generates a smooth, periodic wave signal with a square, triangle, or sine shape. Commonly used for oscillatory motion or smooth transitions.

Timing
------

* :doc:`Alarm` Triggers an event after a specified time duration. Can be used to schedule delays or time-based actions.
* :doc:`Chronometer` Measures elapsed time since the start or reset. Useful for timing events or profiling system performance.
* :doc:`Metronome` Produces a periodic tick at specified intervals. Often used in rhythmic applications such as sound or light pulses.

Filters
-------

* :doc:`MinMaxScaler` Scales signals to fit within a specified minimum and maximum range. Essential for normalizing input signals from diverse sources.
* :doc:`Normalizer` Adjusts signals to have a zero mean and unit variance. Useful in signal processing pipelines where consistent scaling is required.
* :doc:`PeakDetector` Detects peaks (local maxima) in input signals, allowing for event-based processing such as edge detection.
* :doc:`Smoother` Reduces noise and fluctuations in input signals using smoothing algorithms like exponential moving averages.

Fields
------
* :doc:`PivotField` Generates a spatial response curve based on a pivot point around which the field transitions happens, making it ideal for creating animations such as VU-meters or fades on arrays of actuators such as LEDs or motors.
* :doc:`TimeSliceField` Collects values over time which can then be sampled spatially like an array accross a normalized range. Useful for plotting time-varying signals, such as mapping audio or sensor input onto an LED strip or a motor array.

Functions
---------

* :doc:`mapFloat` Maps a float value from one range to another. Useful for adapting input ranges to the desired output domain.
* :doc:`mapFrom01` Maps a float value from the normalized [0,1] range to a custom range, such as [-10, 10].
* :doc:`mapTo01` Maps a float value from a custom range to the normalized [0,1] range, simplifying calculations for normalized operations.
* :doc:`randomFloat` Generates a random float between 0 and 1, ideal for simulations or procedural generation.
* :doc:`seconds` Returns the current time in seconds since the program started, enabling precise time tracking.
* :doc:`wrap` Wraps a value within a specified range, making it cyclic. Commonly used for angles or periodic parameters.

Structure
---------

* :doc:`Engine` A control structure managing an ensemble of units, handling their initialization, update, and timing, ensuring they remain synchronized.
* :doc:`begin` Initializes the system, similar to Arduino's `setup()` function. Sets up necessary configurations and prepares units for operation.
* :doc:`step` Repeatedly called during the program's execution, akin to Arduino's `loop()` function. Drives the execution of the main logic.
* :doc:`array` Allows the creation of arrays of Plaquette units for batch operations. Facilitates efficient processing of multiple units simultaneously.
* :doc:`dot` Provides access to an object's methods and data, enabling intuitive object-oriented programming with Plaquette units.
* :doc:`pipe` Sends data across units from left to right, creating a streamlined and intuitive flow of information between connected units.

Extra
-----

* :doc:`easings` Provides easing functions for smooth and natural transitions between values. Commonly used in animations and motion design.
* :doc:`ContinuousServoOut` Controls a continuous rotation servo motor by setting its speed and direction. Ideal for robotics or mechanical motion control.
* :doc:`ServoOut` Controls a standard servo motor by setting its angle. Useful for applications like robotic arms or pan-tilt systems.
* :doc:`StreamIn` Streams input data continuously, allowing real-time signal processing from external devices.
* :doc:`StreamOut` Streams output data continuously, enabling real-time control of external actuators or visualizations.
