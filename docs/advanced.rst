.. include:: defs.hrst

Advanced Usage
==============

Avoiding Plaquette Style
------------------------

If you don't want to use Plaquette's :doc:`>> <pipe>` operator, or Plaquette's
auto-conversion of units to values, you can avoid these features by simply using
Plaquette's units's ``get()`` and ``put()`` methods. These methods are defined as follow:

The ``get()`` method returns the current value of the unit:

.. code:: cpp

  float get()

The ``put()`` method sends a value to the unit and then returns the current value
of the unit (the same that would be returned by ``get()``):

.. code:: cpp

  float put(float value)

Hence, the following statements are equivalent:

.. list-table::
  :widths: 50 50
  :header-rows: 1

  * - Plaquette Style
    - Object-Oriented Style
  * - ``input >> output``
    - ``output.put(input.get())``
  * - ``(2 * input) >> output``
    - ``output.put(2 * input.get())``
  * - ``if (input)``
    - ``if (input.get())``
  * - ``input >> filter >> output``
    - ``output.put(filter.put(input.get()))``

Using Plaquette as an External Library
--------------------------------------

Seasoned Arduino coders might want to avoid rewriting their code using Plaquette's
builtin ``begin()`` and ``step()`` functions, or they may want to include Plaquette's
self-updating loop in a timer interrupt function. It is possible to do so by
including the file ``PlaquetteLib.h`` instead of ``Plaquette.h``.

One is then responsible for calling ``Plaquette.begin()`` at the beginning of the
``setup()`` function and to call ``Plaquette.step()`` at the beginning of the
``loop()`` function or inside the interrupt.

Here is an example of our blinking code rewritten using this feature:

.. code:: cpp

    #include <PlaquetteLib.h>

    using namespace pq;

    DigitalOut myLed(13);

    SquareOsc myOsc(2.0, 0.5);

    void setup() {
      Plaquette.begin();
    }

    void loop() {
      Plaquette.step();
      myOsc >> myLed;
    }
