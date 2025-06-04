# Plaquette Coding Conventions

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

| Element         | Convention     | Example             |
|-----------------|----------------|---------------------|
| Class names     | PascalCase     | `PeakDetector`      |
| Method names (public)    | camelCase()      | `reloadThreshold()` |
| Variable names (public)  | camelCase      | `value`             |
| Method names (private)   | _camelCase()     | `_read()` |
| Variable names (private or protected) | _camelCase     | `_peakValue`|
| Enum values     | UPPER_SNAKE    | `PEAK_MIN`          |
| Constants/Macros| UPPER_SNAKE    | `PLAQUETTE_DEFAULT_SMOOTH_WINDOW` |
| Singletons      | PascalCase     | `Plaquette`         |

---

## Comments and Documentation

- Use `//` for brief, inline comments.
- Use `/** ... */` or `///` for block or Doxygen-style comments.
- All public API elements should be documented with purpose and units (if applicable).

---

## API Design Principles

### Artist-Friendly Interfaces

- APIs should reflect **concepts familiar to digital artists** (e.g., `frequency()`, `bpm()`, `duration()`, `speed()`).
- Provide high-level metaphors (e.g., `triggerThreshold()`, `put()`, `isOn()`) instead of low-level control structures.

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
- Prefer traditional `enum` unless `enum class` adds clear value.
- Values must be `UPPER_SNAKE_CASE`.

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
#define NORMALIZER_NO_CLAMP 0
#define INFINITE_WINDOW (-1)
```

---

## Memory and Performance

- Code must run efficiently on microcontrollers with limited RAM and CPU.
- **Avoid heap allocations** unless justified and controlled.
- Favor:
  - **Stack allocation**
  - **Pre-allocated buffers**
  - **Fixed-size containers**
- Avoid STL containers unless proven lightweight and portable.
- Use **inline functions** and `const` references to avoid copies.

```cpp
inline float triggerThreshold() const { return _triggerThreshold; }
```

---

## Classes and Inheritance

- Use **composition** over inheritance where appropriate.
- Use `virtual` destructors in base classes.
- Use public inheritance only when there is a clear "is-a" relationship.
- Avoid deep inheritance hierarchies.

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

*Last updated: 2025-05-19*