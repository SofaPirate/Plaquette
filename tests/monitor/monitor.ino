#include <Arduino.h>
#include <PlaquetteLib.h>
#include <MemoryStream.h>
#include <AUnit.h>

using namespace pq;

MemoryStream output;

Monitor monitor(output);

void clearOutput() {
  while (output.available())
    output.read();
}

test(putPrintsValue) {
  clearOutput();
  monitor.precision(2);
  1.5f >> monitor;
  assertEqual(monitor.get(), 1.5f);
  assertStringCaseEqual(output.readString(), "1.50");
}

test(precisionAppliesToPrint) {
  clearOutput();
  monitor.precision(3);
  monitor.print(3.14159);
  monitor.println(2.0);
  assertStringCaseEqual(output.readString(), "3.1422.000\r\n");
}

test(writeBytes) {
  clearOutput();

  // Literal 0 must resolve to an integer overload (not write(const char*)).
  monitor.write(0);
  monitor.write(65);
  monitor.write(66L);
  monitor.write(67u);
  monitor.write(68ul);
  monitor.write('E');
  monitor.write("FG");

  const char expected[] = { 0, 'A', 'B', 'C', 'D', 'E', 'F', 'G' };
  assertEqual(output.available(), (int)sizeof(expected));
  for (size_t i=0; i<sizeof(expected); i++)
    assertEqual(output.read(), (int)expected[i]);
}

void setup() {
  Plaquette.begin();
}

void loop() {
  aunit::TestRunner::run();
}
