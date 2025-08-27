.. include:: defs.hrst

PivotField
==========

This field unit generates a spatial response curve based on a **pivot point** around which the field transitions happens.
It can output sharp steps, smooth ramps, bumps or notches, across the normalized range [0, 1], making it ideal
for creating animations on arrays of actuators such as LEDs or motors (eg. VU-meters, fades, or envelope-like visualizations).

Modes
-----

The behavior of the field is defined by its mode:

- ``PIVOT_FALLING`` (default): Values are `1.0` from the left up to the pivot, then fall to `0.0`.
- ``PIVOT_RISING`` : Values are `0.0` on the left and rise to `1.0` after the pivot.
- ``PIVOT_BUMP``: Peak at the pivot: `1.0` around the pivot, tapering down on both sides.
- ``PIVOT_NOTCH``: Inverse of ``PIVOT_BUMP``: `0.0` around the pivot, `1.0` elsewhere.

Adjustments
-----------

The curve can be finely configured using many different parameters:

- ``rampWidth``: Ramp width as a fraction of total range (e.g., `0.2` for 20% width). Default: `0` (no ramp).
- ``center``: Point from which the curve starts (in [0, 1]). Examples:

   - ``0.0`` (default) Start from left side.
   - ``0.5`` Start from middle going in both directions.
   - ``0.75`` Start from ¾ (75%) going in both directions.
   - ``1.0`` Start from right side.
- ``easing``: Optional :doc:`easing function <easings>` applied to the ramp. Default: ``easeNone``.
- ``rampShift``: Determines the horizontal shift of the ramp. Examples:

   - ``0.0`` Ramp ends at the pivot point.
   - ``0.5`` (default) Ramp is right in the middle of the pivot point.
   - ``1.0`` Ramp starts at the pivot point.
- ``bumpWidth``: When choosing ``PIVOT_BUMP`` or ``PIVOT_NOTCH``, determines the width of the peak. Default: `0.25` (25%).


|Example|
--------

This example uses a single PWM analog LED and two potentiometers to show the behavior
of a PivotField. The first potentiometer is used to control the pivot point, then the second
potentiometer is used to reveal the values of the `PivotField` from 0 to 1.

.. code-block:: cpp

  #include <Plaquette.h>

  // Potentiometer #1 to control the pivot point.
  AnalogIn pot(A0);

  // Potentiometer #2 for exploring the field.
  AnalogIn slider(A1);

  // The PWM LED.
  AnalogOut led(9);

  // The pivot field.
  PivotField field;

  void begin() {
    // Parameters. Change at will to explore different configurations.
    field.rampWidth(0.2);
    field.easing(easeInSine);
    field.mode(PIVOT_FALLING); // default
    field.center(0); // default
    field.bumpWidth(0.25); // default
  }

  void step() {
    // Send potientiometer value to the pivot field.
    pot >> field;

    // Send field value at position of slider to LED.
    field.at(slider) >> led;
  }


|Reference|
----------

.. doxygenclass:: PivotField
   :members:
   :undoc-members:

|SeeAlso|
--------

- :doc:`TimeSliceField`
- :doc:`easings`
- :doc:`Ramp`
