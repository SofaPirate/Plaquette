---
title: "*Plaquette*: An Object-Oriented Framework for Real-Time Signal Processing on Microcontrollers"
tags:
  - C++
  - Arduino
  - Embedded Systems
  - Creative Coding
  - Interactive Media
  - Signal Processing
  - Research-Creation
authors:
  - name: Sofian Audry
    orcid: 0000-0002-6340-2550
    equal-contrib: true
    affiliation: 1
  - name: Thomas Ouellet Fredericks
    orcid: 0009-0004-8587-9508
    equal-contrib: true
    affiliation: 2
affiliations:
  - name: Université du Québec à Montréal, Canada
    ror: 002rjbv21
    index: 1
  - name: Collège Montmorency, Canada
    ror: 05kqg2j33
    index: 2
date: 30 September 2025
bibliography: paper.bib
---

# Summary


*Plaquette* is an open-source, object-oriented C++ framework, built upon Arduino, for developing interactive applications on embedded microcontrollers, supporting a wide range of Arduino-compatible architectures including AVR, SAMD, STM32, and ESP32. It provides a signal-centric architecture and a suite of modular abstractions (oscillators, filters, units, and scheduling engines) that simplify the design of time-based behaviors in embedded interactive systems. Its expressive syntax object-oriented allows fast prototyping with multiple parallel sensors, actuators, and real-time processes, enabling both researchers and creative practitioners to experiment and design with complex interactive environments.

Beyond its technical contributions, *Plaquette* serves as a bridge between creation, observation of phenomena and design. Its applications, in interdisciplinary research projects involving expressive systems, affective biofeedback and basic motor control, demonstrate the framework’s potential for advanced, embodied, interdisciplinary approaches to interactive media while providing a robust infrastructure for scholarly inquiry.

# Statement of Need

 *Plaquette* is targeted for research in interactive media, digital lutherie, and robotic art. These research experiments depend on microcontrollers to sense, process, and actuate in real time. Arduino is the ubiquitous instrument used in this field. However, while Arduino does provide an accessible programming vocabulary, microprogramming in Arduino continues to be challenging due to lack of an integrated object-oriented architecture. This limits the use of abstractions for managing concurrent processes and signal processing. Moreover, dataflow environments popular in creative communities and scientific research on real-time interactive multimedia (e.g., Cycling '74 Max, Pure Data, Derivative TouchDesigner) provide powerful models for composing with signals, but they are computationally heavy and can usually not be exported to run on constrained hardware. Finally, scientific tools such as Python’s NumPy/SciPy, Matlab, or R, while offering rich signal analysis tools, are not designed for real-time signal processing on embedded devices.

*Plaquette* addresses this gap by bringing the expressive power of dataflow-style signal-based programming into a lightweight, efficient, object-oriented C++ framework optimized for microcontrollers (i.e., with low memory trace). It enables intuitive handling of multiple simultaneous parallel processes and provides real-time implementations of core signal processing functions such as peak detection, normalization, and scaling, providing flexible automated calibration using moving averages. This design enables researchers in art, science and engineering, as well as creative practitioners, to focus on experimentation and expressivity with tangible computing, while ensuring accurate and reliable real-time performance on resource-constrained platforms.

*Plaquette* also provides the groundwork for workshop-based and participatory research-creation projects, in which researchers and participants from creative communities collaboratively design and test interactive systems. This methodology is increasingly important in research-creation contexts in media arts, design, and HCI where embodied interaction and situated practices require adaptable prototyping tools that can be learned easily.

The framework has been used to as part of public research projects, illustrating its broad applicability. It was
used to improve real-time physiological signal processing as part of the [BioData](https://github.com/eringee/BioData)
library for affective biofeedback, supporting creative applications in music and performance [Gee2023-BioSynth], and used in
studies of electrodermal activity measurement [Hagler2022-Flexible]. It was integrated at the core of the
[MisBKit](https://misbkit.ensadlab.fr), a robotic kit enabling researchers and practitionners to explore object behaviors
from artistic and scientific perspectives. It was also employed for signal processing and LED-based robotic expression in the
artwork *Morphosis*, an installation featuring three spheroid robots that learn in real-time using reinforcement learning
[Audry2020-Behaviour,Audry2024-Choreomata]. These examples illustrate the framework’s role not only as a technical tool but also
as a catalyst for interdisciplinary exploration.

# Functionality and Design Overview


The core of *Plaquette* is organized around two complementary abstractions called **units** and **engines** that provide a coherent structure for building complex, real-time interactive systems on microcontrollers. Units are modular building blocks that encapsulate behaviors such as sensing, generating, filtering, or actuating. Engines act as deterministic schedulers, managing initialization and timing of units so that they execute consistently without blocking or interruptions.

All units implement a simple, unified interface consisting of a single input and a single output function. This design makes it possible to **chain units together in a dataflow-like manner** using a special piping operator (``>>``), where the output of one unit is sent as input to another unit. This signal-centric approach allows developers to think in terms of flows of information rather than low-level procedural code, similar to how artists and researchers work with dataflow software, but here optimized for resource-constrained hardware.

The framework includes a variety of unit types:
- **Base units**: basic analog and binary input/output.
- **Generators**: generative source signals such as square, triangle, and sine waves.
- **Timing units**: scheduling and temporal control such as timers and ramps.
- **Filters**: real-time signal transformations such as min-max scaling, normalizing, and detecting peaks.
- **Fields**: spatial functions sampled at fractional positions to plot, shape, or transform signals across space.

Engines and units are designed with a very low memory footprint to accommodate microcontrollers with limited RAM and processing power. Memory can be typically assigned statically at compile time, avoiding dynamic allocation and fragmentation. For example, filters such as min-max scaling and normalizing implement efficient exponential moving averages rather than circular buffers, ensuring predictable memory usage.

# Examples

The following program chains an analog input through a min-max scaling filter to control an LED:

```cpp
  #include <*Plaquette*.h>

  AnalogIn input{A0};         // analog input on pin A0
  AnalogOut led{9};           // PWM-controlled LED on pin 9
  MinMaxScaler scaler{};      // min-max scaler

  void begin() {}

  void step() {
    inputs >> scaler >> led;  // rescale to range [0, 1] and send to LED
  }
```

This program reacts to peaks in the incoming signal by triggering a sudden movement (ramp) in a servo motor:

```cpp
  #include <*Plaquette*.h>

  AnalogIn input{A0};          // analog input on pin A0
  ServoOut servo{9};           // servomotor connected on pin 9
  Normalizer normalizer{0, 1}; // re-normalizes to N(0, 1)
  PeakDetector peak{1.5};      // triggers when detecting outliers at 1.5 stddev
  Ramp ramp{2.0};              // ramp with duration of 2 seconds (by default: ramps from 0 to 1)

  void begin() {
    normalizer.timeWindow(60); // normalizing calibration sliding time window: 60 seconds
  }

  void step() {
    inputs >> normalizer >> peak; // process input signal
    if (peak) ramp.start(); // on peak detection: restart ramp
    ramp >> servo; // send ramp value to servo motor
  }
```

# Acknowledgements

This work was partially supported by the Natural Sciences and Engineering Research Council of Canada, the Social Sciences and Humanities Research Council of Canada, the Fonds de Recherche du Québec — Société et Culture, and the Canada Council for the Arts. We thank our collaborators and colleagues for supporting the project, in particular Erin Gee, Luana Belinsky, and Chris Salter.
