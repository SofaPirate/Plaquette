---
title: "Plaquette: An Object-Oriented Framework for Embedded Signal Processing in Interactive Media"
tags:
  - Arduino
  - C++
  - Creative Coding
  - Dataflow
  - Embedded Systems
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

*Plaquette* is an open-source, object-oriented C++ framework for interactive media experimentation and design on embedded microcontrollers, supporting a wide range of architectures including AVR, SAMD, STM32, and ESP32. It provides a signal-centric architecture and a suite of modular abstractions (oscillators, filters, units, and scheduling engines) that simplify the design of time-based behaviors. Its expressive syntax allows fast prototyping with multiple sensors, actuators, and real-time processes, enabling researchers and creative practitioners to experiment and design with complex interactive sytems.

Beyond its technical contributions, *Plaquette* serves as a bridge between scientific research and creative practice. Its applications to interdisciplinary research-creation [Loveless2019-How] projects involving affective biofeedback and robotic behaviors demonstrate the framework’s flexible and robust infrastructure in support of creativity and experimentation in interactive media.

# Statement of Need

*Plaquette* is targeted for research and creation in interactive media, digital lutherie, affective computing, and robotic art. These fields depend on microcontrollers to respond in real time to signal-based events. The open-source platform [Arduino](https://arduino.cc) is the one of the most popular tools used in these fields. Arduino has made microcontroller programming widely accessible to artists, makers, and researchers. Yet, its API is limited: it is not object-oriented, offers few abstractions for handling concurrent processes, and forces users to manipulate raw numerical values rather than focus on creative intentions.

In contrast, dataflow softwares popular within creative and scientific communities working with real-time interactive media (e.g., Max, Pure Data, TouchDesigner) provide powerful models for composing with signals, but are computationally heavy and usually cannot run on constrained hardware. Similarly, scientific tools such as Python’s NumPy/SciPy, Matlab, or R, while offering rich signal analysis tools, are not designed for real-time signal processing on embedded devices.

*Plaquette* addresses these gaps by bringing the expressive power of dataflow signal-based programming into a lightweight, efficient, object-oriented C++ framework optimized for microcontrollers (i.e., requiring minimal CPU and memory usage). It enables intuitive handling of multiple signal and provides efficient implementations of core signal processing functions such as peak detection, normalization, scaling, and smoothing (low-pass). This design enables researchers in art and science, as well as creative practitioners, to focus on experimentation and expressivity while ensuring accurate and reliable real-time performance on resource-constrained platforms.

The framework constitutes strong foundation for workshop-based and participatory research-creation projects, where participants often have diverse levels of technical experience. Its accessibility ensures that beginners can quickly grasp and apply core concepts, while its efficient, expressive, and extensible architecture support the needs of intermediate and advanced users. This combination makes it particularly well-suited for collaborative prototyping in media arts, design, and human-computer interaction, where embodied and situated practices demand adaptable tools.

The framework has already supported a number of public research projects. It was used to improve real-time physiological signal processing as part of the [BioData](https://github.com/eringee/BioData) library for affective biofeedback, supporting creative applications in music and performance [Gee2023-BioSynth], and used in studies of electrodermal activity measurement [Hagler2022-Flexible]. It was integrated at the core of the [MisBKit](https://misbkit.ensadlab.fr), a robotic kit enabling researchers and practitionners to explore object behaviors from artistic and scientific perspectives [Bianchini2015-Misbehavioral]. It was also employed for signal processing and LED-based robotic expression in the artwork *Morphosis*, an installation featuring three spheroid robots that learn in real-time using reinforcement learning [Audry2020-Behaviour,Audry2024-Choreomata]. These examples illustrate the framework’s role not only as a technical tool but also as a catalyst for interdisciplinary exploration.

# Functionality and Design Overview

The core of *Plaquette* is organized around two complementary abstractions called *units* and *engines* that provide a coherent structure for building complex, real-time interactive systems on microcontrollers. *Units* are modular building blocks that encapsulate behaviors such as sensing, generating, filtering, or actuating. *Engines* operate as conductors, managing initialization and timing of units so that they execute consistently without blocking or interruptions.

All units implement a simple, unified interface consisting of a single input and a single output function. This design makes it possible to chain units together in a dataflow-like manner using a special piping operator (``>>``), where the output of one unit is sent as input to another unit. This signal-centric approach allows developers to think in terms of flows of information rather than low-level procedural code, similar to how artists and researchers work with dataflow software, but here optimized for resource-constrained hardware.

The framework includes a variety of unit types:
- **Base units**: basic analog and binary input/output.
- **Generators**: generative source signals such as square, triangle, and sine waves, as well as ramps.
- **Timing units**: scheduling and temporal control such as timers and metronomes.
- **Filters**: real-time signal transformations such as min-max scaling, normalizing, and detecting peaks.
- **Fields**: spatial functions sampled at fractional positions to plot, shape, or transform signals across space.

Engines and units have a low memory footprint, with static allocation at compile time to avoid dynamic allocation and fragmentation. In particular, signal-processing units such as min-max scaling and normalizing use exponential moving averages rather than circular buffers, ensuring low and predictable memory usage.

# Examples

The following program chains an analog input through a min-max scaling filter to control an LED.

```cpp
  #include <Plaquette.h>

  AnalogIn input{A0};         // analog input on pin A0
  AnalogOut led{9};           // PWM-controlled LED on pin 9
  MinMaxScaler scaler{};      // min-max scaler

  void begin() {}

  void step() {
    inputs >> scaler >> led;  // rescale to full range [0, 1] and send to LED
  }
```

This program reacts to peaks in the incoming signal by triggering a sudden movement (ramp) in a servo motor.
The peak detector reacts to outliers after signal normalization. The normalization is calibrated over a
sliding time window, adapting to slow changes in the input.

```cpp
  #include <Plaquette.h>

  AnalogIn input{A0};          // analog input on pin A0
  ServoOut servo{9};           // servomotor connected on pin 9
  Normalizer normalizer{0, 1}; // re-normalizes to N(0, 1)
  PeakDetector peak{1.5};      // triggers when detecting outliers at 1.5 stddev
  Ramp ramp{2.0};              // ramp with duration of 2 seconds (by default: ramps from 0 to 1)

  void begin() {
    normalizer.timeWindow(60); // calibration sliding time window of 60 seconds
  }

  void step() {
    inputs >> normalizer >> peak; // process input signal
    if (peak) ramp.start();       // on peak detection: restart ramp
    ramp >> servo;                // send ramp value to servo motor
  }
```

# Acknowledgements

This work was partially supported by the Natural Sciences and Engineering Research Council of Canada, the Social Sciences and Humanities Research Council of Canada, the Fonds de Recherche du Québec — Société et Culture, and the Canada Council for the Arts. We thank our collaborators and colleagues for supporting the project, in particular Erin Gee, Luana Belinsky, and Chris Salter.
