Advanced Usage
==============

Some programmers might want to avoid rewriting their code using Plaquette's
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
