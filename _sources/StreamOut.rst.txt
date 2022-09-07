.. include:: defs.hrst

StreamOut
=========

An output unit that transmits values through a stream -- for example, the `Arduino serial line <https://www.arduino.cc/reference/en/language/functions/communication/serial/>`_. Values are sent in clear text and separated by newlines and/or carriage returns.

|Example|
---------

Outputs the number of seconds to serial.

.. code-block:: c++

   #include <Plaquette.h>

   StreamOut serialOut(Serial);

   void begin() {}

   void step() {
     // Output the number of seconds
     seconds() >> serialOut;
   }

To run this example:

1. Upload the code.
2. In the Arduino software open the serial monitor: **Tools > Serial Monitor**.
3. Make sure the default baudrate of **9600** bps is selected.
4. You should see the seconds increase.
5. Close the monitor and open serial plotter: **Tools > Serial Plotter**.
6. You should see a graphical representation of the seconds.
7. Replace the line in ``step()`` by: ``sin(seconds()) >> serialOut`` and upload. You should now see a sine wave signal in the serial plotter.

|Reference|
-----------

.. doxygenclass:: StreamOut
   :project: Plaquette
   :members: StreamOut, get, put, precision

|SeeAlso|
---------
- :doc:`AnalogOut`
- :doc:`DigitalOut`
- :doc:`StreamIn`
- `Arduino serial <https://www.arduino.cc/reference/en/language/functions/communication/serial/>`_
- `Arduino streams <https://www.arduino.cc/reference/en/language/functions/communication/stream/>`_
