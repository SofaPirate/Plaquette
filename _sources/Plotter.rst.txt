.. include:: defs.hrst

Plotter
=======

An output unit that streams values to a text-based output using a configurable format.
The Plotter is designed for quick signal inspection in tools such as the Arduino Serial
Plotter, while also supporting structured formats (e.g., CSV, JSON) for logging and
external processing.

To create a new Plotter on the default serial port with a specific baudrate:

.. code-block:: c++

   Plotter plotter(baudrate);

Values are sent in clear text and arranged in **rows**. Each call to ``put()`` (usually
via the ``>>`` operator) appends one value to the current row. The row is then finalized
automatically at the end of the Plaquette engine step (or when the Plotter decides to
close the row).

.. code-block:: c++

   void step() {
     value1 >> plotter;
     value2 >> plotter;
     ...
     valueN >> plotter;
   }

The Plotter can optionally take a comma-separated list of **labels** (``const char*``)
such as ``"wave,signal"``. When labels are provided, some formats will include a header
(e.g., CSV) or will use key/value rendering (e.g., JSON objects, ``"label":value``).

.. code-block:: c++

   Plotter plotter(baudrate, "label1,label2,...,labelN");

.. tip::

  On Arduino, you can visualize the data using the `Serial Plotter <https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-serial-plotter/>`__
  by selecting **Tools > Serial Plotter**.

.. caution::

  Avoid using a ``Monitor`` unit or calling ``Serial`` functions while using a ``Plotter``, as
  it risks breaking the output syntax of the plotter.

|Example|
---------

Streaming multiple values per row.

.. code-block:: c++

   #include <Plaquette.h>

   Plotter plotter(115200); // baudrate = 115200

   Wave wave(SINE);

   AnalogIn signal(A0);

   void step() {
     wave   >> plotter;
     signal >> plotter;
   }

To add labels to the output:

.. code-block:: c++

  Plotter plotter(115200, "wave,signal");

You can also change the format by using presets (``PLOTTER_CSV``, ``PLOTTER_JSON``):

.. code-block:: c++

  void begin() {
    plotter.format(PLOTTER_CSV);
  }

Depending on the chosen format and whether labels are provided, labels may be rendered as a
header (CSV), as keys (JSON objects), or as ``label:value`` pairs (default space-delimited mode with
labels).

|Reference|
-----------

.. doxygenclass:: Plotter
   :project: Plaquette
   :members:

|SeeAlso|
---------

- :doc:`Monitor`
- `Arduino Serial Plotter <https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-serial-plotter/>`_
- `Arduino serial <https://www.arduino.cc/reference/en/language/functions/communication/serial/>`_

.. - :doc:`StreamIn`
