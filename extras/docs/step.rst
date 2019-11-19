.. include:: defs.hrst

step()
======

After creating a ``begin()`` function, which initializes and sets the initial
values, the ``step()`` function does precisely what its name suggests, and performs
one processing step that loops indefinitely as fast as possible, allowing your
program to change and respond. Use it to actively control the board.

.. note::
  Function ``step()`` is the Plaquette equivalent of Arduino's
  `loop() <https://www.arduino.cc/reference/en/language/functions/loop/>`_.
  However, it is highly recommended that this function executes as fast as
  possible. Hence, one should performing computationally-intensive processing
  or calling blocking functions such as ``delay()``

|Example|
---------

.. code-block:: c++

  #include <Plaquette.h>

  DigitalIn button(2);

  DigitalOut led(13);

  void begin() {
  }

  void step() {
    button >> led;
  }

|Reference|
-----------

.. doxygenfunction:: step()
   :project: Plaquette

|SeeAlso|
---------
- :doc:`begin`
