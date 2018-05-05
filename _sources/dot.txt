.. include:: defs.hrst

. (dot)
=======

Provides access to an object's methods and data. An object is one instance of a class
and may contain both methods (object functions) and data (object variables and constants),
as specified in the class definition. The dot operator directs the program to the
information encapsulated within an object.

|Example|
---------

.. code-block:: c++

   int i;

   DigitalOut led(13);

   void begin() {
     i = 0;
     led.on();
   }

   void step() {
     i++;
     if (i % 1000 == 0)
       led.toggle();
   }

|Syntax|
--------

.. code-block:: c++

   object.method()
   object.variable
