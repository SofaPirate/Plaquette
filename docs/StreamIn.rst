.. include:: defs.hrst

StreamIn
========

An input unit that can receive values transmitted through a stream -- for example, the `Arduino serial line <https://www.arduino.cc/reference/en/language/functions/communication/serial/>`_. Values are sent in clear text and separated by newlines and/or carriage returns.

|Example|
---------

Controls the value of a LED using serial. Try opening the serial monitor and
sending values between 0 and 1.

.. code-block:: c++

   #include <Plaquette.h>

   StreamIn serialIn(Serial);

   AnalogOut led(9);

   void begin() {}

   void step() {
     serialIn >> led;
   }

To run this example:

1. Upload the code.
2. In the Arduino software open the serial monitor: **Tools > Serial Monitor**.
3. Make sure the default baudrate of **9600** bps is selected.
4. Make sure one of the options "Newline", "Carriage return", or "Both NL + CR" is selected.
5. Write a number between 0.0 and 1.0 and press "Enter". This should allow you to set the LED intensity.
6. Try different values.

|Reference|
-----------

.. doxygenclass:: StreamIn
   :project: Plaquette
   :members: StreamIn, get

|SeeAlso|
---------
- :doc:`AnalogIn`
- :doc:`DigitalIn`
- :doc:`StreamOut`
- `Arduino serial <https://www.arduino.cc/reference/en/language/functions/communication/serial/>`_
- `Arduino streams <https://www.arduino.cc/reference/en/language/functions/communication/stream/>`_
