.. include:: defs.hrst

. (dot)
=======

Provides access to an object's methods and data. An object is one instance of a class
and may contain both methods (object functions) and data (object variables and constants),
as specified in the class definition. The dot operator directs the program to the
information encapsulated within an object.

|Example|
---------

Switches LED on every 4 seconds.

.. code-block:: c++

   #include <Plaquette.h>

   DigitalOut led(13);

   void begin() {
     led.off();
   }

   void step() {
     if (round(seconds()) % 4 == 0)
       led.on();
    else
       led.off();
   }

|Syntax|
--------

.. code-block:: c++

   object.method()
   object.variable
