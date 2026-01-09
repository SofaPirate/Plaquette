.. include:: defs.hrst

Monitor
=======

An output unit that writes textual information to a print device, typically the serial line.
The Monitor is intended for **logging, debugging, and human-readable output**, rather than
structured data plotting like the :doc:`Plotter`.

To create a monitor on the default serial port with a specific baudrate:

.. code-block:: c++

   Monitor monitor(baudrate);

Plaquette automatically uses the first Monitor you create as the default device where information
is printed. This means that instead of writing:

.. code-block:: c++

  monitor.println("Hello world!");

You can just write:

.. code-block:: c++

  println("Hello world!");

.. tip::

  On Arduino, you can read the text printed by the Monitor unit using the
  `Serial Monitor <https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-serial-monitor/>`__
  by selecting **Tools > Serial Monitor**. Make sure the baudrate
  matches the value used to create the Monitor.

|Example|
---------

Printing messages and values to the serial monitor.

.. code-block:: c++

   #include <Plaquette.h>

   Monitor monitor(115200);

   void step() {
     println("Stepping...");

     print("Seconds: ");
     seconds >> monitor;
     println(); // new line
   }

|Notes|
-------

- The Monitor is **not** intended for structured data export or plotting; use :doc:`Plotter` for that purpose.
- Blocking read operations (such as ``readStringUntil()``) are intentionally not exposed to avoid
  unintended blocking behavior.
- Precision for numeric output can be configured on the Monitor by calling its function ``precision()`` and
  applies to floating-point values printed through it.

|Reference|
-----------

.. doxygenclass:: Monitor
   :project: Plaquette
   :members:

|SeeAlso|
---------

- :doc:`Plotter`
.. - :doc:`StreamIn`
- `Arduino Serial Monitor <https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-serial-monitor/>`__
- `Arduino serial <https://www.arduino.cc/reference/en/language/functions/communication/serial/>`_
