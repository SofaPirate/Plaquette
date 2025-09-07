---
title: "Plaquette: An Object-Oriented Framework for Real-Time Signal Processing on Microcontrollers"
tags:
  - C++
  - Arduino
  - Embedded Systems
  - Creative Coding
  - Interactive Media
  - Research-Creation
authors:
  - name: Sofian Audry
    affiliation: 1
    orcid: 0000-0002-6340-2550
  - name: Thomas Ouellet Fredericks
    affiliation: 2
affiliations:
  - name: Université du Québec à Montréal (UQAM), École des médias
    index: 1
  - name: Collège Montmorency, Laval, QC
    index: 2
date: 2025-09-07
bibliography: paper.bib
---

# Summary

*Plaquette* is an open-source, object-oriented C++ framework for developing interactive applications on
embedded microcontrollers, supporting a wide range of architectures including AVR (Arduino Uno), SAM and SAMD
(Arduino Due, MKR series), STM32, and ESP32. It provides a signal-centric architecture and a suite of
modular abstractions (oscillators, filters, units, and scheduling engines) that simplify the design of
time-based behaviors in interactive systems. By lowering the barrier to prototyping with sensors, actuators,
and real-time processes, *Plaquette* enables researchers and artists to design complex interactive environments that are both
reproducible and extensible.

Beyond its technical contributions, *Plaquette* serves as a bridge between science, engineering, and
artistic practice. It has been adopted in diverse research-creation contexts, including affective
biofeedback and robotic expression through artistic demonstrations and participatory research. Its applications demonstrate the
framework’s potential to advance embodied, interdisciplinary approaches to interactive media while
also providing a reproducible infrastructure for scholarly inquiry.

# Statement of Need

Research in interactive media, human–computer interaction (HCI), sound and music computing, and
research-creation increasingly depends on microcontrollers to sense, process, and actuate in real
time. However, programming microcontrollers directly through environments such as Arduino is
challenging: it lacks an integrated object-oriented framework, offers limited abstractions for
managing concurrent processes, and makes timing-sensitive operations cumbersome. In parallel,
dataflow environments popular in creative communities (e.g., Max/MSP, Pure Data, TouchDesigner)
provide powerful models for composing signals and behaviors, but they are computationally heavy and
cannot run on constrained hardware. Finally, scientific tools such as Python’s NumPy/SciPy or R, while
allow rich offline data analysis tools, are not designed for real-time signal processing on embedded devices.

*Plaquette* addresses this gap by bringing the expressive power of dataflow-style signal-based
programming into a lightweight, efficient, object-oriented C++ framework optimized for
microcontrollers. It enables intuitive handling of multiple simultaneous processes and provides
real-time implementations of core signal processing functions such as peak detection, normalization, and
scaling, using efficient moving averages parameterized in intuitive units (seconds, hertz). This
design empowers researchers in art, science and engineering, as well as creative practitioners, to focus on
experimentation and artistic expression with tangible computing, while ensuring accurate and reliable real-time performance on
resource-constrained platforms.

From a scientific standpoint, *Plaquette* also provides the groundwork for workshop-based and
participatory research-creation projects, in which researchers and participants from creative communities
collaboratively design and test interactive systems. This methodology is increasingly important in research-creation
contexts in media arts, design, and HCI where embodied interaction and situated practices
require adaptable prototyping tools. By making embedded programming accessible and robust,
*Plaquette* thus supports interdisciplinary collaboration and experimentation in art, engineering, and science.

The framework has already been used to support public research projects, illustrating its
broad applicability:

- **Physiological signal processing**: incorporated in the [BioData](https://github.com/eringee/BioData) Arduino
  library for affective biofeedback, supporting creative applications in music and performance [Gee2023-BioSynth], and used in
  studies of electrodermal activity measurement [Hagler2022-Flexible].
- **Interdisciplinary robotic kits**: integrated at the core of the [MisBKit](https://misbkit.ensadlab.fr),
  a robotic kit that enables researchers and practitionners to explore object behaviors from artistic and scientific
  perspectives.
- **Artistic research**: employed for signal processing and LED-based robotic expression in the
  artwork *Morphosis* that involves three spheroid robots learning in real-time using reinforcement learning
  [Audry2020-Behaviour,Audry2024-Choreomata].

These examples illustrate the framework’s role not only as a technical tool but also as a catalyst
for cross-disciplinary exploration, supporting both reproducible scientific experiments and
open-ended artistic inquiry.

# Functionality and Design Overview

The core of *Plaquette* is organized around two complementary abstractions: the **engines** and the
**units**. Units are modular building blocks that
encapsulate behaviors such as sensing, generating, filtering, or actuating. Together, the engine and
units provide a coherent structure for building complex, real-time interactive systems on
microcontrollers. Engines act as deterministic schedulers, managing initialization and timing of units
so that they execute consistently without blocking or interruptions.

All units implement a simple, unified interface consisting of a single input and a single output
function. This design makes it possible to **chain units together in a dataflow-like manner**, where
the output of one unit becomes the input of another. This signal-centric approach allows developers
to think in terms of flows of information rather than low-level procedural code, similar to how
artists and researchers work with Max/MSP or Pure Data, but here optimized for resource-constrained
hardware.

The framework includes a variety of unit types:

- **Base units**: basic input/output.
- **Generators**: periodic signals such as square, triangle, and sine waves.
- **Timing units**: scheduling and temporal control with timers and ramps.
- **Filters**: real-time signal transformations such as min-max scaling, normalizing, and detecting peaks.
- **Fields**: spatial functions sampled at fractional positions to plot, shape, or transform signals across space.

# Examples

The following program chains an analog input through a min-max scaling filter to control an LED:

```cpp

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

  AnalogIn input{A0};          // analog input on pin A0
  ServoOut servo{9};           // servomotor connected on pin 9
  Normalizer normalizer{0, 1}; // re-normalizes to N(0, 1)
  PeakDetector peak{1.5};      // triggers when detecting outliers at 1.5 stddev
  Ramp ramp{2.0};              // ramp with duration of 2 seconds (by default: ramps from 0 to 1)

  void begin() {
    normalizer.timeWindow(60); // normalizing sliding time window: 60 seconds
  }

  void step() {
    inputs >> normalizer >> peak; // process input signal

    if (peak) ramp.start(); // on peak detection: restart ramp

    ramp >> servo; // send ramp value to servo motor
  }
```


# Acknowledgements

This work was partially supported by the Natural Sciences and Engineering Research Council of Canada, the
Social Sciences and Humanities Research Council of Canada, the Fonds de Recherche du Québec -- Société et Culture,
and the Canada Council for the Arts. We thank our collaborators and colleagues for supporting the project, in
paticular Luana Belinsky, Erin Gee, and Chris Salter.
