.. include:: defs.hrst

Plotter
=======

An output unit that streams values to a text-based output using a configurable format.
The Plotter is designed for quick signal inspection in tools such as the Arduino Serial
Plotter, while also supporting structured formats (e.g., CSV, JSON) for logging and
external processing.

Values are sent in clear text and arranged in **rows**. Each call to ``put()`` (usually
via the ``>>`` operator) appends one value to the current row; the row is then finalized
automatically at the end of the Plaquette engine step (or when the Plotter decides to
close the row).

The Plotter can optionally take a comma-separated list of **labels** (``const char*``)
such as ``"wave,signal"``. When labels are provided, some formats will include a header
(e.g., CSV) or will use key/value rendering (e.g., JSON objects, ``"label":value``).

|Example|
---------

Streaming multiple values per row, with labels.

.. tip::

  On Arduino, you can visualize the data using the `Serial Plotter <https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-serial-plotter/>`__
  by selecting **Tools > Serial Plotter**.

.. code-block:: c++

   #include <Plaquette.h>

   // Labels are a single comma-separated string.
   Plotter plotter(115200);

   Wave wave(SINE);
   Signal signal;

   void step() {
     wave >> plotter;
     signal >> plotter;
   }

You can add labels to the output by specifying them at the creation of the ``Plotter`` unit:

.. code-block:: c++

  Plotter plotter(115200, "wave,signal");

You can also output in Comma Separated Values format by using presets
(``PLOTTER_CSV``, ``PLOTTER_JSON``):

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
.. - :doc:`StreamIn`
- `Arduino Serial Plotter <https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-serial-plotter/>`_
- `Arduino serial <https://www.arduino.cc/reference/en/language/functions/communication/serial/>`_
