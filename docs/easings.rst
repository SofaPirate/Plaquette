.. include:: defs.hrst

Easings
=======

Easing functions apply non-linear effects to changing values, in order to create
expressive real-time outputs. Plaquette provides users with a wide range of such
functions, typically used with a Ramp unit.

All easing functions have the same signature:

.. code-block:: c++

   float easeFunction(float t)

Value ``x`` should be in range [0, 1] and returned value is also in [0, 1].

This is the list of all easing functions (source: http://easings.net):

.. image:: images/easings.png

|SeeAlso|
---------
- :doc:`Ramp`
