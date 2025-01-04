#include <Arduino.h>
#include <PlaquetteLib.h>
#include <MemoryStream.h>
#include <AUnit.h>

using namespace pq;

MemoryStream input;
MemoryStream output;

StreamIn streamIn(input);
StreamOut streamOut(output);

void clearOutput() {
  while (output.available())
    output.read();
}

test(inputValidNumbers) {
  input.println("1 2 3 4");
  Plaquette.step();
  assertEqual(streamIn.get(), 1.0f);
  assertEqual(streamIn.get(), 2.0f);
  assertEqual(streamIn.get(), 3.0f);
  assertEqual(streamIn.get(), 4.0f);
}

test(inputValidNumbersLoop) {
  input.println("1 2 3 4");
  Plaquette.step();
  assertEqual(streamIn.get(), 1.0f);
  assertEqual(streamIn.get(), 2.0f);
  assertEqual(streamIn.get(), 3.0f);
  assertEqual(streamIn.get(), 4.0f);
}

test(inputValidDifferentSizes) {
  input.println("1 2 3 4");
  Plaquette.step();
  assertEqual(streamIn.get(), 1.0f);
  assertEqual(streamIn.get(), 2.0f);
  assertEqual(streamIn.get(), 3.0f);
  assertEqual(streamIn.get(), 4.0f);
  input.println("5 6 7");
  Plaquette.step();
  assertEqual(streamIn.get(), 5.0f);
  assertEqual(streamIn.get(), 6.0f);
  assertEqual(streamIn.get(), 7.0f);
  input.println("8 9 10 11 12");
  Plaquette.step();
  assertEqual(streamIn.get(), 8.0f);
  assertEqual(streamIn.get(), 9.0f);
  assertEqual(streamIn.get(), 10.0f);
  assertEqual(streamIn.get(), 11.0f);
  assertEqual(streamIn.get(), 12.0f);
}

test(inputValidNumbersExtraSpaces) {
  input.println("  1 2  3 4 ");
  Plaquette.step();
  assertEqual(streamIn.get(), 1.0f);
  assertEqual(streamIn.get(), 2.0f);
  assertEqual(streamIn.get(), 3.0f);
  assertEqual(streamIn.get(), 4.0f);
}

test(inputExtraBackslashR) {
  input.println("1 2 3 4\r");
  Plaquette.step();
  assertEqual(streamIn.get(), 1.0f);
  assertEqual(streamIn.get(), 2.0f);
  assertEqual(streamIn.get(), 3.0f);
  assertEqual(streamIn.get(), 4.0f);
}

test(inputInterruptMidWay) {
  input.println("1 2 3 4");
  input.print("5 6 7 ");
  Plaquette.step();
  assertEqual(streamIn.get(), 5.0f);
  assertEqual(streamIn.get(), 6.0f);
  assertEqual(streamIn.get(), 7.0f);
  assertEqual(streamIn.get(), 4.0f);
  input.println();
}

test(outputValid) {
  // FLush output.
  clearOutput();
  1 >> streamOut;
  2 >> streamOut;
  3 >> streamOut;
  4 >> streamOut;
  Plaquette.step();
  assertEqual(streamOut, 1.0);
  assertEqual(streamOut, 2.0);
  assertEqual(streamOut, 3.0);
  assertEqual(streamOut, 4.0);
  assertStringCaseEqual(output.readString(), "1 2 3 4\n");
}

test(outputInterruptMidWay) {
  // FLush output.
  clearOutput();
  1 >> streamOut;
  2 >> streamOut;
  3 >> streamOut;
  4 >> streamOut;
  Plaquette.step();
  clearOutput();
  5 >> streamOut;
  6 >> streamOut;
  7 >> streamOut;
  Plaquette.step();
  assertEqual(streamOut, 5.0);
  assertEqual(streamOut, 6.0);
  assertEqual(streamOut, 7.0);
  assertStringCaseEqual(output.readString(), "5 6 7\n");
}

test(inputToOutput) {
  input.println("1 2 3 4");
  Plaquette.step();

  // Flush output.
  clearOutput();

  for (int i=0; i<4; i++)
    streamIn >> streamOut;

  Plaquette.step();
  assertStringCaseEqual(output.readString(), "1 2 3 4\n");
}


void setup() {
  Plaquette.begin();
}

void loop() {
  aunit::TestRunner::run();
}
