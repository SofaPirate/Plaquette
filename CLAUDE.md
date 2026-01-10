# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Plaquette is an object-oriented, signal-centric programming framework for **creative physical computing** on Arduino and compatible microcontrollers. It abstracts real-time signal processing, sensors, actuators, oscillators, and timing into intuitive, artist-friendly APIs while maintaining full Arduino compatibility.

**Key Philosophy:**
- Signal-centric design with flow operators (`>>`) for data routing
- Step-based execution model replacing traditional Arduino `loop()`
- Automatic sensor calibration and signal filtering
- Real-time signal processing without blocking
- Memory-efficient design for resource-constrained microcontrollers

## Build & Test Commands

### Compile Examples

Compile all examples for default boards:
```bash
./compile_examples.sh
```

Compile a specific example:
```bash
./compile_examples.sh -p examples/01.Basics/Blink
# or relative to examples/
./compile_examples.sh -p 01.Basics/Blink
```

Compile for specific board(s):
```bash
./compile_examples.sh -b "teensy:avr:teensy31,arduino:avr:uno"
```

### Run Unit Tests

Run all AUnit tests (uses EpoxyDuino for native Linux/macOS execution):
```bash
cd tests
make clean
make
make test
```

Run a specific test suite:
```bash
cd tests/filters
make
./filters.out
```

Test directories: `arrays`, `castings`, `engines`, `filters`, `functions`, `generators`, `hybridarrays`, `normalizer`, `operations`, `timing`, `values`

### Continuous Integration

Tests run via GitHub Actions on push/PR:
- **AUnit Tests**: `make -C tests && make -C tests test`
- **Arduino Compilation**: Via Arduino CLI for multiple boards
- **Arduino Lint**: Validates library structure

## Architecture

### Core Execution Model

Plaquette replaces Arduino's `setup()`/`loop()` paradigm with `settings()`/`begin()`/`step()`:

```cpp
#include <Plaquette.h>

void settings() {
  // Optional: configure before initialization
}

void begin() {
  // Initialize components (called once)
}

void step() {
  // Called repeatedly (replaces loop())
}
```

The framework automatically handles:
- Unit registration and lifecycle management
- Sample rate control (auto or fixed)
- Time synchronization across all units

### Engine System

**`Engine`** (`PqCore.h`): The central orchestrator managing all Units.
- Singleton `Plaquette` is the primary engine
- Manages unit registration, stepping, and timing
- Provides global time functions: `seconds()`, `milliSeconds()`, `microSeconds()`
- Supports multiple engines for complex scenarios

**Step Execution Flow:**
1. `Plaquette.preBegin()` → calls `begin()` on all units
2. User's `begin()` executes
3. Each loop: `Plaquette.timeStep()` checks timing, then `preStep()` calls `step()` on all units
4. User's `step()` executes

### Class Hierarchy

**Base Classes:**

- **`Flowable`** (`PqCore.h`): Abstract interface for chainable signal flow
  - `float get()`: Read current value
  - `float put(float)`: Write value
  - Operator overloads for flow chaining (`>>`)

- **`Unit`** (`PqCore.h`): Base for all Plaquette components
  - Extends `Flowable`
  - Auto-registers with Engine
  - Virtual `begin()` and `step()` lifecycle hooks
  - Access to engine time/sample rate

- **`AnalogSource`**: Units with values in [0, 1] (sensors, oscillators)
- **`DigitalUnit`**: Boolean units (on/off states)
- **`DigitalSource`**: Digital units with change detection (rise/fall events)

**Specialized Units:**

- **Inputs/Outputs**: `AnalogIn`, `AnalogOut`, `DigitalIn`, `DigitalOut`, `StreamIn`, `StreamOut`
- **Timing**: `Chronometer`, `Alarm`, `Metronome` (extends `AbstractTimer`)
- **Waves**: `SineWave`, `SquareWave`, `TriangleWave`, `Wave` (extends `AbstractWave` → `AbstractOscillator`)
- **Filters**: `Smoother`, `MovingAverage`, `MinMaxScaler`, `Normalizer`, `RobustScaler`
- **Events**: `PeakDetector`, `Ramp`
- **Fields**: `AbstractField`, `PivotField`, `TimeSliceField` (generate arrays from functions)
- **Visualization**: `Plotter`, `Monitor`

### Signal Flow Operators

The **flow operator** (`>>`) chains units for data routing:

```cpp
AnalogIn sensor(A0);
Smoother filtered;
AnalogOut led(LED_BUILTIN);

void step() {
  sensor >> filtered >> led;  // sensor → filter → actuator
}
```

**Type Safety:**
- Compile-time checks prevent invalid chaining (e.g., `value >> nonFlowable`)
- Supports `Flowable` lvalues and rvalues (including `ParameterSlot` for dynamic properties)
- Integral types auto-convert to float

### Value Wrappers

**`Value<T>`** (`PqValue.h`): Typed wrappers for basic types as Flowables
- `Value<float>`, `Value<int32_t>`, `Value<bool>`
- Enables: `sensor >> myValue; myValue >> actuator;`
- C++ value semantics: `myValue++`, `myValue += 5`, `if (myValue > 10)`

### Step-Invariance Principle

Units follow strict timing guarantees:
- `get()` returns constant value throughout a step (unless `put()` called)
- ADC/DAC operations only occur in `step()` (never in `get()`/`put()`)
- `step()` applies changes using last received value or aggregate (e.g., average)

### Event System

Units support event callbacks:

```cpp
DigitalIn button(2);

void begin() {
  button.onRise([]() { Serial.println("Pressed!"); });
}
```

Event types: `EVENT_RISE`, `EVENT_FALL`, `EVENT_CHANGE`

## Coding Conventions (from CONTRIBUTING.md)

### Naming
- **Classes**: `PascalCase` (e.g., `PeakDetector`)
- **Public methods/vars**: `camelCase()` / `camelCase`
- **Private/protected**: `_camelCase()` / `_camelCase`
- **Enums/constants**: `UPPER_SNAKE`
- **Getter/setter**: Same name (e.g., `float period()`, `void period(float)`)

### Style
- **Indentation**: 2 spaces (no tabs)
- **Braces**: Opening on same line, closing on own line
- **Line length**: ≤100 characters
- **Include guards**: `#define FILE_NAME_H_` (not `#pragma once`)

### API Design
- **Artist-friendly**: Use familiar concepts (frequency, bpm, duration) over technical jargon
- **Avoid in public APIs**: Pointers, `::` operator, class enums, deep inheritance
- **Units**: Metric system (seconds, Hz/BPM, degrees, ratios [0.0–1.0])
- **Use references** over pointers in public APIs
- **Default arguments** for common use cases

### Memory Constraints
- Target microcontrollers with limited RAM/CPU
- Avoid heap allocations; prefer stack/pre-allocated buffers
- Use `PQ_OPTIMIZE_FOR_CPU` flag for speed over memory when needed
- Inline functions and const references to avoid copies

### Documentation
- Doxygen-style (`/** ... */` or `///`)
- Document all public APIs with purpose and units
- Use `//` for block comments (end with period)

### License
- All files must include GPLv3+ header
- Credit: Sofian Audry, Thomas O. Fredericks

## File Organization

```
src/                   # Library source (headers + implementations)
  PqCore.h            # Engine, Unit base classes, flow operators
  Plaquette.h         # Main header (redefines setup/loop)
  PlaquetteLib.h      # Library includes
  PlaquetteSketch.h   # Sketch function declarations
  PqValue.h           # Typed value wrappers
  [Units].h/.cpp      # Individual unit implementations
  pq_*.h/.cpp         # Utility functions (math, time, random, etc.)

examples/             # Organized by category (01.Basics, 02.Digital, etc.)
tests/                # AUnit tests (run natively via EpoxyDuino)
docs/                 # Sphinx documentation (reStructuredText)
libraries/            # Bundled dependencies (AUnit, EpoxyDuino, MemoryStream)
```

## Dependencies

- **Servo**: Required dependency (auto-installed by Arduino)
- **AUnit**: Unit testing framework (bundled in `libraries/`)
- **EpoxyDuino**: Arduino emulation for native Linux/macOS testing (bundled)
- **MemoryStream**: In-memory stream for testing (bundled)

## Key Files to Understand

1. **`src/PqCore.h`**: Core architecture (Engine, Unit, Flowable, flow operators)
2. **`src/Plaquette.h`**: Setup/loop redefinition magic
3. **`src/AbstractWave.h/.cpp`**: Oscillator architecture
4. **`src/AbstractTimer.h/.cpp`**: Timing unit base class
5. **`CONTRIBUTING.md`**: Detailed coding standards and conventions
6. **`tests/example/example.ino`**: Minimal test example structure

## Common Patterns

### Adding a New Unit

1. Extend appropriate base (`Unit`, `AnalogSource`, `DigitalSource`, etc.)
2. Implement `begin()` (initialization) and `step()` (per-frame update)
3. Follow step-invariance: cache values in `step()`, return cache in `get()`
4. Add GPLv3 header with copyright
5. Create test in `tests/` directory
6. Add documentation in `docs/` (reStructuredText)

### Using Parameters as Flow Targets

```cpp
SineWave lfo;
SquareWave carrier;

void step() {
  lfo >> carrier.frequency();  // Modulate frequency dynamically
}
```

Units often expose `ParameterSlot` accessors (e.g., `frequency()`) that return chainable temporary objects.

## Important Flags

- `PQ_OPTIMIZE_FOR_CPU=1`: Prioritize speed over memory
- `PLAQUETTE_MAX_UNITS`: Max units per engine (default varies by platform)
- `PLAQUETTE_NO_SMOOTH_WINDOW`: Disable smoothing
- `ARDUINO` / `EPOXY_DUINO`: Platform detection

## Documentation

- **Online docs**: https://plaquette.org
- **Local build**: `cd docs && make html` (requires Sphinx)
- **Manual**: `extras/Plaquette-Manual.pdf`

## Notes for Code Modifications

- Maintain backward compatibility for public APIs
- Test on resource-constrained boards (Arduino Uno/Nano)
- Verify examples compile for multiple architectures
- Update keywords.txt for new public classes/functions
- Keep examples minimal and focused (avoid over-engineering)
- Document units in measurement (seconds, Hz, degrees, [0-1])
