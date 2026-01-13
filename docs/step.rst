.. include:: defs.hrst

step()
======

The ``step()`` function does precisely what its name suggests, and performs
one processing step that loops indefinitely as fast as possible, allowing your
program to change and respond. Use it to actively control the board.

.. hint::
  Function ``step()`` is the Plaquette equivalent of Arduino's
  `loop() <https://www.arduino.cc/reference/en/language/functions/loop/>`_.

.. important::
  It is highly recommended that this function executes as fast as
  possible. Hence, one should performing computationally-intensive processing
  or calling blocking functions such as ``delay()``

.. tip::
  In Plaquette, function ``step()`` is **optional**: only declare it if you need it...
  (you will, most of the time!)

|Example|
---------

.. code-block:: c++

  #include <Plaquette.h>

  DigitalIn button(2);

  DigitalOut led(13);

  void step() {
    button >> led;
  }

|SeeAlso|
---------
- :doc:`begin`
