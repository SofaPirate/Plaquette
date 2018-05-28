#include <Plaquette.h>

// Simple compilation tests of operations.

AnalogIn aIn;
AnalogOut aOut;
DigitalIn dIn(3);
DigitalOut dOut;

#define TESTIF(expr) if (expr) {}
#define TESTEXPR(expr) (expr) >> aOut;

void begin() {}

void step() {
  // Test conditional expressions (uses bool() operator).
  TESTIF (aIn)
  TESTIF (!aIn)
  TESTIF (dIn)
  TESTIF (!dIn)
  TESTIF (aOut)
  TESTIF (!aOut)
  TESTIF (dOut)
  TESTIF (!dOut)

  TESTIF (aIn && dIn)
  TESTIF (aOut && dOut)
  TESTIF (aIn || dIn)
  TESTIF (aOut || dOut)

  // Test conditional expressions (uses float() operator).
  TESTIF (aIn > dIn)
  TESTIF (aOut > dOut)
  TESTIF (aIn > dIn)
  TESTIF (aOut > dOut)

  // Test arithmetic expressions.
  TESTEXPR(aIn+aOut)
  TESTEXPR(aIn-aOut)
  TESTEXPR(aIn*aOut)
  TESTEXPR(aIn/aOut)
  TESTEXPR(aIn+1)
  TESTEXPR(aIn-1)
  TESTEXPR(aIn*1.1)
  TESTEXPR(aIn/2)
  TESTEXPR(dIn+1)
  TESTEXPR(dIn-1)
  TESTEXPR(dIn*1.1)
  TESTEXPR(aOut/2)
  TESTEXPR(aOut+1)
  TESTEXPR(aOut-1)
  TESTEXPR(aOut*1.1)
  TESTEXPR(dOut/2)
  TESTEXPR(dOut+1)
  TESTEXPR(dOut-1)
  TESTEXPR(dOut*1.1)
  TESTEXPR(dOut/2)

}
