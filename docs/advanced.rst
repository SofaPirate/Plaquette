.. include:: defs.hrst

Advanced Usage
==============

Avoiding Plaquette Style
------------------------

If you don't want to use Plaquette's :doc:`>> <pipe>` operator or auto-conversion 
of units to values (eg., ``if (input)``, ``input >> output``) in favor of a more
typical object-oriented programming style, you can avoid using these features by 
simply using Plaquette units' ``get()`` and ``put()`` methods.

The ``get()`` method returns the current value of the unit:

.. code:: cpp

  float get()

The ``put()`` method sends a value to the unit and then returns the current value
of the unit (the same that would be returned by ``get()``):

.. code:: cpp

  float put(float value)

Additionally, digital input units such as :doc:`DigitalIn`, :doc:`Metronome` 
have a ``boolean isOn()`` method that works for boolean ``true/false`` values,
while digital output units such as :doc:`DigitalOut` have a ``boolean putOn(boolean value)`` 
method.

Here are some examples of how to adopt a classic object-oriented functions style
instead of the Plaquette style.

.. list-table::
  :widths: 50 50
  :header-rows: 1

  * - Plaquette Style
    - Object-Oriented Style
  * - ``input >> output;``
    - ``output.put(input.get());``
  * - ``digitalInput >> digitalOutput;``
    - ``digitalOutput.putOn(digitalInput.isOn());``
  * - ``(2 * input) >> output;``
    - ``output.put(2 * input.get());``
  * - ``!digitalInput >> digitalOutput;``
    - ``digitalOutput.putOn(!digitalInput.isOn());``
  * - ``if (digitalInput)``
    - ``if (digitalInput.isOn())``
  * - ``if (input < 0.4)``
    - ``if (input.get() < 0.4)``
  * - ``input >> filter >> output;``
    - ``output.put(filter.put(input.get()));``

Using Plaquette as an External Library
--------------------------------------

Seasoned Arduino coders might want to avoid rewriting their code using Plaquette's
builtin ``begin()`` and ``step()`` functions, or they may want to include Plaquette's
self-updating loop in a timer interrupt function. It is possible to do so by
including the file ``PlaquetteLib.h`` instead of ``Plaquette.h``.

After this step, you is then responsible for calling ``Plaquette.begin()`` at the beginning of the
``setup()`` function, and also to call ``Plaquette.step()`` at the beginning of the
``loop()`` function, or inside the interrupt.

Here is an example of our blinking code rewritten by using this feature:

.. code:: cpp

    #include <PlaquetteLib.h>

    using namespace pq;

    DigitalOut myLed(13);

    SquareWave myWave(2.0, 0.5);

    void setup() {
      Plaquette.begin();
    }

    void loop() {
      Plaquette.step();
      myWave >> myLed;
    }
