# Contributing to Plaquette

Plaquette is an open-source software library that brings together artists, makers, researchers, and
developers interested in creative computation. This document explains both **how to contribute** to the
project and **how to write code** that follows Plaquette’s conventions.

Whether you are fixing a typo, reporting a bug, improving documentation, or implementing a new feature,
thank you for helping make Plaquette better.

---

## How to Contribute

### 1. Reporting Bugs

Before filing a new issue:
- Search the [existing issues](https://github.com/SofaPirate/Plaquette/issues) to see if the problem has
  already been reported or discussed.
- If not, open a **Bug Report** issue using the provided template.
- Include steps to reproduce, system details, and any relevant logs or screenshots.

Bug reports are most useful when they are **clear, reproducible, and focused** on a specific problem.

---

### 2. Requesting Features

If you have an idea that could improve Plaquette:
- Check whether a similar suggestion already exists in the issue tracker.
- If not, open a **Feature Request** using the appropriate template.
- Explain what problem your idea solves, who benefits, and (optionally) how it could be implemented.

For larger or experimental ideas, open an issue first to discuss feasibility and design.

---

### 3. Seeking Support

If you encounter trouble using Plaquette or its examples:
- Open a **Support Question** issue on GitHub.
- Describe what you tried, what you expected, and what happened.
- Include your operating system, hardware, and relevant code snippets.

Support questions help us identify documentation gaps and improve usability.

---

### 4. Making Code Contributions

We welcome pull requests for bug fixes, documentation, and new features.

Please:
1. Follow the coding and formatting conventions below.
2. Keep pull requests focused — one topic per PR.
3. Write clear commit messages describing your change.
4. Include tests when appropriate.

For **major changes** (new features or refactors), open an issue first to discuss your idea.
For **small fixes** or documentation tweaks, you may submit a PR directly.

---

### 5. Communication and Collaboration

All interactions on the Plaquette GitHub repository are governed by the
[Code of Conduct](https://github.com/SofaPirate/Plaquette/blob/main/CODE_OF_CONDUCT.md).

Please:
- Be respectful and constructive in discussions and reviews.
- Assume good intent and ask clarifying questions before judging others’ work.
- Credit and thank contributors whenever you build upon their ideas.

Our goal is to make contributing to Plaquette a positive, creative, and collaborative experience.

---

# Coding Conventions

## Overview

These conventions define the coding style and design principles of a Plaquette, an object-oriented, user-friendly, signal-centric programming framework for creative physical computing.

---

## General Principles

- **Clarity and minimalism**: Code should prioritize readability over cleverness.
- **Consistency**: Coding style must be unified across modules and header/source files.
- **Efficiency**: Code should be computationally and memory efficient to run on microcontrollers with limited RAM and CPU.
- **Simplicity for artists**: APIs must be intuitive for digital artists and creative coders.
- **Portability**: Favor platform-agnostic C++ when possible.

---

## File and Project Structure

- `.h` header files define interfaces, constants, and inline utilities.
- `.cpp` source files contain implementation details.
- `.ino` files provide tests using AUnit.
- Include guards must protect all header files.
  - ✅ Use the following style for include guards: `#define FILE_NAME_H_` eg. `#define MIN_MAX_SCALER_H_`.
  - ❌ Avoid using `#pragma once` for better compability and robustness.

---

## Formatting Rules

- **Indentation**: 2 spaces (no tabs).
- **Braces**:
  - Opening braces on the same line.
  - Closing braces on their own line.
- **Line length**: Ideally 100 characters or fewer.
- **Spacing**:
  - Space after keywords (`if (...)`, `while (...)`)
  - No space before function call parenthesis: `step()` not `step ()`

---

## Naming Conventions

| Element                  | Convention     | Example             |
|--------------------------|----------------|---------------------|
| Class names              | PascalCase     | `PeakDetector`      |
| Method names (public)    | camelCase()    | `reloadThreshold()` |
| Variable names (public)  | camelCase      | `value`             |
| Method names (private or protected)    | _camelCase()  | `_read()`    |
| Variable names (private or protected)  | _camelCase    | `_peakValue` |
| Enum values              | UPPER_SNAKE    | `PEAK_MIN`          |
| Constants/Macros         | UPPER_SNAKE    | `PLAQUETTE_DEFAULT_SMOOTH_WINDOW` |
| Singletons               | PascalCase     | `Plaquette`         |

- Accessor/getter and mutator/setter functions for class properties should have the same name (eg. `float period()` and `void period(float)`).
- Avoid abbreviations (eg. `frequency` instead of `freq`).
- Avoid acronyms unless they are extermely clear within the domain of real-time signal processing (eg. BPM, OSC).

---

## Comments and Documentation

- Use `/** ... */` or `///` for Doxygen-style comments.
- Use `//` above blocks of code with human-readable sentences ending with a period (`.`)
- Use trailing comments for short clarifications about a line of code.
- All public API elements should be documented with purpose and units (if applicable).

```cpp
/**
 * Sets the frequency (in Hz).
 * @param frequeny the frequency of oscillation (in Hz)
 */
void frequency(float frequency) {
  // Assign frequency.
  frequency = max(frequency, 0.0f); // Ensure frequency >= 0.
  _period = 1.0f / abs(frequency);
}
```

---

## API Design Principles

### Artist-Friendly Interfaces

#### Things to Do

Public APIs should:
- ✅ Reflect **concepts familiar to digital artists** (e.g., `frequency()`, `bpm()`, `duration()`, `speed()`).
- ✅ Provide **high-level metaphors** (e.g., `mapTo()`, `triggerThreshold()`, `isOn()`) instead of low-level control structures.
- ✅ Encourage **default arguments** for common use (eg. `DigitalIn(pin)` default to `DIRECT` mode).

#### Things to Avoid

Public APIs should **avoid**:
- ❌ **API leakage** of implementation details like engine internals, buffer pointers, or state flags.
- ❌ **Pointers** (`*ptr`, `&object`) (with the exception of arrays or chains of characters); use references instead.
- ❌ **Scope-resolution operator** (`::`) such as static members of classes and class enums.
- ❌ **Overuse of inheritance** in favor of composition or plug-and-play interfaces.
- ❌ **Ambiguous overloads** with minimal type differences (e.g., `set(float)` vs `set(double)`).

### Measurement Units

The APIs should make use of the metric system. The following table lists the default units to use for each case.

| Concept            | Unit        | Context of Application                              |
|--------------------|-------------|-----------------------------------------------------|
| Ratio              | 0.0–1.0 (%) | Normalized range for analog values and properties expressible as a ratio such as oscilllator width or phase. |
| Time               | seconds     | Durations, time windows, periods, etc.              |
| Frequency          | Hz, BPM     | Cycles per second and per minute.                   |
| Angle              | degrees     | Rotations.                                          |

### Pointers vs References

- **Public APIs** should use **references** where appropriate.
- **Pointers** may be used internally, especially for optional components or ownership handling.
- This enforces safety and reduces boilerplate at call sites.

```cpp
AnalogIn(int pin, Engine& engine);
```

---

## Enums and Defines

### Enums

- Used for mode/state selection and event classification.
- Enum values must be `UPPER_SNAKE_CASE`.
- ❌ Avoid using class member enums or enum classes that would require the use of the `::` operator.

```cpp
enum {
  PEAK_RISING,
  PEAK_FALLING,
  PEAK_MAX,
  PEAK_MIN
};
```

### Defines

- Use `#define` only for:
  - Feature flags
  - Constants required at preprocessor level
- Prefer `const`, `constexpr`, or `enum` for typed values.

```cpp
#define PLAQUETTE_NO_SMOOTH_WINDOW 0.0f
#define INFINITE_WINDOW (-1)
constexpr float INV_FIXED_MAX = 1.0f / FIXED_MAX;
```

---

## Memory and Performance

- Code must run efficiently on microcontrollers with limited RAM and CPU.
- Favor memory over speed by default.
- Use flag `PQ_OPTIMIZE_FOR_CPU` to provide CPU optimization that might use more memory.
- Use **inline functions** and `const` references to avoid copies.
- ❌ **Avoid heap allocations** unless justified and controlled.
- ✅ Favor
  - **Stack allocation**
  - **Pre-allocated buffers**
  - **Fixed-size containers**

---

## Classes and Inheritance

- Use **composition** over inheritance where appropriate.
- Use `virtual` destructors in base classes.
- Use public inheritance only when there is a clear "is-a" relationship.
- ❌ **Avoid deep inheritance** hierarchies.

---

## Step-Invariance

Units in Plaquette should adhere to the following principles to ensure
consistency and predictability during each execution step:

- The value returned by a unit’s `get()` function **must remain constant
throughout a step**, unless explicitly changed via the unit’s `put()` function.
- After a unit is updated via `put()`, the unit's `step()` function should
apply the change using either the **last received value** or a suitable
**aggregate** of values received during that step (eg. average).
- **Transductions** (such as ADC reads or DAC writes) must **only occur
inside the `step()` function**, which is automatically called by the engine
once per step. This ensures consistent timing and avoids unintended side effects.

---

## Licensing and Attribution

- All files must include a **GPLv3+ license header**.
- Credit original authors (e.g., Sofian Audry, Thomas O. Fredericks).
- Add copyright year.

---

## Visual Identity

- ❌ **Use only with approbation by the core developers**.
- Color palette: #00ACFC (cyan-blue) and #FFFFFF (white)
- Logo font: (OCR A Bold)[https://fonts.adobe.com/fonts/ocr-a]

### Logo (no text)

![Plaquette Logo (no text)](https://github.com/SofaPirate/Plaquette/blob/master/docs/images/Plaquette-Logo.png)

### Logo (with text)

![Plaquette Logo (with text)](https://github.com/SofaPirate/Plaquette/blob/master/docs/images/Plaquette-LogoText.png)

---

*Last updated: 2025-06-16*
